//#include <assert.h>

#include "yl_mmu_bin_internal.h"
#include "yl_mmu_trunk_internal.h"

#include "yl_mmu_trunk_helper_link.h"



#ifdef __cplusplus
extern "C" {
#endif



/*********************** Helper ************************************/


static void 
_BinList_initialize_pivot_trunk(Trunk head, unsigned int size)
{
    head->size              = size;
    head->left_sibling      = head;
    head->right_sibling     = head;
    Trunk_set_use(          head );
    Trunk_set_pivot_trunk( head );
    Trunk_unset_pivot_bin( head );
    head->pre               = NULL;
    head->next              = NULL;
    head->index.all         = 0;
}


static void 
_BinList_initialize_pivot_bin(Trunk head, unsigned int size)
{
    _BinList_initialize_pivot_trunk(head, size);

    Trunk_unset_pivot_trunk( head ); // To work as pivot in some case
    Trunk_set_pivot_bin( head );
}


static void
BinList_initialize_single( BinRec bins[] )
{
    {
        Bin current_bin     = bins;
        unsigned int size   = MIN_ADDRESSING_SIZE;      // 4k
        unsigned int upper  = MAX_ADDRESSING_SIZE;      // 256M

        /// Strange way to prevent overflow
        size = size >> 1;

        while( 1  )
        {
            size = size << 1;

            _BinList_initialize_pivot_trunk( & current_bin->trunk_head, size);

            ++current_bin;
            if ( size >= upper ) break;
        }

        // Pointer is initialized as a non-null, but recognizable value
        _BinList_initialize_pivot_bin( & current_bin->trunk_head, UINT_MAX );
    }
}


static void
BinList_release_single( BinRec bins[] )
{
    {
        Bin current_bin = bins;
        unsigned int size = MIN_ADDRESSING_SIZE;    // 4k
        unsigned int upper = MAX_ADDRESSING_SIZE;   // 256M

        /// Strange way to prevent overflow
        size = size >> 1;

        while( 1  )
        {
            size = size << 1;

            YL_MMU_ASSERT( current_bin->trunk_head.size == size );

            _BinList_initialize_pivot_trunk( & current_bin->trunk_head, size);

            ++current_bin;
            if ( size >= upper ) break;
        }

        // Don't release pivot!
        YL_MMU_ASSERT( current_bin->trunk_head.size == UINT_MAX );
        _BinList_initialize_pivot_bin( & current_bin->trunk_head, UINT_MAX );
    }
}


/*********************** API ************************************/

void 
BinList_initialize( BinList self )
{
    {
        //Trunk bin_trunk = NULL;
        Trunk t         = NULL;

        BinList_initialize_single( self->empty    );
        //BinList_initialize_single( self->occupied );  // Mindos occupied

        //bin_trunk   = & ( self->empty[BIN_LAST_INDEX].trunk_head );
        t           = Trunk_create_first_trunk_and_pivot( MAX_ADDRESSING_SIZE );

        //Trunk_siblings_connect( bin_trunk, t );
        //Trunk_siblings_connect( t, bin_trunk );

        self->first_trunk = t;
    }
}


void 
BinList_release( BinList self )
{
    Trunk first_trunk = self->first_trunk;
    self->first_trunk = NULL;

    BinList_release_single( self->empty     );
    //BinList_release_single( self->occupied  );  // Mindos occupied

    Trunk_release_list(first_trunk);
}



Bin 
BinList_floor( BinRec bins[], size_t size )
{
    int index = BinList_index_floor( size ); // start index

    YL_MMU_ASSERT( (index >= 0) && ((unsigned)index < BIN_LENGTH) );

    return bins + index;
}

#if 0 //unused function
static Bin 
BinList_floor_section( BinRec bins[], size_t size )
{
    int index = BinList_index_floor_section( size ); // start index

    YL_MMU_ASSERT( index >= 0 && index < BIN_LENGTH );

    return bins + index;
}
#endif

static void 
BinList_rounding( BinRec bins[], size_t size, Bin *bin_floor, Bin *bin_ceiling )
{
    int index_floor, index_ceiling;
        
    BinList_index_rounding( size, &index_floor, &index_ceiling);

    YL_MMU_ASSERT( (index_floor >= 0) && ((unsigned)index_ceiling < BIN_LENGTH) );
    //YL_MMU_ASSERT( index_floor <= size && size <= index_ceiling );

    *bin_floor   = bins + index_floor;
    *bin_ceiling = bins + index_ceiling;
}

//Bin 
//BinList_ceiling( BinRec bins[], size_t size, size_t *out_bin_size )
//{
//    int index = index_ceiling( size, out_bin_size ); // start index
//
//    YL_MMU_ASSERT( index >= 0 && index < BIN_LENGTH );
//
//    return bins + index;
//}


//Bin 
//BinList_ceiling_section( BinRec bins[], size_t size, size_t *out_bin_size )
//{
//    int index = index_ceiling_section( size, out_bin_size ); // start index
//    
//    YL_MMU_ASSERT( index >= 0 && index < BIN_LENGTH );
//
//    return bins + index;
//}


Trunk 
BinList_scan_for_first_empty_trunk( BinList bin_list, size_t size )
{
    Bin bin_floor, bin_ceiling, bin;
    Trunk t;
    //size_t start_size;

    BinList_rounding( bin_list->empty, size, &bin_floor, &bin_ceiling );

    bin = bin_ceiling;

    // Shouldn't go that far if the size passed in is limited.    
    YL_MMU_ASSERT( ! Trunk_is_pivot_bin( Bin_first_trunk( bin ) ) ); 

    // 1. search for other (definity enough) bin (for speed)
    while ( Trunk_is_pivot_trunk( Bin_first_trunk( bin ) ) )
    {
        ++bin;
    }

    if ( ! Trunk_is_pivot_bin( & bin->trunk_head ) )
    {
        return Bin_first_trunk( bin );
    }

    // All bins above ceiling are scaned but not found.

    if ( bin_floor == bin_ceiling )
    {
        // Nothing left.
        return NULL;
    }

    // 2. Try to find the suitable item in the ``floor'' bin
    for ( t = Bin_first_trunk( bin_floor ); ! Trunk_is_pivot_trunk( t ); t = t->right_sibling )
    {
        if ( size <= t->size )
        {
            YL_MMU_ASSERT( ! Trunk_is_pivot_trunk( t ) );
            YL_MMU_ASSERT( ! Trunk_is_pivot_bin( t )   );
            YL_MMU_ASSERT( ! Trunk_in_use(t)           );

            return t;
        }
    }

    return NULL;
}



// We ''Waste'' a section after this kind of allocation.
// But this kind of hole actually facilicate small page packing.
Trunk 
BinList_scan_for_empty_trunk_section_fit( BinList bin_list, size_t size, unsigned short index1_start, unsigned short index1_tail )
{
    size_t size_expanded = size + MMU_SECTION_SIZE;

    return BinList_scan_for_first_empty_trunk( bin_list, size_expanded );
}


// trunk = original_bin->first
//void 
//BinList_adjust_sunken_empty_trunk( BinList bin_list, Bin original_bin )
//{
//    {
//        size_t bin_size     = original_bin->trunk_head.size;
//        Trunk t             = Bin_first_trunk( original_bin );
//        size_t trunk_size   = t->size;
//        Trunk t2            = NULL;
//
//
//        // Just can not smaller than min size
//        // no smaller bin avaliable, so put things here no matter how small
//        if ( bin_size == MIN_ADDRESSING_SIZE ) 
//            return ;
//
//        // Though the trunk becomes smaller, it still falls in this bin
//        if ( trunk_size >= bin_size )
//        {
//            // sunken can't become bigger, unless something wrong
//            YL_MMU_ASSERT( trunk_size < bin_size * 2 );
//            return ;
//        }
//
//        // We have no choice but cut down the trunk
//        t2 = Bin_cut_down_first_trunk( original_bin );
//
//        YL_MMU_ASSERT( t2 == t             );
//        YL_MMU_ASSERT( ! Trunk_in_use(t)   );
//        YL_MMU_ASSERT( ! t->is_pivot_trunk );
//        YL_MMU_ASSERT( ! t->is_pivot_bin   );
//
//        BinList_attach_trunk_to_empty_list( bin_list, t );
//    }
//}

void 
BinList_attach_trunk_to_empty_list( BinList bin_list, Trunk trunk )
{
    Trunk_unset_use( trunk );

    _BinList_attach_trunk( bin_list->empty, trunk );
}

void 
BinList_attach_trunk_to_occupied_list( BinList bin_list, Trunk trunk )
{
    Trunk_set_use( trunk );

    //_BinList_attach_trunk( bin_list->occupied, trunk );  // Mindos occupied
}

void 
_BinList_attach_trunk( BinRec bins[], Trunk t )
{
    YL_MMU_ASSERT( t->size > 0 );

    {
        Bin bin = BinList_floor( bins, t->size );
        Trunk_siblings_insert_after( & bin->trunk_head, t );
    }
}



/*********************** Bin API ************************************/
Trunk
Bin_cut_down_first_trunk( Bin bin )
{
    YL_MMU_ASSERT( bin != NULL );


    {
        Trunk t = Bin_first_trunk( bin );

        YL_MMU_ASSERT( t != & bin->trunk_head );

        Trunk_siblings_connect( & bin->trunk_head, t->right_sibling );
        Trunk_siblings_reset( t );  // not necessary

        return t;
    }
}



#ifdef __cplusplus
}
#endif

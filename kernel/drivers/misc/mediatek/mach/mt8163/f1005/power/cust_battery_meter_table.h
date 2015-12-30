#ifndef _CUST_BATTERY_METER_TABLE_H
#define _CUST_BATTERY_METER_TABLE_H

#include <mach/mt_typedefs.h>

// ============================================================
// define
// ============================================================
#define BAT_NTC_10 1
#define BAT_NTC_47 0
#define BAT_NTC_100 0

#if (BAT_NTC_10 == 1)
#define RBAT_PULL_UP_R             16900
#define RBAT_PULL_DOWN_R           30000
#endif
#if (BAT_NTC_47 == 1)
#define RBAT_PULL_UP_R             61900
#define RBAT_PULL_DOWN_R           100000
#endif
#if (BAT_NTC_100 == 1)
#define RBAT_PULL_UP_R             24000
#define RBAT_PULL_DOWN_R           100000000
#endif
#define RBAT_PULL_UP_VOLT          1800


// ============================================================
// ENUM
// ============================================================

// ============================================================
// structure
// ============================================================

// ============================================================
// typedef
// ============================================================
typedef struct _BATTERY_PROFILE_STRUC
{
    kal_int32 percentage;
    kal_int32 voltage;
} BATTERY_PROFILE_STRUC, *BATTERY_PROFILE_STRUC_P;

typedef struct _R_PROFILE_STRUC
{
    kal_int32 resistance; // Ohm
    kal_int32 voltage;
} R_PROFILE_STRUC, *R_PROFILE_STRUC_P;

typedef enum
{
    T1_0C,
    T2_25C,
    T3_50C
} PROFILE_TEMPERATURE;

// ============================================================
// External Variables
// ============================================================

// ============================================================
// External function
// ============================================================

// ============================================================
// <DOD, Battery_Voltage> Table
// ============================================================
#if (BAT_NTC_10 == 1)
    BATT_TEMPERATURE Batt_Temperature_Table[] = {
		{-20 , 75022},
		{-15 , 57926},
		{-10 , 45168},
		{ -5 , 35548},
		{  0 , 28224},
		{  5 , 22595},
		{ 10 , 18231},
		{ 15 , 14820},
		{ 20 , 12133},
		{ 25 , 10000},
		{ 30 , 8295},
		{ 35 , 6922},
		{ 40 , 5810},
		{ 45 , 4903},
		{ 50 , 4160},
		{ 55 , 3547},
		{ 60 , 3039}
	};
#endif

#if (BAT_NTC_47 == 1)
    BATT_TEMPERATURE Batt_Temperature_Table[] = {
        {-20,483954},
        {-15,360850},
        {-10,271697},
        { -5,206463},
        {  0,158214},
        {  5,122259},
        { 10,95227},
        { 15,74730},
        { 20,59065},
        { 25,47000},
        { 30,37643},
        { 35,30334},
        { 40,24591},
        { 45,20048},
        { 50,16433},
        { 55,13539},
        { 60,11210}        
    };
#endif

#if (BAT_NTC_100 == 1)
	BATT_TEMPERATURE Batt_Temperature_Table[] = {
		{-20,1151037},
		{-15,846579},
		{-10,628988},
		{ -5,471632},
		{  0,357012},
		{  5,272500},
		{ 10,209710},
		{ 15,162651},
		{ 20,127080},
		{ 25,100000},
		{ 30,79222},
		{ 35,63167},
		{ 40,50677},
		{ 45,40904},
		{ 50,33195},
		{ 55,27091},
		{ 60,22224}
	};
#endif
// T0 -10C
BATTERY_PROFILE_STRUC battery_profile_t0[] =
{
    {0  , 4327},
    {1  , 4274},
    {3  , 4251},
    {4  , 4232},
    {5  , 4214},
    {7  , 4198},
    {8  , 4182},
    {9	, 4167},
    {11 , 4152},
    {12 , 4138},
    {13 , 4124},
    {15 , 4110},
    {16 , 4096},
    {17 , 4082},
    {19 , 4069},
    {20 , 4058},
    {21 , 4040},
    {23 , 4028},
    {24 , 4019},
    {25 , 3997},
    {27 , 3981},
    {28 , 3969},
    {29 , 3961},
    {31 , 3954},
    {32 , 3948},
    {33 , 3939},
    {35 , 3929},
    {36 , 3919},
    {37 , 3909},
    {39 , 3899},
    {40 , 3889},
    {41 , 3876},
    {43 , 3864},
    {44 , 3851},
    {45 , 3840},
    {47 , 3830},
    {48 , 3822},
    {49 , 3814},
    {51 , 3807},
    {52 , 3802},
    {53 , 3797},
    {55 , 3792},
    {56 , 3788},
    {57 , 3783},
    {59 , 3779},
    {60 , 3775},
    {61 , 3772},
    {63 , 3768},
    {64 , 3766},
    {65 , 3763},
    {67 , 3755},
    {68 , 3749},
    {69 , 3745},
    {71 , 3743},
    {72 , 3735},
    {73 , 3728},
    {75 , 3725},
    {76 , 3720},
    {77 , 3715},
    {79 , 3701},
    {80 , 3695},
    {81 , 3689},
    {83 , 3675},
    {84 , 3661},
    {85 , 3659},
    {87 , 3635},
    {88 , 3627},
    {89 , 3623},
    {91 , 3615},
    {92 , 3606},
    {93 , 3595},
    {95 , 3585},
    {96 , 3568},
    {97 , 3556},
    {99 , 3506},
    {100, 3450}
};

// T1 0C
BATTERY_PROFILE_STRUC battery_profile_t1[] =
{
    {0  , 4327},
    {1  , 4274},
    {3  , 4251},
    {4  , 4232},
    {5  , 4214},
    {7  , 4198},
    {8  , 4182},
    {9	, 4167},
    {11 , 4152},
    {12 , 4138},
    {13 , 4124},
    {15 , 4110},
    {16 , 4096},
    {17 , 4082},
    {19 , 4069},
    {20 , 4058},
    {21 , 4040},
    {23 , 4028},
    {24 , 4019},
    {25 , 3997},
    {27 , 3981},
    {28 , 3969},
    {29 , 3961},
    {31 , 3954},
    {32 , 3948},
    {33 , 3939},
    {35 , 3929},
    {36 , 3919},
    {37 , 3909},
    {39 , 3899},
    {40 , 3889},
    {41 , 3876},
    {43 , 3864},
    {44 , 3851},
    {45 , 3840},
    {47 , 3830},
    {48 , 3822},
    {49 , 3814},
    {51 , 3807},
    {52 , 3802},
    {53 , 3797},
    {55 , 3792},
    {56 , 3788},
    {57 , 3783},
    {59 , 3779},
    {60 , 3775},
    {61 , 3772},
    {63 , 3768},
    {64 , 3766},
    {65 , 3763},
    {67 , 3755},
    {68 , 3749},
    {69 , 3745},
    {71 , 3743},
    {72 , 3735},
    {73 , 3728},
    {75 , 3725},
    {76 , 3720},
    {77 , 3715},
    {79 , 3701},
    {80 , 3695},
    {81 , 3689},
    {83 , 3675},
    {84 , 3661},
    {85 , 3659},
    {87 , 3635},
    {88 , 3627},
    {89 , 3623},
    {91 , 3615},
    {92 , 3606},
    {93 , 3595},
    {95 , 3585},
    {96 , 3568},
    {97 , 3556},
    {99 , 3506},
    {100, 3450}
};

// T2 25C
BATTERY_PROFILE_STRUC battery_profile_t2[] =
{
    {0  , 4327},
    {1  , 4274},
    {3  , 4251},
    {4  , 4232},
    {5  , 4214},
    {7  , 4198},
    {8  , 4182},
    {9	, 4167},
    {11 , 4152},
    {12 , 4138},
    {13 , 4124},
    {15 , 4110},
    {16 , 4096},
    {17 , 4082},
    {19 , 4069},
    {20 , 4058},
    {21 , 4040},
    {23 , 4028},
    {24 , 4019},
    {25 , 3997},
    {27 , 3981},
    {28 , 3969},
    {29 , 3961},
    {31 , 3954},
    {32 , 3948},
    {33 , 3939},
    {35 , 3929},
    {36 , 3919},
    {37 , 3909},
    {39 , 3899},
    {40 , 3889},
    {41 , 3876},
    {43 , 3864},
    {44 , 3851},
    {45 , 3840},
    {47 , 3830},
    {48 , 3822},
    {49 , 3814},
    {51 , 3807},
    {52 , 3802},
    {53 , 3797},
    {55 , 3792},
    {56 , 3788},
    {57 , 3783},
    {59 , 3779},
    {60 , 3775},
    {61 , 3772},
    {63 , 3768},
    {64 , 3766},
    {65 , 3763},
    {67 , 3755},
    {68 , 3749},
    {69 , 3745},
    {71 , 3743},
    {72 , 3735},
    {73 , 3728},
    {75 , 3725},
    {76 , 3720},
    {77 , 3715},
    {79 , 3701},
    {80 , 3695},
    {81 , 3689},
    {83 , 3675},
    {84 , 3661},
    {85 , 3659},
    {87 , 3635},
    {88 , 3627},
    {89 , 3623},
    {91 , 3615},
    {92 , 3606},
    {93 , 3595},
    {95 , 3585},
    {96 , 3568},
    {97 , 3556},
    {99 , 3506},
    {100, 3450}
};
// T3 50C
BATTERY_PROFILE_STRUC battery_profile_t3[] =
{
    {0  , 4327},
    {1  , 4274},
    {3  , 4251},
    {4  , 4232},
    {5  , 4214},
    {7  , 4198},
    {8  , 4182},
    {9	, 4167},
    {11 , 4152},
    {12 , 4138},
    {13 , 4124},
    {15 , 4110},
    {16 , 4096},
    {17 , 4082},
    {19 , 4069},
    {20 , 4058},
    {21 , 4040},
    {23 , 4028},
    {24 , 4019},
    {25 , 3997},
    {27 , 3981},
    {28 , 3969},
    {29 , 3961},
    {31 , 3954},
    {32 , 3948},
    {33 , 3939},
    {35 , 3929},
    {36 , 3919},
    {37 , 3909},
    {39 , 3899},
    {40 , 3889},
    {41 , 3876},
    {43 , 3864},
    {44 , 3851},
    {45 , 3840},
    {47 , 3830},
    {48 , 3822},
    {49 , 3814},
    {51 , 3807},
    {52 , 3802},
    {53 , 3797},
    {55 , 3792},
    {56 , 3788},
    {57 , 3783},
    {59 , 3779},
    {60 , 3775},
    {61 , 3772},
    {63 , 3768},
    {64 , 3766},
    {65 , 3763},
    {67 , 3755},
    {68 , 3749},
    {69 , 3745},
    {71 , 3743},
    {72 , 3735},
    {73 , 3728},
    {75 , 3725},
    {76 , 3720},
    {77 , 3715},
    {79 , 3701},
    {80 , 3695},
    {81 , 3689},
    {83 , 3675},
    {84 , 3661},
    {85 , 3659},
    {87 , 3635},
    {88 , 3627},
    {89 , 3623},
    {91 , 3615},
    {92 , 3606},
    {93 , 3595},
    {95 , 3585},
    {96 , 3568},
    {97 , 3556},
    {99 , 3506},
    {100, 3450}
};

// battery profile for actual temperature. The size should be the same as T1, T2 and T3
BATTERY_PROFILE_STRUC battery_profile_temperature[] =
{
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
	{0, 0},
	{0, 0},
	{0, 0}
};

// ============================================================
// <Rbat, Battery_Voltage> Table
// ============================================================
// T0 -10C
R_PROFILE_STRUC r_profile_t0[] =
{
    {118 , 4327},
    {118 , 4274},
    {120 , 4251},
    {122 , 4232},
    {120 , 4214},
    {122 , 4198},
    {122 , 4182},
    {123 , 4167},
    {123 , 4152},
    {125 , 4138},
    {125 , 4124},
    {125 , 4110},
    {120 , 4096},
    {122 , 4082},
    {123 , 4069},
    {125 , 4058},
    {127 , 4040},
    {127 , 4028},
    {128 , 4019},
    {127 , 3997},
    {130 , 3981},
    {132 , 3969},
    {133 , 3961},
    {135 , 3954},
    {138 , 3948},
    {140 , 3939},
    {143 , 3929},
    {143 , 3919},
    {143 , 3909},
    {143 , 3899},
    {145 , 3889},
    {138 , 3876},
    {132 , 3864},
    {125 , 3851},
    {120 , 3840},
    {115 , 3830},
    {113 , 3822},
    {110 , 3814},
    {108 , 3807},
    {110 , 3802},
    {112 , 3797},
    {113 , 3792},
    {115 , 3788},
    {113 , 3783},
    {115 , 3779},
    {115 , 3775},
    {117 , 3772},
    {117 , 3768},
    {120 , 3766},
    {118 , 3763},
    {120 , 3755},
    {118 , 3749},
    {117 , 3745},
    {117 , 3743},
    {115 , 3735},
    {115 , 3728},
    {113 , 3725},
    {112 , 3720},
    {112 , 3715},
    {113 , 3701},
    {112 , 3695},
    {112 , 3689},
    {112 , 3675},
    {112 , 3661},
    {112 , 3659},
    {112 , 3635},
    {113 , 3627},
    {112 , 3623},
    {112 , 3615},
    {112 , 3606},
    {117 , 3595},
    {118 , 3585},
    {118 , 3568},
    {123 , 3556},
    {130 , 3516},
    {158 , 3450}
};

// T1 0C
R_PROFILE_STRUC r_profile_t1[] =
{
    {118 , 4327},
    {118 , 4274},
    {120 , 4251},
    {122 , 4232},
    {120 , 4214},
    {122 , 4198},
    {122 , 4182},
    {123 , 4167},
    {123 , 4152},
    {125 , 4138},
    {125 , 4124},
    {125 , 4110},
    {120 , 4096},
    {122 , 4082},
    {123 , 4069},
    {125 , 4058},
    {127 , 4040},
    {127 , 4028},
    {128 , 4019},
    {127 , 3997},
    {130 , 3981},
    {132 , 3969},
    {133 , 3961},
    {135 , 3954},
    {138 , 3948},
    {140 , 3939},
    {143 , 3929},
    {143 , 3919},
    {143 , 3909},
    {143 , 3899},
    {145 , 3889},
    {138 , 3876},
    {132 , 3864},
    {125 , 3851},
    {120 , 3840},
    {115 , 3830},
    {113 , 3822},
    {110 , 3814},
    {108 , 3807},
    {110 , 3802},
    {112 , 3797},
    {113 , 3792},
    {115 , 3788},
    {113 , 3783},
    {115 , 3779},
    {115 , 3775},
    {117 , 3772},
    {117 , 3768},
    {120 , 3766},
    {118 , 3763},
    {120 , 3755},
    {118 , 3749},
    {117 , 3745},
    {117 , 3743},
    {115 , 3735},
    {115 , 3728},
    {113 , 3725},
    {112 , 3720},
    {112 , 3715},
    {113 , 3701},
    {112 , 3695},
    {112 , 3689},
    {112 , 3675},
    {112 , 3661},
    {112 , 3659},
    {112 , 3635},
    {113 , 3627},
    {112 , 3623},
    {112 , 3615},
    {112 , 3606},
    {117 , 3595},
    {118 , 3585},
    {118 , 3568},
    {123 , 3556},
    {130 , 3516},
    {158 , 3450}
};

// T2 25C
R_PROFILE_STRUC r_profile_t2[] =
{
    {118 , 4327},
    {118 , 4274},
    {120 , 4251},
    {122 , 4232},
    {120 , 4214},
    {122 , 4198},
    {122 , 4182},
    {123 , 4167},
    {123 , 4152},
    {125 , 4138},
    {125 , 4124},
    {125 , 4110},
    {120 , 4096},
    {122 , 4082},
    {123 , 4069},
    {125 , 4058},
    {127 , 4040},
    {127 , 4028},
    {128 , 4019},
    {127 , 3997},
    {130 , 3981},
    {132 , 3969},
    {133 , 3961},
    {135 , 3954},
    {138 , 3948},
    {140 , 3939},
    {143 , 3929},
    {143 , 3919},
    {143 , 3909},
    {143 , 3899},
    {145 , 3889},
    {138 , 3876},
    {132 , 3864},
    {125 , 3851},
    {120 , 3840},
    {115 , 3830},
    {113 , 3822},
    {110 , 3814},
    {108 , 3807},
    {110 , 3802},
    {112 , 3797},
    {113 , 3792},
    {115 , 3788},
    {113 , 3783},
    {115 , 3779},
    {115 , 3775},
    {117 , 3772},
    {117 , 3768},
    {120 , 3766},
    {118 , 3763},
    {120 , 3755},
    {118 , 3749},
    {117 , 3745},
    {117 , 3743},
    {115 , 3735},
    {115 , 3728},
    {113 , 3725},
    {112 , 3720},
    {112 , 3715},
    {113 , 3701},
    {112 , 3695},
    {112 , 3689},
    {112 , 3675},
    {112 , 3661},
    {112 , 3659},
    {112 , 3635},
    {113 , 3627},
    {112 , 3623},
    {112 , 3615},
    {112 , 3606},
    {117 , 3595},
    {118 , 3585},
    {118 , 3568},
    {123 , 3556},
    {130 , 3516},
    {158 , 3450}
};

// T3 50C
R_PROFILE_STRUC r_profile_t3[] =
{
    {118 , 4327},
    {118 , 4274},
    {120 , 4251},
    {122 , 4232},
    {120 , 4214},
    {122 , 4198},
    {122 , 4182},
    {123 , 4167},
    {123 , 4152},
    {125 , 4138},
    {125 , 4124},
    {125 , 4110},
    {120 , 4096},
    {122 , 4082},
    {123 , 4069},
    {125 , 4058},
    {127 , 4040},
    {127 , 4028},
    {128 , 4019},
    {127 , 3997},
    {130 , 3981},
    {132 , 3969},
    {133 , 3961},
    {135 , 3954},
    {138 , 3948},
    {140 , 3939},
    {143 , 3929},
    {143 , 3919},
    {143 , 3909},
    {143 , 3899},
    {145 , 3889},
    {138 , 3876},
    {132 , 3864},
    {125 , 3851},
    {120 , 3840},
    {115 , 3830},
    {113 , 3822},
    {110 , 3814},
    {108 , 3807},
    {110 , 3802},
    {112 , 3797},
    {113 , 3792},
    {115 , 3788},
    {113 , 3783},
    {115 , 3779},
    {115 , 3775},
    {117 , 3772},
    {117 , 3768},
    {120 , 3766},
    {118 , 3763},
    {120 , 3755},
    {118 , 3749},
    {117 , 3745},
    {117 , 3743},
    {115 , 3735},
    {115 , 3728},
    {113 , 3725},
    {112 , 3720},
    {112 , 3715},
    {113 , 3701},
    {112 , 3695},
    {112 , 3689},
    {112 , 3675},
    {112 , 3661},
    {112 , 3659},
    {112 , 3635},
    {113 , 3627},
    {112 , 3623},
    {112 , 3615},
    {112 , 3606},
    {117 , 3595},
    {118 , 3585},
    {118 , 3568},
    {123 , 3556},
    {130 , 3516},
    {158 , 3450}
};

// r-table profile for actual temperature. The size should be the same as T1, T2 and T3
R_PROFILE_STRUC r_profile_temperature[] =
{
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0}
};

// ============================================================
// function prototype
// ============================================================
int fgauge_get_saddles(void);
BATTERY_PROFILE_STRUC_P fgauge_get_profile(kal_uint32 temperature);

int fgauge_get_saddles_r_table(void);
R_PROFILE_STRUC_P fgauge_get_profile_r_table(kal_uint32 temperature);

#endif	//#ifndef _CUST_BATTERY_METER_TABLE_H


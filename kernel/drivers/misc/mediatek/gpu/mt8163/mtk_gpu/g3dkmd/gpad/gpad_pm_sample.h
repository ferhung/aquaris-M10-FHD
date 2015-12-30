#ifndef _GPAD_PM_SAMPLE_H
#define _GPAD_PM_SAMPLE_H

#define GPAD_PM_COUNTER_CNT        32 /**< 32 PM counters. */
#define GPAD_PM_HW_BUFFER_ALIGN    32 /**< Aligned on 32-byte boundary. */

struct gpad_pm_header {
    /* 0 */
    unsigned short  ff_mod_sel;
    unsigned short  central_sel;
    /* 1 */
    unsigned int    group_sel;
    /* 2 */
    unsigned int    saturation;
    /* 3 */
    unsigned int    timestamp;
    /* 4 */
    unsigned int    frame_id;
    /* 5 */
    unsigned int    subframe_id;
    /* 6 */
    unsigned int    segment_id;
    /* 7 */
    unsigned char   mode[3];
    unsigned char   tag;
};

struct gpad_pm_sample {
    unsigned int            data[GPAD_PM_COUNTER_CNT];
    struct gpad_pm_header   header;
};

#endif /* _GPAD_PM_SAMPLE_H */

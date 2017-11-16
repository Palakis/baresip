#ifndef V23_DSP_H
#define V23_DSP_H

#define PHASE_BITS 16
#define PHASE_BASE (1 << PHASE_BITS)

#define COS_BITS 14
#define COS_BASE (1 << COS_BITS)

#define COS_TABLE_BITS 13
#define COS_TABLE_SIZE (1 << COS_TABLE_BITS)

#define M_PI 3.14159265358979323846264338327

#include <math.h>
#include <re.h>

extern int16_t cos_tab[COS_TABLE_SIZE];

void dsp_init(void);

static inline int16_t dsp_cos(int16_t phase)
{
    return cos_tab[(phase >> (PHASE_BITS - COS_TABLE_BITS)) & (COS_TABLE_SIZE-1)];
}

#endif // V23_DSP_H
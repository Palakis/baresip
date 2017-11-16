#ifndef V23_SINE_H
#define V23_SINE_H

#define COS_BITS 14
#define COS_BASE (1 << COS_BITS)

#define COS_TABLE_BITS 13
#define COS_TABLE_SIZE (1 << COS_TABLE_BITS)

#include <re.h>

struct sine_t {
    uint32_t freq;
    uint32_t srate;
	int phase;
    int omega;
    int16_t cos_tab[COS_TABLE_SIZE];
};

struct sine_t* sine_alloc(void);
void sine_generate(struct sine_t *st, int16_t *sampv, size_t sampc,
                   uint32_t freq, uint32_t srate);

#endif // V23_SINE_H
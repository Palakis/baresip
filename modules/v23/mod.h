#ifndef V23_MOD_H
#define V23_MOD_H

#include <re.h>

struct sine_t {
    uint32_t freq;
    uint32_t srate;
	int phase;
    int omega;
};

struct sine_t* sine_alloc(void);
void sine_generate(struct sine_t *st, int16_t *sampv, size_t sampc,
                   uint32_t freq, uint32_t srate);

#endif // V23_SINE_H
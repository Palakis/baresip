#define PHASE_BITS 16
#define PHASE_BASE (1 << PHASE_BITS)
#define M_PI 3.14159265358979323846264338327

#include <math.h>

#include "sine.h"

static void sine_dealloc(void *arg)
{
	struct sine_t* st = (struct sine_t*)arg;
	(void)st;
}

struct sine_t* sine_alloc(void)
{
	struct sine_t* st;
	int i;

	st = mem_zalloc(sizeof(*st), sine_dealloc);
	if(!st)
		return NULL;

	for (i = 0; i < COS_TABLE_SIZE; i++) {
		st->cos_tab[i] = (int16_t) (cos(2 * M_PI * i / COS_TABLE_SIZE) * COS_BASE);
	}

	return st;
}

void sine_generate(struct sine_t *st, int16_t *sampv, size_t sampc,
						uint32_t freq, uint32_t srate)
{
	if(freq != st->freq || srate != st->srate) {
		st->omega = (PHASE_BASE * freq) / srate;
		st->phase = 0;

		st->freq = freq;
		st->srate = srate;		
	}

	int phase;
	size_t i;

	phase = st->phase;

	for (i = 0; i < sampc; i++) {
		*(sampv + i) = st->cos_tab[(phase >> (PHASE_BITS - COS_TABLE_BITS)) & (COS_TABLE_SIZE-1)];
		phase += st->omega;
	}

	st->phase = phase;
}
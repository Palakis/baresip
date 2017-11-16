#include "dsp.h"
#include "mod.h"

static void sine_dealloc(void *arg)
{
	struct sine_t* st = (struct sine_t*)arg;
	(void)st;
}

struct sine_t* sine_alloc(void)
{
	struct sine_t* st;
	st = mem_zalloc(sizeof(*st), sine_dealloc);
	if(!st)
		return NULL;

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
		*(sampv + i) = dsp_cos(phase);
		phase += st->omega;
	}

	st->phase = phase;
}
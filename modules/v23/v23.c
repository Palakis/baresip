/**
 * @file v23.c V.23 modem module
 */
#include <re.h>
#include <baresip.h>
#include "dsp.h"

#include "v23.h"

#define CARRIER_THRESHOLD 16439.10631

/**
 * V23 modem implementation as a filter
 * NOTE: very experimental
 */

static void state_destructor(void *arg)
{
	struct v23_t *st = (struct v23_t*)arg;
	(void)st;
}

static void enc_destructor(void *arg)
{
	struct v23modem_enc *st = (struct v23modem_enc*)arg;
	mem_deref(st->v23);
	mem_deref(st->sine_gen);
}

static void dec_destructor(void *arg)
{
	struct v23modem_dec *st = (struct v23modem_dec*)arg;
	mem_deref(st->v23);
}

static int encode_update(struct aufilt_enc_st **stp, void **ctx,
						 const struct aufilt *af, struct aufilt_prm *prm)
{
	struct v23modem_enc *st;
	struct v23_t *common_st;

	if (!stp || !af || !prm)
		return EINVAL;

	common_st = mem_zalloc(sizeof(*common_st), state_destructor);
	if (!common_st)
		return ENOMEM;

	st = mem_zalloc(sizeof(*st), enc_destructor);
	if (!st)
		return ENOMEM;

	st->v23 = common_st;

	st->srate = prm->srate;
	st->baud_rate = 1200;
	st->ns_counter = 0;

	st->v23->conn_state = V23_HOOK;
	st->hook_state = HOOK_NONE;

	st->sine_gen = sine_alloc();

	*stp = (struct aufilt_enc_st*)st;

	// encode_update is called before decode_update in
	// in the same scope, with ctx's value accessible from
	// these two handlers.
	// It's then possible to initialize a common context in
	// encode_update, store its pointer in ctx, and fetch it
	// again in decode_update.
	*ctx = common_st;

	return 0;
}

static int decode_update(struct aufilt_dec_st **stp, void **ctx,
						 const struct aufilt *af, struct aufilt_prm *prm)
{
	struct v23modem_dec *st;

	if (!stp || !af || !prm)
		return EINVAL;

	st = mem_zalloc(sizeof(*st), dec_destructor);
	if (!st)
		return ENOMEM;

	st->v23 = (struct v23_t*)(*ctx);
	mem_ref(st->v23);

	st->srate = prm->srate;
	st->baud_rate = 75;
	st->carrier_ns = 0;

	goertzel_init(&st->gtz_hi, 390, st->srate);
	goertzel_init(&st->gtz_lo, 450, st->srate);

	*stp = (struct aufilt_dec_st*)st;

	return 0;
}

static uint64_t ms_to_ns(uint64_t ms) {
    return ms * 1000000;
}

static int encode(struct aufilt_enc_st *st, int16_t *sampv, size_t *sampc)
{
	struct v23modem_enc *ctx = (struct v23modem_enc*)st;

	uint32_t freq = 0;
	size_t i, samples = *sampc;
	
	// in nanoseconds
	uint64_t symbol_interval = 1000000000 / ctx->baud_rate;

	for(i = 0; i < samples; i++) {
		if (ctx->v23->conn_state == V23_HOOK) {
			if (ctx->hook_state == HOOK_FREQ_2100 && ctx->ns_counter >= ms_to_ns(3000)) {
				info("HOOKUP_SILENCE\n");
				ctx->hook_state = HOOK_SILENCE;
				ctx->ns_counter = 0;
			}
			else if (ctx->hook_state == HOOK_SILENCE && ctx->ns_counter >= ms_to_ns(75)) {
				info("HOOKUP_FREQ_1300\n");
				ctx->hook_state = HOOK_FREQ_1300;
				ctx->ns_counter = 0;
			}
			else if (
				(ctx->hook_state == HOOK_FREQ_1300 && ctx->ns_counter >= ms_to_ns(60))
					|| ctx->hook_state == HOOK_NONE
			) {
				info("HOOKUP_FREQ_2100\n");
				ctx->hook_state = HOOK_FREQ_2100;
				ctx->ns_counter = 0;
			}
	
			switch(ctx->hook_state) {
				case HOOK_FREQ_2100:
					freq = 2100;
					break;
	
				case HOOK_FREQ_1300:
					freq = 1300;
					break;
	
				case HOOK_SILENCE:
				default:
					freq = 0;
					break;
			}
		}
		else if (ctx->v23->conn_state == V23_CONNECTED) {
			// TODO
		}

		sine_generate(ctx->sine_gen, sampv + i, 1, freq, ctx->srate);
		ctx->ns_counter += (ms_to_ns(1000) / ctx->srate);
	}

	return 0;
}

static int decode(struct aufilt_dec_st *st, int16_t *sampv, size_t *sampc)
{
	struct v23modem_dec *ctx = (struct v23modem_dec*)st;
	size_t i, samples = *sampc;

	for (i = 0; i < samples; i++) {
		goertzel_update(&ctx->gtz_hi, *(sampv + i));
		goertzel_update(&ctx->gtz_lo, *(sampv + i));

		if (ctx->v23->conn_state == V23_HOOK) {
			double hi_level = goertzel_result(&ctx->gtz_hi);
			if(hi_level > CARRIER_THRESHOLD) {
				if (ctx->carrier_ns <= 0) {
					info("v23 decoder: hook: carrier detected!\n");
				}
				ctx->carrier_ns += (ms_to_ns(1000) / ctx->srate); 
			} else {
				if (ctx->carrier_ns > 0) {
					info("v23 decode: hook: carrier lost...\n");
				}
				ctx->carrier_ns = 0;
			}
		}
	}

	if (ctx->v23->conn_state == V23_HOOK) {
		if (ctx->carrier_ns >= ms_to_ns(100)) {
			info("v23 decoder: hook: carrier is stable, switching to connected state\n");
			ctx->v23->conn_state = V23_CONNECTED;
		}
	}
	else if (ctx->v23->conn_state == V23_CONNECTED) {
		// TODO
	}

	return 0;
}

static struct aufilt v23modem = {
	LE_INIT, "v23modem", encode_update, encode, decode_update, decode
};

static int module_init(void)
{
	dsp_init();
	aufilt_register(baresip_aufiltl(), &v23modem);
	return 0;
}

static int module_close(void)
{
	aufilt_unregister(&v23modem);
	return 0;
}

const struct mod_export DECL_EXPORTS(v23) = {
	"v23",
	"application",
	module_init,
	module_close
};

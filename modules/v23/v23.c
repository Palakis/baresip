/**
 * @file v23.c V.23 modem module
 */
#include <re.h>
#include <rem_autone.h>
#include <baresip.h>

#include "v23.h"

/**
 * V23 modem implementation as a filter
 * NOTE: very experimental
 */

//static struct ausrc* v23in;
//static struct auplay* v23out;

static void set_sine(struct v23modem_enc* st, uint32_t freq, int amplitude) {
	autone_sine(st->tone_buf, st->srate, freq, amplitude, 0, 0);
	mbuf_set_pos(st->tone_buf, 0);
}

static void enc_destructor(void *arg)
{
	struct v23modem_enc *st = arg;
	mem_deref(st->tone_buf);
}

static void dec_destructor(void *arg)
{
	struct v23modem_dec *st = arg;
	(void)st;
}

static int encode_update(struct aufilt_enc_st **stp, void **ctx,
						 const struct aufilt *af, struct aufilt_prm *prm)
{
	struct v23modem_enc *st;
	(void)ctx;

	if (!stp || !af || !prm)
		return EINVAL;

	st = mem_zalloc(sizeof(*st), enc_destructor);
	if (!st)
		return ENOMEM;

	st->srate = prm->srate;
	st->tone_buf = mbuf_alloc(st->srate * 2);
	set_sine(st, 600, 80);

	*stp = (struct aufilt_enc_st*)st;

	return 0;
}

static int decode_update(struct aufilt_dec_st **stp, void **ctx,
						 const struct aufilt *af, struct aufilt_prm *prm)
{
	struct v23modem_dec *st;
	(void)ctx;

	if (!stp || !af || !prm)
		return EINVAL;

	st = mem_zalloc(sizeof(*st), dec_destructor);
	if (!st)
		return ENOMEM;

	st->srate = prm->srate;

	*stp = (struct aufilt_dec_st*)st;

	return 0;
}

static int encode(struct aufilt_enc_st *st, int16_t *sampv, size_t *sampc)
{
	struct v23modem_enc *ctx = (struct v23modem_enc*)st;
	size_t i, samples = *sampc;

	for(i = 0; i < samples; i++) {
		*(sampv + i) = mbuf_read_u16(ctx->tone_buf);
		if(mbuf_get_left(ctx->tone_buf) <= 0) {
			mbuf_set_pos(ctx->tone_buf, 0);
		}
	}

	return 0;
}

static int decode(struct aufilt_dec_st *st, int16_t *sampv, size_t *sampc)
{
	struct v23modem_dec *ctx = (struct v23modem_dec*)st;
	(void)ctx;

	return 0;
}

static struct aufilt v23modem = {
	LE_INIT, "v23modem", encode_update, encode, decode_update, decode
};

static int module_init(void)
{
	int err = 0;

	//err = ausrc_register(&v23in, baresip_ausrcl(), "v23", v23in_src_alloc);
	//err |= auplay_register(&v23out, baresip_auplayl(), "v23", v23out_play_alloc);

	aufilt_register(baresip_aufiltl(), &v23modem);

	return err;
}

static int module_close(void)
{
	//ausrc = mem_deref(ausrc);
	//auplay = mem_deref(auplay);

	aufilt_unregister(&v23modem);

	return 0;
}

const struct mod_export DECL_EXPORTS(v23) = {
	"v23",
	"application",
	module_init,
	module_close
};

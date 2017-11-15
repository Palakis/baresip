/**
 * @file v23.c V.23 modem module
 */
#include <re.h>
#include <rem.h>
#include <baresip.h>

/**
 * V23 modem implementation as a module
 * NOTE: very experimental
 */

struct v23in_src_t {
	const struct ausrc *as;
	struct device *dev;
	struct ausrc_prm prm;
	ausrc_read_h *rh;
	void *arg;
};

struct v23out_play_t {
	const struct auplay *ap;
	struct device *dev;
	struct auplay_prm prm;
	auplay_write_h *wh;
	void *arg;
};

static void v23in_destructor(void *arg)
{
	struct v23in_src_t *in_ctx = arg;
	(void)in_ctx;
}

int v23in_src_alloc(struct ausrc_st **stp, const struct ausrc *as,
		      struct media_ctx **ctx,
		      struct ausrc_prm *prm, const char *device,
		      ausrc_read_h *rh, ausrc_error_h *errh, void *arg)
{
	struct v23in_src_t *in_ctx;
	(void)ctx;
	(void)errh;

	if (!stp || !as || !prm)
		return EINVAL;

	in_ctx = mem_zalloc(sizeof(*in_ctx), v23in_destructor);
	if (!in_ctx)
		return ENOMEM;

	in_ctx->as 	= as;
	in_ctx->prm 	= *prm;
	in_ctx->rh	= rh;
	in_ctx->arg	= arg;

	return 0;
}

static void v23out_destructor(void *arg)
{
	struct v23out_play_t *out_ctx = arg;
	(void)out_ctx;
}

int v23out_play_alloc(struct auplay_st **stp, const struct auplay *ap,
		      struct auplay_prm *prm, const char *device,
		      auplay_write_h *wh, void *arg)
{
	struct v23out_play_t *out_ctx;

	if (!stp || !ap || !prm)
		return EINVAL;

	out_ctx = mem_zalloc(sizeof(*out_ctx), v23out_destructor);
	if (!out_ctx)
		return ENOMEM;

	out_ctx->ap = ap;
	out_ctx->prm = *prm;
	out_ctx->wh = wh;
	out_ctx->arg = arg;

	return 0;
}

static struct ausrc* v23in;
static struct auplay* v23out;

static int module_init(void)
{
	int err = 0;

	err = ausrc_register(&v23in, baresip_ausrcl(), "v23", v23in_src_alloc);
	err |= auplay_register(&v23out, baresip_auplayl(), "v23", v23out_play_alloc);

	return err;
}

static int module_close(void)
{
	return 0;
}

const struct mod_export DECL_EXPORTS(v23) = {
	"v23",
	"audio",
	module_init,
	module_close
};

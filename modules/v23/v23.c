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

struct v23out_ctx {

};

struct v23in_ctx {

};

int v23out_src_alloc(struct ausrc_st **stp, const struct ausrc *as,
		      struct media_ctx **ctx,
		      struct ausrc_prm *prm, const char *device,
		      ausrc_read_h *rh, ausrc_error_h *errh, void *arg) {
	return 0;
}

int v23in_play_alloc(struct auplay_st **stp, const struct auplay *ap,
		      struct auplay_prm *prm, const char *device,
		      auplay_write_h *wh, void *arg) {
	return 0;
}

static struct ausrc* v23out;
static struct auplay* v23in;

static int module_init(void)
{
	int err = 0;

	err = ausrc_register(&v23out, baresip_ausrcl(), "v23", v23out_src_alloc);
	err |= auplay_register(&v23in, baresip_auplayl(), "v23", v23in_play_alloc);

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

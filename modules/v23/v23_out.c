#include <re.h>
#include <rem.h>
#include <pthread.h>
#include <baresip.h>

#include "v23.h"

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
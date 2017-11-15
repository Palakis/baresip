#include <re.h>
#include <rem.h>
#include <pthread.h>
#include <baresip.h>

#include "v23.h"

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
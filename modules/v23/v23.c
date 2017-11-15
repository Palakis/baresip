/**
 * @file v23.c V.23 modem module
 */
#include <re.h>
#include <baresip.h>
#include "v23.h"

/**
 * V23 modem implementation as a module
 * NOTE: very experimental
 */

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

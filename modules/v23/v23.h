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

int v23in_src_alloc(struct ausrc_st **stp, const struct ausrc *as,
                    struct media_ctx **ctx,
                    struct ausrc_prm *prm, const char *device,
                    ausrc_read_h *rh, ausrc_error_h *errh, void *arg);

int v23out_play_alloc(struct auplay_st **stp, const struct auplay *ap,
                      struct auplay_prm *prm, const char *device,
                      auplay_write_h *wh, void *arg);
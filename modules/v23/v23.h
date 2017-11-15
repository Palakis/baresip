/*
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
*/

enum v23_enc_state {
    HOOKUP_FREQ_2100,
    HOOKUP_SILENCE,
    HOOKUP_FREQ_1300
};

struct v23modem_enc {
    struct aufilt_enc_st af; // base class
    uint32_t srate;
    struct mbuf *tone_buf;
    uint64_t ns_counter;
    enum v23_enc_state state;
};

struct v23modem_dec {
    struct aufilt_dec_st af; // base class
    uint32_t srate;
};

/*
int v23in_src_alloc(struct ausrc_st **stp, const struct ausrc *as,
                    struct media_ctx **ctx,
                    struct ausrc_prm *prm, const char *device,
                    ausrc_read_h *rh, ausrc_error_h *errh, void *arg);

int v23out_play_alloc(struct auplay_st **stp, const struct auplay *ap,
                      struct auplay_prm *prm, const char *device,
                      auplay_write_h *wh, void *arg);
*/
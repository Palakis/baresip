enum v23_hook_sequence_state {
    HOOK_NONE,
    HOOK_FREQ_2100,
    HOOK_SILENCE,
    HOOK_FREQ_1300
};

enum v23_connection_state {
    V23_HOOK,
    V23_CONNECTED,
    V23_DISCONNECTED
};

struct v23_t {
    enum v23_connection_state conn_state;
};

struct v23modem_enc {
    struct aufilt_enc_st af; // base class
    struct v23_t *v23;
    uint32_t srate;
    struct mbuf *tone_buf;
    uint64_t ns_counter;
    enum v23_hook_sequence_state hook_state;
};

struct v23modem_dec {
    struct aufilt_dec_st af; // base class
    struct v23_t *v23;
    uint32_t srate;
};

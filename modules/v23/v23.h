#ifndef V23_H
#define V23_H

#include "sine.h"

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
    uint64_t ns_counter;
    enum v23_hook_sequence_state hook_state;
    struct sine_t *sine_gen;
};

struct v23modem_dec {
    struct aufilt_dec_st af; // base class
    struct v23_t *v23;
    uint32_t srate;
};

#endif // V23_H

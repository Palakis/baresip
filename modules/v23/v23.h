#ifndef V23_H
#define V23_H

#include <rem_goertzel.h>

#include "mod.h"

enum v23_hook_sequence_state {
    HOOK_NONE,
    HOOK_FREQ_2100,
    HOOK_SILENCE,
    HOOK_FREQ_1300
};

enum v23_connection_state {
    V23_HOOK,
    V23_CONNECTED,
    V23_HANGUP
};

struct v23_t {
    enum v23_connection_state conn_state;
};

struct v23modem_enc {
    struct aufilt_enc_st af; // base class
    struct v23_t *v23;
    uint32_t srate;
    enum v23_hook_sequence_state hook_state;
    uint16_t baud_rate;
    uint64_t ns_counter;
    struct sine_t *sine_gen;
};

struct v23modem_dec {
    struct aufilt_dec_st af; // base class
    struct v23_t *v23;
    uint32_t srate;
    uint16_t baud_rate;
    uint64_t carrier_ns;
    struct goertzel gtz_hi;
    struct goertzel gtz_lo;
};

#endif // V23_H

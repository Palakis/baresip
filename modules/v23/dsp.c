#include "dsp.h"

int16_t cos_tab[COS_TABLE_SIZE];

void dsp_init(void)
{
    int i;
    for (i = 0; i < COS_TABLE_SIZE; i++) {
        cos_tab[i] = (int16_t)(cos(2 * M_PI * i / COS_TABLE_SIZE) * COS_BASE);
    }
}
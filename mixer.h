/**
 *
 **/

#ifndef __MIXER_H__
#define __MIXER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#include "constants.h"

void mixer_init(void);
void send_line_phono_switch(uint8_t value);
void send_input_gain(uint32_t ch1_val, uint32_t ch2_val);
void send_hi_shelf_ch1(uint32_t value);
void send_hi_shelf_ch2(uint32_t value);
void send_mid_peaking_ch1(uint32_t value);
void send_mid_peaking_ch2(uint32_t value);
void send_low_shelf_ch1(uint32_t value);
void send_low_shelf_ch2(uint32_t value);
void send_xfader(uint32_t *xf_adc);

#ifdef __cplusplus
}
#endif

#endif
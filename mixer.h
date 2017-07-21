/**
 *
 **/

#ifndef __MIXER_H__
#define __MIXER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
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
void send_ifader(uint32_t ch1_val, uint32_t ch2_val);
void send_xfader(uint32_t *xf_adc, double xf_curve, bool xf_rev);
void send_pitch_shifter(uint32_t xf_adc);
void send_lpf(uint32_t value);
void send_select_fx(uint8_t type);
void send_master_booth_gain(uint32_t master_val, uint32_t booth_val);

#ifdef __cplusplus
}
#endif

#endif
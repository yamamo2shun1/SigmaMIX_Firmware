/*
 * Copylight (C) 2017, Shunichi Yamamoto, tkrworks.net
 *
 * This file is part of SigmaMIX_Firmware.
 *
 * SigmaMIX_Firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option ) any later version.
 *
 * SigmaMIX_Firmware is distributed in the hope that it will be useful,
 * but WITHIOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COSC. if not, see <http:/www.gnu.org/licenses/>.
 *
 * mixer.h
 */

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
void send_ifader(uint32_t ch1_val, uint32_t ch2_val, double if_curve, bool if_rev);
void send_xfader(uint32_t *xf_adc, double xf_curve, bool xf_rev);
void send_pitch_shifter(uint32_t xf_adc, uint8_t type);
void send_lpf(uint32_t value);
void send_select_fx(uint8_t type);
void send_master_booth_gain(uint32_t master_val, uint32_t booth_val);
void send_monitor_mix_gain(bool ch_sel, uint32_t mix_val, uint32_t monitor_val);

#ifdef __cplusplus
}
#endif

#endif

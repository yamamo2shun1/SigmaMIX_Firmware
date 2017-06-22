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
void send_xfader(uint32_t *xf_adc);
void send_high_shelf_ch1(uint32_t value);
void send_mid_peaking_ch1(uint32_t value);
void send_low_shelf_ch1(uint32_t value);

#ifdef __cplusplus
}
#endif

#endif
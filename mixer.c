/*
 *
 */

#include <stdint.h>

#include "mixer.h"

//#include "SigmaStudioFW.h"
#include "SigmaMIX_IC_1.h"
#include "SigmaMIX_IC_1_PARAM.h"
#include "SigmaMIX_IC_2.h"
#include "SigmaMIX_IC_2_PARAM.h"

ADI_REG_U8 xf1_val[4] = {0x00};
ADI_REG_U8 xf2_val[4] = {0x00};
ADI_REG_U8 xf_step[4] = {0x00, 0x01, 0x00, 0x00};

static uint32_t xf0_adc[2];

void mixer_init(void)
{
  default_download_IC_1();
  default_download_IC_2();
}

void send_line_phono_switch(uint8_t value)
{
  ADI_REG_U8 ch1_line_sw[4] = {0x00, 0x00, 0x00, 0x00};
  ADI_REG_U8 ch1_phono_sw[4] = {0x00, 0x00, 0x00, 0x00};
  ADI_REG_U8 ch2_line_sw[4] = {0x00, 0x00, 0x00, 0x00};
  ADI_REG_U8 ch2_phono_sw[4] = {0x00, 0x00, 0x00, 0x00};

  if ((value >> 4) & 0x01)
  {
    ch1_line_sw[1] = 0x00;
    ch1_phono_sw[1] = 0x80;
  }
  else {
    ch1_line_sw[1] = 0x80;
    ch1_phono_sw[1] = 0x00;
  }

  if (value & 0x01)
  {
    ch2_line_sw[1] = 0x00;
    ch2_phono_sw[1] = 0x80;
  }
  else {
    ch2_line_sw[1] = 0x80;
    ch2_phono_sw[1] = 0x00;
  }

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_0, 4, ch1_line_sw);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_0, MOD_PHLNSW1_ALG0_STEREODEMUX1940NS10_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_1, 4, ch1_phono_sw);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_1, MOD_PHLNSW1_ALG0_STEREODEMUX1940NS11_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_1);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_0, 4, ch2_line_sw);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_0, MOD_PHLNSW2_ALG0_STEREODEMUX1940NS20_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_1, 4, ch2_phono_sw);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_1, MOD_PHLNSW2_ALG0_STEREODEMUX1940NS21_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_1);
}

void send_input_gain(uint32_t ch1_val, uint32_t ch2_val)
{
  double ch1_db = ((ch1_val / 127.0) - 1.0) * 15.0;
  double ch2_db = ((ch2_val / 127.0) - 1.0) * 15.0;

  double ch1_rate = pow(10.0, ch1_db / 20);
  double ch2_rate = pow(10.0, ch2_db / 20);

  uint8_t ch1_gain[4] = {0x00};
  ch1_gain[0] = ((uint32_t)(ch1_rate * pow(2, 23)) >> 24) & 0x000000FF;
  ch1_gain[1] = ((uint32_t)(ch1_rate * pow(2, 23)) >> 16) & 0x000000FF;
  ch1_gain[2] = ((uint32_t)(ch1_rate * pow(2, 23)) >> 8)  & 0x000000FF;
  ch1_gain[3] =  (uint32_t)(ch1_rate * pow(2, 23))        & 0x000000FF;

  uint8_t ch2_gain[4] = {0x00};
  ch2_gain[0] = ((uint32_t)(ch2_rate * pow(2, 23)) >> 24) & 0x000000FF;
  ch2_gain[1] = ((uint32_t)(ch2_rate * pow(2, 23)) >> 16) & 0x000000FF;
  ch2_gain[2] = ((uint32_t)(ch2_rate * pow(2, 23)) >> 8)  & 0x000000FF;
  ch2_gain[3] =  (uint32_t)(ch2_rate * pow(2, 23))        & 0x000000FF;

  ADI_REG_U8 if_step[4] = {0x00, 0x00, 0x80, 0x00};

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_0, 4, ch1_gain);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_0, MOD_GAIN1_ALG0_TARGET_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_1, 4, if_step);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_1, MOD_GAIN1_ALG0_STEP_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_2, 4, ch2_gain);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_2, MOD_GAIN2_ALG0_TARGET_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_3, 4, if_step);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_3, MOD_GAIN2_ALG0_STEP_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_1);
}

void send_hi_shelf_ch1(uint32_t value)
{
  ADI_REG_U8 hi_shelf_b0[4];
  ADI_REG_U8 hi_shelf_b1[4];
  ADI_REG_U8 hi_shelf_b2[4];
  ADI_REG_U8 hi_shelf_a1[4];
  ADI_REG_U8 hi_shelf_a2[4];

  double hi_shelf_gain = (value / 127.0) - 1.0;
  if (hi_shelf_gain > 0.0)
  {
    hi_shelf_gain *= 15.0;
  }
  else if(hi_shelf_gain < 0.0)
  {
    hi_shelf_gain *= 42.0;
  }

  double w0 = 2.0 * M_PI * 6500.0 / SAMPLE_RATE;
  double A = pow(10.0, hi_shelf_gain / 20.0);
  double S = 1.0;
  double a = sin(w0) / 2.0 * sqrt((A + (1.0 / A)) * ((1.0 / S) - 1.0) + 2.0);
  double b = 2.0 * sqrt(A) * a;

  double b0 = A * ((A + 1.0) + (A - 1.0) * cos(w0) + b);
  double b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cos(w0));
  double b2 = A * ((A + 1.0) + (A - 1.0) * cos(w0) - b);
  double a0 =      (A + 1.0) - (A - 1.0) * cos(w0) + b;
  double a1 =  2.0 *     ((A - 1.0) - (A + 1.0) * cos(w0));
  double a2 =      (A + 1.0) - (A - 1.0) * cos(w0) - b;

  b0 /= a0;
  b1 /= a0;
  b2 /= a0;
  a1 /= -a0;
  a2 /= -a0;

  hi_shelf_b0[0] = ((int32_t)(b0 * pow(2, 23)) >> 24) & 0x000000FF;
  hi_shelf_b0[1] = ((int32_t)(b0 * pow(2, 23)) >> 16) & 0x000000FF;
  hi_shelf_b0[2] = ((int32_t)(b0 * pow(2, 23)) >> 8)  & 0x000000FF;
  hi_shelf_b0[3] = ((int32_t)(b0 * pow(2, 23)))       & 0x000000FF;
  hi_shelf_b1[0] = ((int32_t)(b1 * pow(2, 23)) >> 24) & 0x000000FF;
  hi_shelf_b1[1] = ((int32_t)(b1 * pow(2, 23)) >> 16) & 0x000000FF;
  hi_shelf_b1[2] = ((int32_t)(b1 * pow(2, 23)) >> 8)  & 0x000000FF;
  hi_shelf_b1[3] = ((int32_t)(b1 * pow(2, 23)))       & 0x000000FF;
  hi_shelf_b2[0] = ((int32_t)(b2 * pow(2, 23)) >> 24) & 0x000000FF;
  hi_shelf_b2[1] = ((int32_t)(b2 * pow(2, 23)) >> 16) & 0x000000FF;
  hi_shelf_b2[2] = ((int32_t)(b2 * pow(2, 23)) >> 8)  & 0x000000FF;
  hi_shelf_b2[3] = ((int32_t)(b2 * pow(2, 23)))       & 0x000000FF;
  hi_shelf_a1[0] = ((int32_t)(a1 * pow(2, 23)) >> 24) & 0x000000FF;
  hi_shelf_a1[1] = ((int32_t)(a1 * pow(2, 23)) >> 16) & 0x000000FF;
  hi_shelf_a1[2] = ((int32_t)(a1 * pow(2, 23)) >> 8)  & 0x000000FF;
  hi_shelf_a1[3] = ((int32_t)(a1 * pow(2, 23)))       & 0x000000FF;
  hi_shelf_a2[0] = ((int32_t)(a2 * pow(2, 23)) >> 24) & 0x000000FF;
  hi_shelf_a2[1] = ((int32_t)(a2 * pow(2, 23)) >> 16) & 0x000000FF;
  hi_shelf_a2[2] = ((int32_t)(a2 * pow(2, 23)) >> 8)  & 0x000000FF;
  hi_shelf_a2[3] = ((int32_t)(a2 * pow(2, 23)))       & 0x000000FF;

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_0, 4, hi_shelf_b0);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_0, MOD_EQ1_ALG0_STAGE0_B0_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_1, 4, hi_shelf_b1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_1, MOD_EQ1_ALG0_STAGE0_B1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_2, 4, hi_shelf_b2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_2, MOD_EQ1_ALG0_STAGE0_B2_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_3, 4, hi_shelf_a1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_3, MOD_EQ1_ALG0_STAGE0_A1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_4, 4, hi_shelf_a2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_4, MOD_EQ1_ALG0_STAGE0_A2_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_1);
}

void send_hi_shelf_ch2(uint32_t value)
{
  ADI_REG_U8 hi_shelf_b0[4];
  ADI_REG_U8 hi_shelf_b1[4];
  ADI_REG_U8 hi_shelf_b2[4];
  ADI_REG_U8 hi_shelf_a1[4];
  ADI_REG_U8 hi_shelf_a2[4];

  double hi_shelf_gain = (value / 127.0) - 1.0;
  if (hi_shelf_gain > 0.0)
  {
    hi_shelf_gain *= 15.0;
  }
  else if(hi_shelf_gain < 0.0)
  {
    hi_shelf_gain *= 42.0;
  }

  double w0 = 2.0 * M_PI * 6500.0 / SAMPLE_RATE;
  double A = pow(10.0, hi_shelf_gain / 20.0);
  double S = 1.0;
  double a = sin(w0) / 2.0 * sqrt((A + (1.0 / A)) * ((1.0 / S) - 1.0) + 2.0);
  double b = 2.0 * sqrt(A) * a;

  double b0 = A * ((A + 1.0) + (A - 1.0) * cos(w0) + b);
  double b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cos(w0));
  double b2 = A * ((A + 1.0) + (A - 1.0) * cos(w0) - b);
  double a0 =      (A + 1.0) - (A - 1.0) * cos(w0) + b;
  double a1 =  2.0 *     ((A - 1.0) - (A + 1.0) * cos(w0));
  double a2 =      (A + 1.0) - (A - 1.0) * cos(w0) - b;

  b0 /= a0;
  b1 /= a0;
  b2 /= a0;
  a1 /= -a0;
  a2 /= -a0;

  hi_shelf_b0[0] = ((int32_t)(b0 * pow(2, 23)) >> 24) & 0x000000FF;
  hi_shelf_b0[1] = ((int32_t)(b0 * pow(2, 23)) >> 16) & 0x000000FF;
  hi_shelf_b0[2] = ((int32_t)(b0 * pow(2, 23)) >> 8)  & 0x000000FF;
  hi_shelf_b0[3] = ((int32_t)(b0 * pow(2, 23)))       & 0x000000FF;
  hi_shelf_b1[0] = ((int32_t)(b1 * pow(2, 23)) >> 24) & 0x000000FF;
  hi_shelf_b1[1] = ((int32_t)(b1 * pow(2, 23)) >> 16) & 0x000000FF;
  hi_shelf_b1[2] = ((int32_t)(b1 * pow(2, 23)) >> 8)  & 0x000000FF;
  hi_shelf_b1[3] = ((int32_t)(b1 * pow(2, 23)))       & 0x000000FF;
  hi_shelf_b2[0] = ((int32_t)(b2 * pow(2, 23)) >> 24) & 0x000000FF;
  hi_shelf_b2[1] = ((int32_t)(b2 * pow(2, 23)) >> 16) & 0x000000FF;
  hi_shelf_b2[2] = ((int32_t)(b2 * pow(2, 23)) >> 8)  & 0x000000FF;
  hi_shelf_b2[3] = ((int32_t)(b2 * pow(2, 23)))       & 0x000000FF;
  hi_shelf_a1[0] = ((int32_t)(a1 * pow(2, 23)) >> 24) & 0x000000FF;
  hi_shelf_a1[1] = ((int32_t)(a1 * pow(2, 23)) >> 16) & 0x000000FF;
  hi_shelf_a1[2] = ((int32_t)(a1 * pow(2, 23)) >> 8)  & 0x000000FF;
  hi_shelf_a1[3] = ((int32_t)(a1 * pow(2, 23)))       & 0x000000FF;
  hi_shelf_a2[0] = ((int32_t)(a2 * pow(2, 23)) >> 24) & 0x000000FF;
  hi_shelf_a2[1] = ((int32_t)(a2 * pow(2, 23)) >> 16) & 0x000000FF;
  hi_shelf_a2[2] = ((int32_t)(a2 * pow(2, 23)) >> 8)  & 0x000000FF;
  hi_shelf_a2[3] = ((int32_t)(a2 * pow(2, 23)))       & 0x000000FF;

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_0, 4, hi_shelf_b0);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_0, MOD_EQ2_ALG0_STAGE0_B0_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_1, 4, hi_shelf_b1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_1, MOD_EQ2_ALG0_STAGE0_B1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_2, 4, hi_shelf_b2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_2, MOD_EQ2_ALG0_STAGE0_B2_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_3, 4, hi_shelf_a1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_3, MOD_EQ2_ALG0_STAGE0_A1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_4, 4, hi_shelf_a2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_4, MOD_EQ2_ALG0_STAGE0_A2_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_1);
}

void send_mid_peaking_ch1(uint32_t value)
{
  ADI_REG_U8 peaking_b0[4];
  ADI_REG_U8 peaking_b1[4];
  ADI_REG_U8 peaking_b2[4];
  ADI_REG_U8 peaking_a1[4];
  ADI_REG_U8 peaking_a2[4];

  double peaking_gain = (value / 127.0) - 1.0;
  if (peaking_gain > 0.0)
  {
    peaking_gain *= 15.0;
  }
  else if(peaking_gain < 0.0)
  {
    peaking_gain *= 42.0;
  }

  double A = pow(10.0, peaking_gain / 20.0);
  double Q = 1.41;
  double w0 = 2.0 * M_PI * 1200.0 / SAMPLE_RATE;
  //double a = sin(w0) / (2.0 * A * Q);
  double a = sin(w0) / (2.0 * Q);

  double a0 = 0.0;
  double a1 = 0.0;
  double a2 = 0.0;
  double b0 = 0.0;
  double b1 = 0.0;
  double b2 = 0.0;

  if (peaking_gain == 0.0)
  {
    b0 = 1.0;
    b1 = 0.0;
    b2 = 0.0;
    a1 = 0.0;
    a2 = 0.0;
  }
  else
  {
    a0 = 1.0 + a / A;
    a1 = -2.0 * cos(w0);
    a2 = 1.0 - a / A;
    b0 = 1.0 + a * A;
    b1 = -2.0 * cos(w0);
    b2 = 1.0 - a * A;

    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= -a0;
    a2 /= -a0;
  }

  peaking_b0[0] = ((int32_t)(b0 * pow(2, 23)) >> 24) & 0x000000FF;
  peaking_b0[1] = ((int32_t)(b0 * pow(2, 23)) >> 16) & 0x000000FF;
  peaking_b0[2] = ((int32_t)(b0 * pow(2, 23)) >> 8)  & 0x000000FF;
  peaking_b0[3] = ((int32_t)(b0 * pow(2, 23)))       & 0x000000FF;
  peaking_b1[0] = ((int32_t)(b1 * pow(2, 23)) >> 24) & 0x000000FF;
  peaking_b1[1] = ((int32_t)(b1 * pow(2, 23)) >> 16) & 0x000000FF;
  peaking_b1[2] = ((int32_t)(b1 * pow(2, 23)) >> 8)  & 0x000000FF;
  peaking_b1[3] = ((int32_t)(b1 * pow(2, 23)))       & 0x000000FF;
  peaking_b2[0] = ((int32_t)(b2 * pow(2, 23)) >> 24) & 0x000000FF;
  peaking_b2[1] = ((int32_t)(b2 * pow(2, 23)) >> 16) & 0x000000FF;
  peaking_b2[2] = ((int32_t)(b2 * pow(2, 23)) >> 8)  & 0x000000FF;
  peaking_b2[3] = ((int32_t)(b2 * pow(2, 23)))       & 0x000000FF;
  peaking_a1[0] = ((int32_t)(a1 * pow(2, 23)) >> 24) & 0x000000FF;
  peaking_a1[1] = ((int32_t)(a1 * pow(2, 23)) >> 16) & 0x000000FF;
  peaking_a1[2] = ((int32_t)(a1 * pow(2, 23)) >> 8)  & 0x000000FF;
  peaking_a1[3] = ((int32_t)(a1 * pow(2, 23)))       & 0x000000FF;
  peaking_a2[0] = ((int32_t)(a2 * pow(2, 23)) >> 24) & 0x000000FF;
  peaking_a2[1] = ((int32_t)(a2 * pow(2, 23)) >> 16) & 0x000000FF;
  peaking_a2[2] = ((int32_t)(a2 * pow(2, 23)) >> 8)  & 0x000000FF;
  peaking_a2[3] = ((int32_t)(a2 * pow(2, 23)))       & 0x000000FF;

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_0, 4, peaking_b0);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_0, MOD_EQ1_ALG0_STAGE1_B0_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_1, 4, peaking_b1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_1, MOD_EQ1_ALG0_STAGE1_B1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_2, 4, peaking_b2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_2, MOD_EQ1_ALG0_STAGE1_B2_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_3, 4, peaking_a1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_3, MOD_EQ1_ALG0_STAGE1_A1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_4, 4, peaking_a2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_4, MOD_EQ1_ALG0_STAGE1_A2_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_1);
}

void send_mid_peaking_ch2(uint32_t value)
{
  ADI_REG_U8 peaking_b0[4];
  ADI_REG_U8 peaking_b1[4];
  ADI_REG_U8 peaking_b2[4];
  ADI_REG_U8 peaking_a1[4];
  ADI_REG_U8 peaking_a2[4];

  double peaking_gain = (value / 127.0) - 1.0;
  if (peaking_gain > 0.0)
  {
    peaking_gain *= 15.0;
  }
  else if(peaking_gain < 0.0)
  {
    peaking_gain *= 42.0;
  }

  double A = pow(10.0, peaking_gain / 20.0);
  double Q = 1.41;
  double w0 = 2.0 * M_PI * 1200.0 / SAMPLE_RATE;
  //double a = sin(w0) / (2.0 * A * Q);
  double a = sin(w0) / (2.0 * Q);

  double a0 = 0.0;
  double a1 = 0.0;
  double a2 = 0.0;
  double b0 = 0.0;
  double b1 = 0.0;
  double b2 = 0.0;

  if (peaking_gain == 0.0)
  {
    b0 = 1.0;
    b1 = 0.0;
    b2 = 0.0;
    a1 = 0.0;
    a2 = 0.0;
  }
  else
  {
    a0 = 1.0 + a / A;
    a1 = -2.0 * cos(w0);
    a2 = 1.0 - a / A;
    b0 = 1.0 + a * A;
    b1 = -2.0 * cos(w0);
    b2 = 1.0 - a * A;

    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= -a0;
    a2 /= -a0;
  }

  peaking_b0[0] = ((int32_t)(b0 * pow(2, 23)) >> 24) & 0x000000FF;
  peaking_b0[1] = ((int32_t)(b0 * pow(2, 23)) >> 16) & 0x000000FF;
  peaking_b0[2] = ((int32_t)(b0 * pow(2, 23)) >> 8)  & 0x000000FF;
  peaking_b0[3] = ((int32_t)(b0 * pow(2, 23)))       & 0x000000FF;
  peaking_b1[0] = ((int32_t)(b1 * pow(2, 23)) >> 24) & 0x000000FF;
  peaking_b1[1] = ((int32_t)(b1 * pow(2, 23)) >> 16) & 0x000000FF;
  peaking_b1[2] = ((int32_t)(b1 * pow(2, 23)) >> 8)  & 0x000000FF;
  peaking_b1[3] = ((int32_t)(b1 * pow(2, 23)))       & 0x000000FF;
  peaking_b2[0] = ((int32_t)(b2 * pow(2, 23)) >> 24) & 0x000000FF;
  peaking_b2[1] = ((int32_t)(b2 * pow(2, 23)) >> 16) & 0x000000FF;
  peaking_b2[2] = ((int32_t)(b2 * pow(2, 23)) >> 8)  & 0x000000FF;
  peaking_b2[3] = ((int32_t)(b2 * pow(2, 23)))       & 0x000000FF;
  peaking_a1[0] = ((int32_t)(a1 * pow(2, 23)) >> 24) & 0x000000FF;
  peaking_a1[1] = ((int32_t)(a1 * pow(2, 23)) >> 16) & 0x000000FF;
  peaking_a1[2] = ((int32_t)(a1 * pow(2, 23)) >> 8)  & 0x000000FF;
  peaking_a1[3] = ((int32_t)(a1 * pow(2, 23)))       & 0x000000FF;
  peaking_a2[0] = ((int32_t)(a2 * pow(2, 23)) >> 24) & 0x000000FF;
  peaking_a2[1] = ((int32_t)(a2 * pow(2, 23)) >> 16) & 0x000000FF;
  peaking_a2[2] = ((int32_t)(a2 * pow(2, 23)) >> 8)  & 0x000000FF;
  peaking_a2[3] = ((int32_t)(a2 * pow(2, 23)))       & 0x000000FF;

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_0, 4, peaking_b0);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_0, MOD_EQ2_ALG0_STAGE1_B0_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_1, 4, peaking_b1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_1, MOD_EQ2_ALG0_STAGE1_B1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_2, 4, peaking_b2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_2, MOD_EQ2_ALG0_STAGE1_B2_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_3, 4, peaking_a1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_3, MOD_EQ2_ALG0_STAGE1_A1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_4, 4, peaking_a2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_4, MOD_EQ2_ALG0_STAGE1_A2_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_1);
}

void send_low_shelf_ch1(uint32_t value)
{
  ADI_REG_U8 low_shelf_b0[4];
  ADI_REG_U8 low_shelf_b1[4];
  ADI_REG_U8 low_shelf_b2[4];
  ADI_REG_U8 low_shelf_a1[4];
  ADI_REG_U8 low_shelf_a2[4];

  double low_shelf_gain = (value / 127.0) - 1.0;
  if (low_shelf_gain > 0.0)
  {
    low_shelf_gain *= 15.0;
  }
  else if(low_shelf_gain < 0.0)
  {
    low_shelf_gain *= 42.0;
  }

  double w0 = 2.0 * M_PI * 200.0 / SAMPLE_RATE;
  double A = pow(10.0, low_shelf_gain / 20.0);
  double S = 1.0;
  double a = sin(w0) / 2.0 * sqrt((A + (1.0 / A)) * ((1.0 / S) - 1.0) + 2.0);
  double b = 2.0 * sqrt(A) * a;

  double b0 = A * ((A + 1.0) - (A - 1.0) * cos(w0) + b);
  double b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * cos(w0));
  double b2 = A * ((A + 1.0) - (A - 1.0) * cos(w0) - b);
  double a0 = (A + 1.0) + (A - 1.0) * cos(w0) + b;
  double a1 = -2.0 * ((A - 1.0) + (A + 1.0) * cos(w0));
  double a2 = (A + 1.0) + (A - 1.0) * cos(w0) - b;

  b0 /= a0;
  b1 /= a0;
  b2 /= a0;
  a1 /= -a0;
  a2 /= -a0;

  low_shelf_b0[0] = ((int32_t)(b0 * pow(2, 23)) >> 24) & 0x000000FF;
  low_shelf_b0[1] = ((int32_t)(b0 * pow(2, 23)) >> 16) & 0x000000FF;
  low_shelf_b0[2] = ((int32_t)(b0 * pow(2, 23)) >> 8)  & 0x000000FF;
  low_shelf_b0[3] = ((int32_t)(b0 * pow(2, 23)))       & 0x000000FF;
  low_shelf_b1[0] = ((int32_t)(b1 * pow(2, 23)) >> 24) & 0x000000FF;
  low_shelf_b1[1] = ((int32_t)(b1 * pow(2, 23)) >> 16) & 0x000000FF;
  low_shelf_b1[2] = ((int32_t)(b1 * pow(2, 23)) >> 8)  & 0x000000FF;
  low_shelf_b1[3] = ((int32_t)(b1 * pow(2, 23)))       & 0x000000FF;
  low_shelf_b2[0] = ((int32_t)(b2 * pow(2, 23)) >> 24) & 0x000000FF;
  low_shelf_b2[1] = ((int32_t)(b2 * pow(2, 23)) >> 16) & 0x000000FF;
  low_shelf_b2[2] = ((int32_t)(b2 * pow(2, 23)) >> 8)  & 0x000000FF;
  low_shelf_b2[3] = ((int32_t)(b2 * pow(2, 23)))       & 0x000000FF;
  low_shelf_a1[0] = ((int32_t)(a1 * pow(2, 23)) >> 24) & 0x000000FF;
  low_shelf_a1[1] = ((int32_t)(a1 * pow(2, 23)) >> 16) & 0x000000FF;
  low_shelf_a1[2] = ((int32_t)(a1 * pow(2, 23)) >> 8)  & 0x000000FF;
  low_shelf_a1[3] = ((int32_t)(a1 * pow(2, 23)))       & 0x000000FF;
  low_shelf_a2[0] = ((int32_t)(a2 * pow(2, 23)) >> 24) & 0x000000FF;
  low_shelf_a2[1] = ((int32_t)(a2 * pow(2, 23)) >> 16) & 0x000000FF;
  low_shelf_a2[2] = ((int32_t)(a2 * pow(2, 23)) >> 8)  & 0x000000FF;
  low_shelf_a2[3] = ((int32_t)(a2 * pow(2, 23)))       & 0x000000FF;

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_0, 4, low_shelf_b0);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_0, MOD_EQ1_ALG0_STAGE2_B0_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_1, 4, low_shelf_b1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_1, MOD_EQ1_ALG0_STAGE2_B1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_2, 4, low_shelf_b2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_2, MOD_EQ1_ALG0_STAGE2_B2_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_3, 4, low_shelf_a1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_3, MOD_EQ1_ALG0_STAGE2_A1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_4, 4, low_shelf_a2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_4, MOD_EQ1_ALG0_STAGE2_A2_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_1);
}

void send_low_shelf_ch2(uint32_t value)
{
  ADI_REG_U8 low_shelf_b0[4];
  ADI_REG_U8 low_shelf_b1[4];
  ADI_REG_U8 low_shelf_b2[4];
  ADI_REG_U8 low_shelf_a1[4];
  ADI_REG_U8 low_shelf_a2[4];

  double low_shelf_gain = (value / 127.0) - 1.0;
  if (low_shelf_gain > 0.0)
  {
    low_shelf_gain *= 15.0;
  }
  else if(low_shelf_gain < 0.0)
  {
    low_shelf_gain *= 42.0;
  }

  double w0 = 2.0 * M_PI * 200.0 / SAMPLE_RATE;
  double A = pow(10.0, low_shelf_gain / 20.0);
  double S = 1.0;
  double a = sin(w0) / 2.0 * sqrt((A + (1.0 / A)) * ((1.0 / S) - 1.0) + 2.0);
  double b = 2.0 * sqrt(A) * a;

  double b0 = A * ((A + 1.0) - (A - 1.0) * cos(w0) + b);
  double b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * cos(w0));
  double b2 = A * ((A + 1.0) - (A - 1.0) * cos(w0) - b);
  double a0 = (A + 1.0) + (A - 1.0) * cos(w0) + b;
  double a1 = -2.0 * ((A - 1.0) + (A + 1.0) * cos(w0));
  double a2 = (A + 1.0) + (A - 1.0) * cos(w0) - b;

  b0 /= a0;
  b1 /= a0;
  b2 /= a0;
  a1 /= -a0;
  a2 /= -a0;

  low_shelf_b0[0] = ((int32_t)(b0 * pow(2, 23)) >> 24) & 0x000000FF;
  low_shelf_b0[1] = ((int32_t)(b0 * pow(2, 23)) >> 16) & 0x000000FF;
  low_shelf_b0[2] = ((int32_t)(b0 * pow(2, 23)) >> 8)  & 0x000000FF;
  low_shelf_b0[3] = ((int32_t)(b0 * pow(2, 23)))       & 0x000000FF;
  low_shelf_b1[0] = ((int32_t)(b1 * pow(2, 23)) >> 24) & 0x000000FF;
  low_shelf_b1[1] = ((int32_t)(b1 * pow(2, 23)) >> 16) & 0x000000FF;
  low_shelf_b1[2] = ((int32_t)(b1 * pow(2, 23)) >> 8)  & 0x000000FF;
  low_shelf_b1[3] = ((int32_t)(b1 * pow(2, 23)))       & 0x000000FF;
  low_shelf_b2[0] = ((int32_t)(b2 * pow(2, 23)) >> 24) & 0x000000FF;
  low_shelf_b2[1] = ((int32_t)(b2 * pow(2, 23)) >> 16) & 0x000000FF;
  low_shelf_b2[2] = ((int32_t)(b2 * pow(2, 23)) >> 8)  & 0x000000FF;
  low_shelf_b2[3] = ((int32_t)(b2 * pow(2, 23)))       & 0x000000FF;
  low_shelf_a1[0] = ((int32_t)(a1 * pow(2, 23)) >> 24) & 0x000000FF;
  low_shelf_a1[1] = ((int32_t)(a1 * pow(2, 23)) >> 16) & 0x000000FF;
  low_shelf_a1[2] = ((int32_t)(a1 * pow(2, 23)) >> 8)  & 0x000000FF;
  low_shelf_a1[3] = ((int32_t)(a1 * pow(2, 23)))       & 0x000000FF;
  low_shelf_a2[0] = ((int32_t)(a2 * pow(2, 23)) >> 24) & 0x000000FF;
  low_shelf_a2[1] = ((int32_t)(a2 * pow(2, 23)) >> 16) & 0x000000FF;
  low_shelf_a2[2] = ((int32_t)(a2 * pow(2, 23)) >> 8)  & 0x000000FF;
  low_shelf_a2[3] = ((int32_t)(a2 * pow(2, 23)))       & 0x000000FF;

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_0, 4, low_shelf_b0);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_0, MOD_EQ2_ALG0_STAGE2_B0_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_1, 4, low_shelf_b1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_1, MOD_EQ2_ALG0_STAGE2_B1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_2, 4, low_shelf_b2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_2, MOD_EQ2_ALG0_STAGE2_B2_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_3, 4, low_shelf_a1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_3, MOD_EQ2_ALG0_STAGE2_A1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_4, 4, low_shelf_a2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_4, MOD_EQ2_ALG0_STAGE2_A2_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_1);
}

void send_ifader(uint32_t ch1_val, uint32_t ch2_val)
{
  double ch1_db = ((ch1_val / 255.0) - 1.0) * 80.0;
  double ch2_db = ((ch2_val / 255.0) - 1.0) * 80.0;

  double ch1_rate = pow(10.0, ch1_db / 20);
  double ch2_rate = pow(10.0, ch2_db / 20);

  uint8_t ch1_gain[4] = {0x00};
  ch1_gain[0] = ((uint32_t)(ch1_rate * pow(2, 23)) >> 24) & 0x000000FF;
  ch1_gain[1] = ((uint32_t)(ch1_rate * pow(2, 23)) >> 16) & 0x000000FF;
  ch1_gain[2] = ((uint32_t)(ch1_rate * pow(2, 23)) >> 8)  & 0x000000FF;
  ch1_gain[3] =  (uint32_t)(ch1_rate * pow(2, 23))        & 0x000000FF;

  uint8_t ch2_gain[4] = {0x00};
  ch2_gain[0] = ((uint32_t)(ch2_rate * pow(2, 23)) >> 24) & 0x000000FF;
  ch2_gain[1] = ((uint32_t)(ch2_rate * pow(2, 23)) >> 16) & 0x000000FF;
  ch2_gain[2] = ((uint32_t)(ch2_rate * pow(2, 23)) >> 8)  & 0x000000FF;
  ch2_gain[3] =  (uint32_t)(ch2_rate * pow(2, 23))        & 0x000000FF;

  ADI_REG_U8 if_step[4] = {0x00, 0x00, 0x80, 0x00};

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, ch1_gain);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_IF1_ALG0_TARGET_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, if_step);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_IF1_ALG0_STEP_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, ch2_gain);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_IF2_ALG0_TARGET_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, if_step);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_IF2_ALG0_STEP_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
}

void send_xfader(uint32_t *xf_adc, double xf_curve, bool xf_rev)
{
  if (xf_adc[0] != xf0_adc[0] && xf_adc[1] != xf0_adc[1])
    {
      double xf1 = (xf_rev ? xf_adc[1] : xf_adc[0]) / 2047.0;
      if (xf1 < 0.0064)
        xf1 = 0.0;
      else if (xf1 > 1.0)
        xf1 = 1.0;

      double xf11 = pow(xf1, pow(2.0, 2.0 * xf_curve - 8.0));
      uint32_t xf11_u32 = (uint32_t)(xf11 * pow(2, 23));
      xf1_val[0] = (xf11_u32 >> 24) & 0x000000FF;
      xf1_val[1] = (xf11_u32 >> 16) & 0x000000FF;
      xf1_val[2] = (xf11_u32 >> 8)  & 0x000000FF;
      xf1_val[3] =  xf11_u32        & 0x000000FF;

      double xf2 = (xf_rev ? xf_adc[0] : xf_adc[1]) / 2047.0;
      if (xf2 < 0.0064)
        xf2 = 0.0;
      else if (xf2 > 1.0)
        xf2 = 1.0;

      double xf12 = pow(xf2, pow(2.0, 2.0 * xf_curve - 8.0));
      uint32_t xf12_u32 = (uint32_t)(xf12 * pow(2, 23));
      xf2_val[0] = (xf12_u32 >> 24) & 0x000000FF;
      xf2_val[1] = (xf12_u32 >> 16) & 0x000000FF;
      xf2_val[2] = (xf12_u32 >> 8)  & 0x000000FF;
      xf2_val[3] =  xf12_u32        & 0x000000FF;

      SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, xf1_val);
      SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_DC1_DCINPALG1_ADDR);
      SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, xf2_val);
      SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_DC2_DCINPALG2_ADDR);

      SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
    }

    xf0_adc[0] = xf_adc[0];
    xf0_adc[1] = xf_adc[1];
}

void send_pitch_shifter(uint32_t xf_adc)
{
  double pitch_trans = (((xf_adc / 4095.0) * 2.0 - 1.0) * 360.0) / SAMPLE_RATE;
  uint32_t pitch_u32 = 0;

  if (pitch_trans >= 0.0)
  {
    pitch_u32 = (uint32_t)(pitch_trans * pow(2, 23));
  }
  else
  {
    pitch_u32 = ~((uint32_t)(fabs(pitch_trans) * pow(2, 23))) + 1;
  }

  ADI_REG_U8 pt_val[4] = {0x00};
  pt_val[0] = (pitch_u32 >> 24) & 0x000000FF;
  pt_val[1] = (pitch_u32 >> 16) & 0x000000FF;
  pt_val[2] = (pitch_u32 >> 8)  & 0x000000FF;
  pt_val[3] =  pitch_u32        & 0x000000FF;

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, pt_val);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_PITCHTRANSL1_ALG0_PITCHSHIFTSALG1FREQ_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, pt_val);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_PITCHTRANSL2_ALG0_PITCHSHIFTSALG2FREQ_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
}

void send_lpf(uint32_t value)
{
  ADI_REG_U8 low_pass_b0[4];
  ADI_REG_U8 low_pass_b1[4];
  ADI_REG_U8 low_pass_b2[4];
  ADI_REG_U8 low_pass_a1[4];
  ADI_REG_U8 low_pass_a2[4];

  double low_pass_gain = 1.0;
  //double Q = 1.41;
  double Q = 2.82;//8.1;

  //double freq = ((double)value / 4095.0) * 20000.0;
  //double freq = pow(10.0, ((double)value / 4095.0) * 2) * 200.0;
  //double freq = pow(100.0, ((double)value / 4095.0) * 2) * 2.0 + 198.0 * (1.0 - (double)value / 4095.0);
  //double freq = pow(100.0, (tanh((double)value / 4095.0 * 3.0) * 2.0)) * 2.08 + 198.0 * (1.0 - tanh((double)value / 4095.0 * 3.0));
  double freq = (tanh((((double)value / 4095.0) - 1.0) * 3.0) + 1.0) * 20000 + 198.0 * (1 - ((double)value / 4095.0));

  double w0 = 2.0 * M_PI * freq / SAMPLE_RATE;
  double a = sin(w0) / (2.0 * Q);

  double b0 = (1.0 - cos(w0)) * low_pass_gain / 2.0;
  double b1 = 1.0 - cos(w0) * low_pass_gain;
  double b2 = (1.0 - cos(w0)) * low_pass_gain / 2.0;
  double a0 = 1.0 + a;
  double a1 = -2.0 * cos(w0);
  double a2 = 1 - a;

  b0 /= a0;
  b1 /= a0;
  b2 /= a0;
  a1 /= -a0;
  a2 /= -a0;

  uint32_t b0_u32 = (int32_t)(b0 * pow(2, 23));
  uint32_t b1_u32 = (int32_t)(b1 * pow(2, 23));
  uint32_t b2_u32 = (int32_t)(b2 * pow(2, 23));
  uint32_t a1_u32 = (int32_t)(a1 * pow(2, 23));
  uint32_t a2_u32 = (int32_t)(a2 * pow(2, 23));

  low_pass_b0[0] = (b0_u32 >> 24) & 0x000000FF;
  low_pass_b0[1] = (b0_u32 >> 16) & 0x000000FF;
  low_pass_b0[2] = (b0_u32 >> 8)  & 0x000000FF;
  low_pass_b0[3] =  b0_u32        & 0x000000FF;
  low_pass_b1[0] = (b1_u32 >> 24) & 0x000000FF;
  low_pass_b1[1] = (b1_u32 >> 16) & 0x000000FF;
  low_pass_b1[2] = (b1_u32 >> 8)  & 0x000000FF;
  low_pass_b1[3] =  b1_u32        & 0x000000FF;
  low_pass_b2[0] = (b2_u32 >> 24) & 0x000000FF;
  low_pass_b2[1] = (b2_u32 >> 16) & 0x000000FF;
  low_pass_b2[2] = (b2_u32 >> 8)  & 0x000000FF;
  low_pass_b2[3] =  b2_u32        & 0x000000FF;
  low_pass_a1[0] = (a1_u32 >> 24) & 0x000000FF;
  low_pass_a1[1] = (a1_u32 >> 16) & 0x000000FF;
  low_pass_a1[2] = (a1_u32 >> 8)  & 0x000000FF;
  low_pass_a1[3] =  a1_u32        & 0x000000FF;
  low_pass_a2[0] = (a2_u32 >> 24) & 0x000000FF;
  low_pass_a2[1] = (a2_u32 >> 16) & 0x000000FF;
  low_pass_a2[2] = (a2_u32 >> 8)  & 0x000000FF;
  low_pass_a2[3] =  a2_u32        & 0x000000FF;

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, low_pass_b0);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_LPF1_ALG0_STAGE0_B0_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, low_pass_b1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_LPF1_ALG0_STAGE0_B1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, low_pass_b2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_LPF1_ALG0_STAGE0_B2_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, low_pass_a1);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_LPF1_ALG0_STAGE0_A1_ADDR);

  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_4, 4, low_pass_a2);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_4, MOD_LPF1_ALG0_STAGE0_A2_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
}

void send_select_fx(uint8_t type)
{
  ADI_REG_U8 effect_on[4] = {0x00, 0x80, 0x00, 0x00};
  ADI_REG_U8 effect_off[4] = {0x00, 0x00, 0x00, 0x00};
  ADI_REG_U8 effect_return[4] = {0x00, 0x00, 0x00, 0x00};

  switch (type)
  {
  case 0: // BOTH NO EFFECT
    effect_return[3] = 0x00;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND1_ALG0_STEREODEMUXSLEW10_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND1_ALG0_STEREODEMUXSLEW11_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND1_ALG0_STEREODEMUXSLEW12_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN1_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);

    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND2_ALG0_STEREODEMUXSLEW20_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND2_ALG0_STEREODEMUXSLEW21_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND2_ALG0_STEREODEMUXSLEW22_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN2_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
    break;
  case 1: // CH1 PITCH SHIFTER & CH2 NO EFFECT
    effect_return[3] = 0x01;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND1_ALG0_STEREODEMUXSLEW10_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND1_ALG0_STEREODEMUXSLEW11_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND1_ALG0_STEREODEMUXSLEW12_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN1_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);

    effect_return[3] = 0x00;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND2_ALG0_STEREODEMUXSLEW20_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND2_ALG0_STEREODEMUXSLEW21_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND2_ALG0_STEREODEMUXSLEW22_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN2_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
    break;
  case 2: // CH1 LPF & CH2 NO EFFECT
    effect_return[3] = 0x02;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND1_ALG0_STEREODEMUXSLEW10_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND1_ALG0_STEREODEMUXSLEW11_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND1_ALG0_STEREODEMUXSLEW12_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN1_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);

    effect_return[3] = 0x00;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND2_ALG0_STEREODEMUXSLEW20_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND2_ALG0_STEREODEMUXSLEW21_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND2_ALG0_STEREODEMUXSLEW22_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN2_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
    break;
  case 3: // CH1 NO EFFECT & CH2 PITCH SHIFTER
    effect_return[3] = 0x00;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND1_ALG0_STEREODEMUXSLEW10_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND1_ALG0_STEREODEMUXSLEW11_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND1_ALG0_STEREODEMUXSLEW12_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN1_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);

    effect_return[3] = 0x01;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND2_ALG0_STEREODEMUXSLEW20_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND2_ALG0_STEREODEMUXSLEW21_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND2_ALG0_STEREODEMUXSLEW22_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN2_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
    break;
  case 4: // CH1 NO EFFECT & CH2 LPF
    effect_return[3] = 0x00;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND1_ALG0_STEREODEMUXSLEW10_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND1_ALG0_STEREODEMUXSLEW11_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND1_ALG0_STEREODEMUXSLEW12_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN1_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);

    effect_return[3] = 0x02;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND2_ALG0_STEREODEMUXSLEW20_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND2_ALG0_STEREODEMUXSLEW21_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND2_ALG0_STEREODEMUXSLEW22_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN2_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
    break;
  case 5: // CH1 PITCH SHIFTER & CH2 LPF
    effect_return[3] = 0x01;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND1_ALG0_STEREODEMUXSLEW10_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND1_ALG0_STEREODEMUXSLEW11_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND1_ALG0_STEREODEMUXSLEW12_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN1_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);

    effect_return[3] = 0x02;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND2_ALG0_STEREODEMUXSLEW20_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND2_ALG0_STEREODEMUXSLEW21_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND2_ALG0_STEREODEMUXSLEW22_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN2_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
    break;
  case 6: // CH1 LPF & CH2 PITCH SHIFTER
    effect_return[3] = 0x02;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND1_ALG0_STEREODEMUXSLEW10_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND1_ALG0_STEREODEMUXSLEW11_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND1_ALG0_STEREODEMUXSLEW12_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN1_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);

    effect_return[3] = 0x01;
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_FXSND2_ALG0_STEREODEMUXSLEW20_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, effect_on);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_FXSND2_ALG0_STEREODEMUXSLEW21_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, effect_off);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_FXSND2_ALG0_STEREODEMUXSLEW22_ADDR);
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, effect_return);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_FXRTN2_STEREOSWSLEW_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
    break;
  }
}

void send_master_booth_gain(uint32_t master_val, uint32_t booth_val)
{
  double master_db = (master_val / 127.0) - 1.0;
  double booth_db = (booth_val / 127.0) - 1.0;

  if (master_db > 0.0)
  {
    master_db *= 15.0;
  }
  else if(master_db < 0.0)
  {
    master_db *= 120.0;
  }

  if (booth_db > 0.0)
  {
    booth_db *= 15.0;
  }
  else if(booth_db < 0.0)
  {
    booth_db *= 120.0;
  }

  double master_rate = pow(10.0, master_db / 20);
  double booth_rate = pow(10.0, booth_db / 20);

  uint8_t master_gain[4] = {0x00};
  master_gain[0] = ((uint32_t)(master_rate * pow(2, 23)) >> 24) & 0x000000FF;
  master_gain[1] = ((uint32_t)(master_rate * pow(2, 23)) >> 16) & 0x000000FF;
  master_gain[2] = ((uint32_t)(master_rate * pow(2, 23)) >> 8)  & 0x000000FF;
  master_gain[3] =  (uint32_t)(master_rate * pow(2, 23))        & 0x000000FF;

#if 0
  uint8_t booth_gain[4] = {0x00};
  booth_gain[0] = ((uint32_t)(booth_rate * pow(2, 23)) >> 24) & 0x000000FF;
  booth_gain[1] = ((uint32_t)(booth_rate * pow(2, 23)) >> 16) & 0x000000FF;
  booth_gain[2] = ((uint32_t)(booth_rate * pow(2, 23)) >> 8)  & 0x000000FF;
  booth_gain[3] =  (uint32_t)(booth_rate * pow(2, 23))        & 0x000000FF;
#endif

  ADI_REG_U8 if_step[4] = {0x00, 0x00, 0x80, 0x00};

#if 0
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, booth_gain);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_BOOTHGAIN_ALG0_TARGET_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, if_step);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_BOOTHGAIN_ALG0_STEP_ADDR);
#endif
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, master_gain);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_MASTERGAIN_ALG0_TARGET_ADDR);
  SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, if_step);
  SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_MASTERGAIN_ALG0_STEP_ADDR);
  SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
}
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
  double ch1_ig = ch1_val / 4095.0;
  double ch2_ig = ch2_val / 4095.0;

  uint8_t ch1_gain[4] = {0x00};
  ch1_gain[0] = ((uint32_t)(ch1_ig * pow(2, 23)) >> 24) & 0x000000FF;
  ch1_gain[1] = ((uint32_t)(ch1_ig * pow(2, 23)) >> 16) & 0x000000FF;
  ch1_gain[2] = ((uint32_t)(ch1_ig * pow(2, 23)) >> 8)  & 0x000000FF;
  ch1_gain[3] =  (uint32_t)(ch1_ig * pow(2, 23))        & 0x000000FF;

  uint8_t ch2_gain[4] = {0x00};
  ch2_gain[0] = ((uint32_t)(ch2_ig * pow(2, 23)) >> 24) & 0x000000FF;
  ch2_gain[1] = ((uint32_t)(ch2_ig * pow(2, 23)) >> 16) & 0x000000FF;
  ch2_gain[2] = ((uint32_t)(ch2_ig * pow(2, 23)) >> 8)  & 0x000000FF;
  ch2_gain[3] =  (uint32_t)(ch2_ig * pow(2, 23))        & 0x000000FF;

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

void send_xfader(uint32_t *xf_adc)
{
  if (xf_adc[0] != xf0_adc[0] && xf_adc[1] != xf0_adc[1])
    {
      //double xf1 = xf_adc[0] / 4095.0;
      double xf1 = pow(10.0, (((xf_adc[0] / 2047.0) - 1.0) * 2.0)) - 0.01;
      if (xf1 < 0.0008)
        xf1 = 0.0;
      else if (xf1 > 1.0)
        xf1 = 1.0;

      //double xf2 = xf_adc[1] / 4095.0;
      //double xf2 = 20 * log10(xf_adc[1] / 4095.0) / 40.0 + 1.0;
      double xf2 = pow(10.0, (((xf_adc[1] / 2047.0) - 1.0) * 2.0)) - 0.01;
      if (xf2 < 0.0008)
        xf2 = 0.0;
      else if (xf2 > 1.0)
        xf2 = 1.0;

      double xf11 = pow(xf1, pow(2.0, -8));
      double xf12 = pow(xf2, pow(2.0, -8));

      xf1_val[0] = ((uint32_t)(xf11 * pow(2, 23)) >> 24) & 0x000000FF;
      xf1_val[1] = ((uint32_t)(xf11 * pow(2, 23)) >> 16) & 0x000000FF;
      xf1_val[2] = ((uint32_t)(xf11 * pow(2, 23)) >> 8)  & 0x000000FF;
      xf1_val[3] =  (uint32_t)(xf11 * pow(2, 23))        & 0x000000FF;

      xf2_val[0] = ((uint32_t)(xf12 * pow(2, 23)) >> 24) & 0x000000FF;
      xf2_val[1] = ((uint32_t)(xf12 * pow(2, 23)) >> 16) & 0x000000FF;
      xf2_val[2] = ((uint32_t)(xf12 * pow(2, 23)) >> 8)  & 0x000000FF;
      xf2_val[3] =  (uint32_t)(xf12 * pow(2, 23))        & 0x000000FF;

      SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, xf2_val);
      SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_XF1_ALG0_TARGET_ADDR);
      SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, xf_step);
      SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_XF1_ALG0_STEP_ADDR);
      SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, xf2_val);
      SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_XF1_ALG1_TARGET_ADDR);
      SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, xf_step);
      SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_XF1_ALG1_STEP_ADDR);
      SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);

      SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, xf1_val);
      SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_XF2_ALG0_TARGET_ADDR);
      SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, xf_step);
      SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_XF2_ALG0_STEP_ADDR);
      SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_2, 4, xf1_val);
      SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_2, MOD_XF2_ALG1_TARGET_ADDR);
      SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_3, 4, xf_step);
      SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_3, MOD_XF2_ALG1_STEP_ADDR);
      SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);

      //SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_0, 4, xf2_val);
      //SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_0, MOD_XF_ALG0_STAGE0_VOLUME_ADDR);

      //SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_DATA_1, 4, xf1_val);
      //SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_2, SIGMA_SAFELOAD_ADDR_1, MOD_XF_ALG0_STAGE1_VOLUME_ADDR);
      //SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_2);
    }

    xf0_adc[0] = xf_adc[0];
    xf0_adc[1] = xf_adc[1];
}

void send_high_shelf_ch1(uint32_t value)
{
  static uint32_t eq_count = 0;

  ADI_REG_U8 high_shelf_b0[4];
  ADI_REG_U8 high_shelf_b1[4];
  ADI_REG_U8 high_shelf_b2[4];
  ADI_REG_U8 high_shelf_a1[4];
  ADI_REG_U8 high_shelf_a2[4];

  double high_shelf_gain = (value / 4095.0) * 20.0 - 10.0;

  double w0 = 2.0 * M_PI * 6500.0 / 48000.0;
  double A = pow(10.0, high_shelf_gain / 20.0);
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

  high_shelf_b0[0] = ((int32_t)(b0 * pow(2, 23)) >> 24) & 0x000000FF;
  high_shelf_b0[1] = ((int32_t)(b0 * pow(2, 23)) >> 16) & 0x000000FF;
  high_shelf_b0[2] = ((int32_t)(b0 * pow(2, 23)) >> 8)  & 0x000000FF;
  high_shelf_b0[3] = ((int32_t)(b0 * pow(2, 23)))       & 0x000000FF;
  high_shelf_b1[0] = ((int32_t)(b1 * pow(2, 23)) >> 24) & 0x000000FF;
  high_shelf_b1[1] = ((int32_t)(b1 * pow(2, 23)) >> 16) & 0x000000FF;
  high_shelf_b1[2] = ((int32_t)(b1 * pow(2, 23)) >> 8)  & 0x000000FF;
  high_shelf_b1[3] = ((int32_t)(b1 * pow(2, 23)))       & 0x000000FF;
  high_shelf_b2[0] = ((int32_t)(b2 * pow(2, 23)) >> 24) & 0x000000FF;
  high_shelf_b2[1] = ((int32_t)(b2 * pow(2, 23)) >> 16) & 0x000000FF;
  high_shelf_b2[2] = ((int32_t)(b2 * pow(2, 23)) >> 8)  & 0x000000FF;
  high_shelf_b2[3] = ((int32_t)(b2 * pow(2, 23)))       & 0x000000FF;
  high_shelf_a1[0] = ((int32_t)(a1 * pow(2, 23)) >> 24) & 0x000000FF;
  high_shelf_a1[1] = ((int32_t)(a1 * pow(2, 23)) >> 16) & 0x000000FF;
  high_shelf_a1[2] = ((int32_t)(a1 * pow(2, 23)) >> 8)  & 0x000000FF;
  high_shelf_a1[3] = ((int32_t)(a1 * pow(2, 23)))       & 0x000000FF;
  high_shelf_a2[0] = ((int32_t)(a2 * pow(2, 23)) >> 24) & 0x000000FF;
  high_shelf_a2[1] = ((int32_t)(a2 * pow(2, 23)) >> 16) & 0x000000FF;
  high_shelf_a2[2] = ((int32_t)(a2 * pow(2, 23)) >> 8)  & 0x000000FF;
  high_shelf_a2[3] = ((int32_t)(a2 * pow(2, 23)))       & 0x000000FF;

  if(eq_count == 0)
  {
    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_0, 4, high_shelf_b0);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_0, MOD_EQ1_ALG0_STAGE0_B0_ADDR);

    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_1, 4, high_shelf_b1);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_1, MOD_EQ1_ALG0_STAGE0_B1_ADDR);

    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_2, 4, high_shelf_b2);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_2, MOD_EQ1_ALG0_STAGE0_B2_ADDR);

    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_3, 4, high_shelf_a1);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_3, MOD_EQ1_ALG0_STAGE0_A1_ADDR);

    SIGMA_SAFELOAD_WRITE_DATA(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_DATA_4, 4, high_shelf_a2);
    SIGMA_SAFELOAD_WRITE_ADDR(DEVICE_ADDR_IC_1, SIGMA_SAFELOAD_ADDR_4, MOD_EQ1_ALG0_STAGE0_A2_ADDR);
    SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(DEVICE_ADDR_IC_1);
  }

  eq_count++;
  eq_count = (eq_count % 16);
}

void send_mid_peaking_ch1(uint32_t value)
{
  static uint32_t eq_count = 0;

  ADI_REG_U8 peaking_b0[4];
  ADI_REG_U8 peaking_b1[4];
  ADI_REG_U8 peaking_b2[4];
  ADI_REG_U8 peaking_a1[4];
  ADI_REG_U8 peaking_a2[4];

  double peaking_gain = (value / 4095.0) * 20.0 - 10.0;

  double A = pow(10.0, peaking_gain / 20.0);
  double Q = 1.41;
  double w0 = 2.0 * M_PI * 1200.0 / 48000.0;
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

  if(eq_count == 0)
  {
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

  eq_count++;
  eq_count = (eq_count % 16);
}

void send_low_shelf_ch1(uint32_t value)
{
  static uint32_t eq_count = 0;

  ADI_REG_U8 low_shelf_b0[4];
  ADI_REG_U8 low_shelf_b1[4];
  ADI_REG_U8 low_shelf_b2[4];
  ADI_REG_U8 low_shelf_a1[4];
  ADI_REG_U8 low_shelf_a2[4];

  double low_shelf_gain = (value / 4095.0) * 20.0 - 10.0;

  double w0 = 2.0 * M_PI * 200.0 / 48000.0;
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

  if(eq_count == 0)
  {
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

  eq_count++;
  eq_count = (eq_count % 16);
}
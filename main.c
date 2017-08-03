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
 * main.c
 */

#ifndef GENERATION_DONE
#error You must run generate first!
#endif

/* Board headers */
#include "boards.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "aat.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#ifdef FEATURE_BOARD_DETECTED
#include "bspconfig.h"
#include "pti.h"
#endif

/* Device initialization header */
#include "InitDevice.h"

#ifdef FEATURE_SPI_FLASH
#include "em_usart.h"
#include "mx25flash_spi.h"
#endif /* FEATURE_SPI_FLASH */

#include "em_msc.h"
#include "em_adc.h"

#include "mixer.h"

#define CONTROL_SIGMADSP

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

#ifdef FEATURE_PTI_SUPPORT
static const RADIO_PTIInit_t ptiInit = RADIO_PTI_INIT;
#endif

/* Gecko configuration parameters (see gecko_configuration.h) */
static const gecko_configuration_t config = {
  .config_flags=0,
  .sleep.flags=SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections=MAX_CONNECTIONS,
  .bluetooth.heap=bluetooth_stack_heap,
  .bluetooth.heap_size=sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb=&bg_gattdb_data,
  .ota.flags=0,
  .ota.device_name_len=3,
  .ota.device_name_ptr="OTA",
  #ifdef FEATURE_PTI_SUPPORT
  .pti = &ptiInit,
  #endif
};

/* Flag for indicating DFU Reset must be performed */
uint8_t boot_to_dfu = 0;

uint32_t settings[18] = {0x00000000};

bool xf_rev = false;
double xf_curve = 4.0;

void init_settings()
{
  settings[0]  = 0x00; // ch1 line/phono sw(upper) , ch2 line/phono sw(lower)
  settings[1]  = 0x7F; // ch1 input gain
  settings[2]  = 0x7F; // ch2 input gain
  settings[3]  = 0x7F; // ch1 eq hi
  settings[4]  = 0x7F; // ch2 eq hi
  settings[5]  = 0x7F; // ch1 eq mid
  settings[6]  = 0x7F; // ch2 eq mid
  settings[7]  = 0x7F; // ch1 eq lo
  settings[8]  = 0x7F; // ch2 eq lo
  settings[9]  = 0xFF; // ch1 input fader
  settings[10] = 0xFF; // ch2 input fader
  settings[11] = 0x04; // input fader setting
  settings[12] = 0x02; // cross fader setting
  settings[13] = 0x7F; // master gain
  settings[14] = 0x7F; // booth gain
  settings[15] = 0x00; // monitor select
  settings[16] = 0x7F; // monitor gain
  settings[17] = 0x00; // effect select
}

void reset_settings()
{
  MSC_Init();

  settings[0]  = 0x00; // ch1 line/phono sw(upper) , ch2 line/phono sw(lower)
  settings[1]  = 0x7F; // ch1 input gain
  settings[2]  = 0x7F; // ch2 input gain
  settings[3]  = 0x7F; // ch1 eq hi
  settings[4]  = 0x7F; // ch2 eq hi
  settings[5]  = 0x7F; // ch1 eq mid
  settings[6]  = 0x7F; // ch2 eq mid
  settings[7]  = 0x7F; // ch1 eq lo
  settings[8]  = 0x7F; // ch2 eq lo
  settings[9]  = 0xFF; // ch1 input fader
  settings[10] = 0xFF; // ch2 input fader
  settings[11] = 0x04; // input fader setting
  settings[12] = 0x02; // cross fader setting
  settings[13] = 0x7F; // master gain
  settings[14] = 0x7F; // booth gain
  settings[15] = 0x00; // monitor select
  settings[16] = 0x7F; // monitor gain
  settings[17] = 0x00; // effect select

  uint32_t *w_addr = (uint32_t *)(0x3e000 + 0 * 4);
  MSC_ErasePage(w_addr);
  MSC_WriteWord(w_addr, (void const *)(&settings[0]), 18 * 4);

  MSC_Deinit();

  send_line_phono_switch(settings[0]);
  send_input_gain(settings[1], settings[2]);
  send_hi_shelf_ch1(settings[3]);
  send_hi_shelf_ch2(settings[4]);
  send_mid_peaking_ch1(settings[5]);
  send_mid_peaking_ch2(settings[6]);
  send_low_shelf_ch1(settings[7]);
  send_low_shelf_ch2(settings[8]);
  send_ifader(settings[9], settings[10]);
  send_master_booth_gain(settings[13], settings[14]);
  send_select_fx(settings[17]);

  // xfader setting
  if ((settings[12] >> 4) & 0x01 == 0x01)
  {
    xf_rev = true;
  }
  else
  {
    xf_rev = false;
  }
  xf_curve = (double)(settings[12] & 0x0F);

  uint8_t test[18] = {0};
  for (int i = 0; i < 18; i++)
  {
    test[i] = (uint8_t)settings[i];
  }
  gecko_cmd_gatt_server_write_attribute_value(gattdb_settings_read, 0, 18, test);
}

void write_settings()
{
  MSC_Init();

  uint32_t *w_addr = (uint32_t *)(0x3e000 + 0 * 4);
  MSC_ErasePage(w_addr);
  MSC_WriteWord(w_addr, (void const *)(&settings[0]), 18 * 4);

  MSC_Deinit();

  uint8_t test[18] = {0};
  for (int i = 0; i < 18; i++)
  {
    test[i] = (uint8_t)settings[i];
  }
  gecko_cmd_gatt_server_write_attribute_value(gattdb_settings_read, 0, 18, test);
}

void read_settings()
{
  for (int i = 0; i < 18; i++)
  {
    uint32_t *r_addr = (uint32_t *)(0x3e000 + i * 4);
    settings[i] = *r_addr;
  }

  send_line_phono_switch(settings[0]);
  send_input_gain(settings[1], settings[2]);
  send_hi_shelf_ch1(settings[3]);
  send_hi_shelf_ch2(settings[4]);
  send_mid_peaking_ch1(settings[5]);
  send_mid_peaking_ch2(settings[6]);
  send_low_shelf_ch1(settings[7]);
  send_low_shelf_ch2(settings[8]);
  send_ifader(settings[9], settings[10]);
  send_master_booth_gain(settings[13], settings[14]);
  send_select_fx(settings[17]);

  // xfader setting
  if ((settings[12] >> 4) & 0x01 == 0x01)
  {
    xf_rev = true;
  }
  else
  {
    xf_rev = false;
  }
  xf_curve = (double)(settings[12] & 0x0F);

  uint8_t test[18] = {0};
  for (int i = 0; i < 18; i++)
  {
    test[i] = (uint8_t)settings[i];
  }
  gecko_cmd_gatt_server_write_attribute_value(gattdb_settings_read, 0, 18, test);
}

/**
 * @brief  Main function
 */
void main(void)
{
#ifdef FEATURE_SPI_FLASH
  /* Put the SPI flash into Deep Power Down mode for those radio boards where it is available */
  MX25_init();
  MX25_DP();
  /* We must disable SPI communication */
  USART_Reset(USART1);

#endif /* FEATURE_SPI_FLASH */

  /* Initialize peripherals */
  enter_DefaultMode_from_RESET();

  /* Initialize stack */
  gecko_init(&config);

#ifdef CONTROL_SIGMADSP
  bool is_mixer_initialized = false;
  if (GPIO_PinInGet(gpioPortA, 0))
  {
    mixer_init();
    is_mixer_initialized = true;
  }

  bool debounce_flag = false;
  uint8_t debounce_count = 0;
  uint32_t test_adc = 0;
  uint32_t xf_adc[2];
#endif

  double xf1_avg[16] = {0.0};
  double xf2_avg[16] = {0.0};

  uint8_t xf_type = 0;

  read_settings();

  while (1)
  {
#if 1
    /* Event pointer for handling events */
    struct gecko_cmd_packet* evt;

    /* Check for stack event. */
    //evt = gecko_wait_event();
    evt = gecko_peek_event();

#if 0
    uint32_t test_header = BGLIB_MSG_ID(evt->header);
    if (test_header != 536871096)
    {
      GPIO_PinOutSet(gpioPortA, 0);
    }
#endif

    /* Handle events */
    switch (BGLIB_MSG_ID(evt->header)) {

      /* This boot event is generated when the system boots up after reset.
       * Here the system is set to start advertising immediately after boot procedure. */
      case gecko_evt_system_boot_id:

        /* Set advertising parameters. 100ms advertisement interval. All channels used.
         * The first two parameters are minimum and maximum advertising interval, both in
         * units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */
        gecko_cmd_le_gap_set_adv_parameters(160,160,7);

        /* Start general advertising and enable connections. */
        gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
        break;

      case gecko_evt_le_connection_closed_id:

        /* Check if need to boot to dfu mode */
        if (boot_to_dfu) {
          /* Enter to DFU OTA mode */
          gecko_cmd_system_reset(2);
        }
        else {
          /* Restart advertising after client has disconnected */
          gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
        }
        break;

      /* Value of attribute changed from the local database by remote GATT client */
      case gecko_evt_gatt_server_attribute_value_id:
#ifdef CONTROL_SIGMADSP
        if (!is_mixer_initialized)
        {
          break;
        }
	/* Check if changed characteristic is the Immediate Alert level */
        if (gattdb_line_phono_switch == evt->data.evt_gatt_server_attribute_value.attribute)
        {
          uint8_t lps_val = evt->data.evt_gatt_server_attribute_value.value.data[0];

          settings[0] = lps_val;
          send_line_phono_switch(lps_val);
        }
        else if (gattdb_input_gain == evt->data.evt_gatt_server_attribute_value.attribute)
        {
          uint32_t ch1_gain = 0;
          uint32_t ch2_gain = 0;
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[3];
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[4] << 8;
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[5] << 16;
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[0];
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[1] << 8;
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[2] << 16;

          settings[1] = ch1_gain;
          settings[2] = ch2_gain;

          send_input_gain(ch1_gain, ch2_gain);
        }
        else if (gattdb_eq_hi == evt->data.evt_gatt_server_attribute_value.attribute)
        {
          uint32_t ch1_gain = 0;
          uint32_t ch2_gain = 0;
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[3];
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[4] << 8;
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[5] << 16;
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[0];
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[1] << 8;
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[2] << 16;

          settings[3] = ch1_gain;
          settings[4] = ch2_gain;

          send_hi_shelf_ch1(ch1_gain);
          send_hi_shelf_ch2(ch2_gain);
        }
        else if (gattdb_eq_mid == evt->data.evt_gatt_server_attribute_value.attribute)
        {
          uint32_t ch1_gain = 0;
          uint32_t ch2_gain = 0;
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[3];
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[4] << 8;
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[5] << 16;
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[0];
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[1] << 8;
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[2] << 16;

          settings[5] = ch1_gain;
          settings[6] = ch2_gain;

          send_mid_peaking_ch1(ch1_gain);
          send_mid_peaking_ch2(ch2_gain);
        }
        else if (gattdb_eq_low == evt->data.evt_gatt_server_attribute_value.attribute)
        {
          uint32_t ch1_gain = 0;
          uint32_t ch2_gain = 0;
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[3];
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[4] << 8;
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[5] << 16;
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[0];
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[1] << 8;
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[2] << 16;

          settings[7] = ch1_gain;
          settings[8] = ch2_gain;

          send_low_shelf_ch1(ch1_gain);
          send_low_shelf_ch2(ch2_gain);
        }
        else if (gattdb_input_fader == evt->data.evt_gatt_server_attribute_value.attribute)
        {
          uint32_t ch1_gain = 0;
          uint32_t ch2_gain = 0;
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[3];
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[4] << 8;
          ch1_gain += evt->data.evt_gatt_server_attribute_value.value.data[5] << 16;
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[0];
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[1] << 8;
          ch2_gain += evt->data.evt_gatt_server_attribute_value.value.data[2] << 16;

          settings[9] = ch1_gain;
          settings[10] = ch2_gain;

          send_ifader(ch1_gain, ch2_gain);
        }
        else if (gattdb_ifader_setting == evt->data.evt_gatt_server_attribute_value.attribute)
        {
        }
        else if (gattdb_xfader_setting == evt->data.evt_gatt_server_attribute_value.attribute)
        {
          uint8_t xf_setting_val = evt->data.evt_gatt_server_attribute_value.value.data[0];
          if ((xf_setting_val >> 4) & 0x01)
          {
            xf_rev = true;
          }
          else
          {
            xf_rev = false;
          }

          settings[12] = xf_setting_val;

          xf_curve = (double)(xf_setting_val & 0x0F);
        }
        else if (gattdb_master_booth_gain == evt->data.evt_gatt_server_attribute_value.attribute)
        {
          uint32_t master_gain = 0;
          uint32_t booth_gain = 0;
          master_gain += evt->data.evt_gatt_server_attribute_value.value.data[3];
          master_gain += evt->data.evt_gatt_server_attribute_value.value.data[4] << 8;
          master_gain += evt->data.evt_gatt_server_attribute_value.value.data[5] << 16;
          booth_gain += evt->data.evt_gatt_server_attribute_value.value.data[0];
          booth_gain += evt->data.evt_gatt_server_attribute_value.value.data[1] << 8;
          booth_gain += evt->data.evt_gatt_server_attribute_value.value.data[2] << 16;

          settings[13] = master_gain;
          settings[14] = booth_gain;

          send_master_booth_gain(master_gain, booth_gain);
        }
        else if (gattdb_monitor_level_select == evt->data.evt_gatt_server_attribute_value.attribute)
        {
        }
        else if (gattdb_effect_selector == evt->data.evt_gatt_server_attribute_value.attribute)
        {
          settings[17] = evt->data.evt_gatt_server_attribute_value.value.data[0];
          send_select_fx(settings[17]);

          send_pitch_shifter(2047, 0);
          send_lpf(4095);
        }
        else if (gattdb_settings_write == evt->data.evt_gatt_server_attribute_value.attribute)
        {
          if (evt->data.evt_gatt_server_attribute_value.value.data[0] == 1)
          {
            write_settings();
          }
          else if (evt->data.evt_gatt_server_attribute_value.value.data[0] == 2)
          {
            read_settings();
          }
          else if(evt->data.evt_gatt_server_attribute_value.value.data[0] == 3)
          {
            reset_settings();
          }
        }
#endif
        break;
      case gecko_evt_gatt_characteristic_id:
        break;

      case gecko_evt_gatt_characteristic_value_id:
        break;

      /* Events related to OTA upgrading
      ----------------------------------------------------------------------------- */

      /* Check if the user-type OTA Control Characteristic was written.
       * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
      case gecko_evt_gatt_server_user_write_request_id:

        if(evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control)
        {
          /* Set flag to enter to OTA mode */
          boot_to_dfu = 1;
          /* Send response to Write Request */
          gecko_cmd_gatt_server_send_user_write_response(
            evt->data.evt_gatt_server_user_write_request.connection,
            gattdb_ota_control,
            bg_err_success);

          /* Close connection to enter to DFU OTA mode */
          gecko_cmd_endpoint_close(evt->data.evt_gatt_server_user_write_request.connection);
        }
        else if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_xfader_setting)
        {
          //test uint8array test = evt->data.evt_gatt_server_user_write_request.value;
          //test uint8 test0 = test.data[0];
        }
        break;

      default:
        break;
    }
#endif

#ifdef CONTROL_SIGMADSP
    if (!is_mixer_initialized)
    {
      continue;
    }

#if 1
    for (uint8_t i = 0; i < 16; i++)
    {
      ADC_Start(ADC0, adcStartScan);
      while ((ADC0->IF & ADC_IF_SCAN) == 0)
        ;
      xf1_avg[i] = ADC_DataScanGet(ADC0);
      xf2_avg[i] = ADC_DataScanGet(ADC0);
      test_adc = ADC_DataScanGet(ADC0);

      for (uint32_t i = 0; i < 5000; i++)
        ;
    }
    double sum1 = 0.0;
    double sum2 = 0.0;
    for (uint8_t i = 0; i < 16; i++)
    {
      sum1 += xf1_avg[i];
      sum2 += xf2_avg[i];
    }
    xf_adc[0] = (uint32_t)(sum1 / 16.0);
    xf_adc[1] = (uint32_t)(sum2 / 16.0);
#else
    ADC_Start(ADC0, adcStartScan);
    while ((ADC0->IF & ADC_IF_SCAN) == 0)
      ;
    xf1_avg[avg_index] = ADC_DataScanGet(ADC0);
    xf2_avg[avg_index] = ADC_DataScanGet(ADC0);
    test_adc = ADC_DataScanGet(ADC0);

    avg_index = (avg_index + 1) % 16;
    double sum1 = 0.0;
    double sum2 = 0.0;
    for (uint8_t i = 0; i < 16; i++)
    {
      sum1 += xf1_avg[i];
      sum2 += xf2_avg[i];
    }
    xf_adc[0] = sum1 / 16.0;
    xf_adc[1] = sum2 / 16.0;
#endif

    if (GPIO_PinInGet(gpioPortA, 0))
    {
      if (debounce_flag)
      {
        debounce_count++;
        if (debounce_count == 4)
        {
          // off
          GPIO_PinOutClear(gpioPortA, 1);
          debounce_flag = false;
          debounce_count = 0;

          xf_type = 0;
          send_lpf(4095);
        }
      }
      else
      {
        debounce_count = 0;
      }
    }
    else
    {
      if (!debounce_flag)
      {
        debounce_count++;
        if (debounce_count == 4)
        {
          // on
          GPIO_PinOutSet(gpioPortA, 1);
          debounce_flag = true;
          debounce_count = 0;

          xf_type = 1;
        }
      }
      else
      {
        debounce_count = 0;
      }
    }

    switch (xf_type)
    {
    case 0:
      send_xfader(xf_adc, xf_curve, xf_rev);
      break;
    case 1:
      uint8_t pitch_type = 2;
      switch (settings[17])
      {
      case 1:
        send_pitch_shifter(xf_rev ? xf_adc[1] : xf_adc[0], pitch_type);
        break;
      case 2:
        send_lpf(xf_rev ? xf_adc[0] : xf_adc[1]);
        break;
      case 3:
        send_pitch_shifter(xf_rev ? xf_adc[0] : xf_adc[1], pitch_type);
        break;
      case 4:
        send_lpf(xf_rev ? xf_adc[1] : xf_adc[0]);
        break;
      case 5:
        send_pitch_shifter(xf_rev ? xf_adc[1] : xf_adc[0], pitch_type);
        send_lpf(xf_rev ? xf_adc[1] : xf_adc[0]);
        break;
      case 6:
        send_pitch_shifter(xf_rev ? xf_adc[0] : xf_adc[1], pitch_type);
        send_lpf(xf_rev ? xf_adc[0] : xf_adc[1]);
        break;
      }
      uint32_t center[2] = {2047, 2047};
      send_xfader(center, xf_curve, xf_rev);
      break;
    }

    const uint8 val[2] = {(uint32_t)xf_adc[0] >> 4, (uint32_t)xf_adc[1] >> 4};
    gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_cross_fader, 2, val);
#endif
  }
}


/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */

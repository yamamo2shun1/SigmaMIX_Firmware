/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

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
  mixer_init();
#if 0
  ADI_REG_U8 val0[4] = {0x00, 0x00, 0x00, 0xFF};
  ADI_REG_U8 val1[4] = {0x00, 0x02, 0x58, 0xBF};
  ADI_REG_U8 val2[4] = {0x00, 0x00, 0x00, 0x00};
#endif
  bool debounce_flag = false;
  uint8_t debounce_count = 0;
  uint32_t test_adc = 0;
  uint32_t xf_adc[2];
#endif

#if 1
  double xf1_avg[16] = {0.0};
  double xf2_avg[16] = {0.0};
#endif

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
	/* Check if changed characteristic is the Immediate Alert level */
        if ( gattdb_xfader_setting == evt->data.evt_gatt_server_attribute_value.attribute) {
          /* Write the Immediate Alert level value */
          //iaImmediateAlertWrite(&evt->data.evt_gatt_server_attribute_value.value);
          //test uint8_t test_len = evt->data.evt_gatt_server_attribute_value.value.len;
          uint8_t test_val = evt->data.evt_gatt_server_attribute_value.value.data[0];
          //uint8 test_val = test_array.data[0];
          //GPIO_PinOutSet(gpioPortA, 0);
#if 1
          switch (test_val)
          {
            case 0:
#ifdef CONTROL_SIGMADSP
              //val2[1] = 0x00;
              //SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_STATIC_TONE1_ALG0_MASK_ADDR, 4, val0);
              //SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_TONE1_ALG0_INCREMENT_ADDR,   4, val1);
              //SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_TONE1_ALG0_ON_ADDR,          4, val2);
#endif
              break;
            case 1:
#ifdef CONTROL_SIGMADSP
              //val2[1] = 0x80;
              //SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_STATIC_TONE1_ALG0_MASK_ADDR, 4, val0);
              //SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_TONE1_ALG0_INCREMENT_ADDR,   4, val1);
              //SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_TONE1_ALG0_ON_ADDR,          4, val2);
#endif
              break;
          }
          if (test_val == 1)
          {
            GPIO_PinOutSet(gpioPortA, 0);
          }
          else
          {
            GPIO_PinOutClear(gpioPortA, 0);
          }
#endif
        }
        break;
#if 0
      /* Indicates the changed value of CCC or received characteristic confirmation */
      case gecko_evt_gatt_server_characteristic_status_id:
        /* Check if changed client char config is for the temperature measurement */
        if ((gattdb_xfader_curve == evt->data.evt_gatt_server_attribute_value.attribute)
            && (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01)) {
          /* Call HTM temperature characteristic status changed callback */
          htmTemperatureCharStatusChange(
            evt->data.evt_gatt_server_characteristic_status.connection,
            evt->data.evt_gatt_server_characteristic_status.client_config_flags);
        }
        break;
#endif
      case gecko_evt_gatt_characteristic_id:
        GPIO_PinOutSet(gpioPortA, 0);
        break;

      case gecko_evt_gatt_characteristic_value_id:
        GPIO_PinOutClear(gpioPortA, 0);
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
#if 0
    if (GPIO_PinInGet(gpioPortA, 1))
    {
      if (debounce_flag)
      {
        debounce_count++;
        if (debounce_count == 16)
        {
          GPIO_PinOutClear(gpioPortA, 0);
          debounce_flag = false;
          debounce_count = 0;

          // Sine Tone
          val2[1] = 0x00;
          SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_STATIC_TONE1_ALG0_MASK_ADDR, 4, val0);
          SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_TONE1_ALG0_INCREMENT_ADDR,   4, val1);
          SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_TONE1_ALG0_ON_ADDR,          4, val2);
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
        if (debounce_count == 16)
        {
          GPIO_PinOutSet(gpioPortA, 0);
          debounce_flag = true;
          debounce_count = 0;

#if 1
          // Sine Tone
          val2[1] = 0x80;

          // ADI_REG_TYPE val1[4] = {0x00, 0x02, 0x58, 0xBF};
          if(test_adc < 2048)
          {
            val1[0] = 0x00;
            val1[1] = 0x02;
            val1[2] = 0x58;
            val1[3] = 0xBF;
          }
          else
          {
            val1[0] = ((uint32_t)((880.0 / 24000.0) * pow(2, 23)) >> 24) & 0x000000FF;
            val1[1] = ((uint32_t)((880.0 / 24000.0) * pow(2, 23)) >> 16) & 0x000000FF;
            val1[2] = ((uint32_t)((880.0 / 24000.0) * pow(2, 23)) >> 8)  & 0x000000FF;
            val1[3] =  (uint32_t)((880.0 / 24000.0) * pow(2, 23))        & 0x000000FF;
          }

          SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_STATIC_TONE1_ALG0_MASK_ADDR, 4, val0);
          SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_TONE1_ALG0_INCREMENT_ADDR,   4, val1);
          SIGMA_WRITE_REGISTER_CONTROL(DEVICE_ADDR_IC_1, MOD_TONE1_ALG0_ON_ADDR,          4, val2);
#endif
        }
      }
      else
      {
        debounce_count = 0;
      }
    }
#endif
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
    //send_high_shelf_ch1(test_adc);
    send_mid_peaking_ch1(test_adc);
    //send_low_shelf_ch1(test_adc);

    send_xfader(xf_adc);
#endif
  }
}


/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */

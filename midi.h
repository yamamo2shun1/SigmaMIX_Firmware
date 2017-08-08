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
 * midi.h
 */

#ifndef __MIXER_H__
#define __MIXER_H__

#include <stdint.h>

#include "em_cryotimer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t header;
  uint8_t timestamp;
  uint8_t status;
  uint8_t data1;
  uint8_t data2;
} midi_event_packet_t;

typedef union {
  midi_event_packet_t midi_event;
  uint8_t payload[5];
} midi_data_t;

midi_data_t* note_on(uint8_t pitch, uint8_t velocity);
midi_data_t* note_off(uint8_t pitch);
midi_data_t* control_change(uint8_t number, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif

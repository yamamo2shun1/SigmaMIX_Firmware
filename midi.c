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
 * midi.c
 */

#include "midi.h"

midi_data_t* note_on(uint8_t pitch, uint8_t velocity)
{
  static midi_data_t note_on;

  uint32_t temp = CRYOTIMER_CounterGet();
  temp = temp & 0x00001fff;

  note_on.midi_event.header    = 0x80 | (temp >> 7);
  note_on.midi_event.timestamp = 0x80 | (temp & 0x003f);
  note_on.midi_event.status    = 0x90;
  note_on.midi_event.data1     = pitch;
  note_on.midi_event.data2     = velocity;

  return &note_on;
}

midi_data_t* note_off(uint8_t pitch)
{
  static midi_data_t note_off;

  uint32_t temp = CRYOTIMER_CounterGet();
  temp = temp & 0x00001fff;

  note_off.midi_event.header    = 0x80 | (temp >> 7);
  note_off.midi_event.timestamp = 0x80 | (temp & 0x003f);
  note_off.midi_event.status    = 0x80;
  note_off.midi_event.data1     = pitch;
  note_off.midi_event.data2     = 0;

  return &note_off;
}

midi_data_t* control_change(uint8_t number, uint8_t value)
{
  static midi_data_t cc;

  uint32_t temp = CRYOTIMER_CounterGet(); /* 1ms timestamp */
  temp = temp & 0x00001fff;             /* only the lower 13 bit needed */

  cc.midi_event.header    = 0x80 | (temp >> 7);
  cc.midi_event.timestamp = 0x80 | (temp & 0x003f);
  cc.midi_event.status    = 0xB0;
  cc.midi_event.data1     = number;
  cc.midi_event.data2     = value;

  return &cc;
}
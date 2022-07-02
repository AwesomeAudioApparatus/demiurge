/*
  Copyright 2019, Awesome Audio Apparatus.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
      limitations under the License.
*/

#include <stdbool.h>
#include <stdint.h>

#ifndef _DEMIURGE_DEMIURGE_SPI_H_
#define _DEMIURGE_DEMIURGE_SPI_H_

// Hardware Demiurge API
void demiurge_tick();

// Hardware Driver SPI

typedef struct {
    const char *name;
    const uint16_t inputs;
    const uint16_t outputs;
    const uint16_t gates;
    const uint16_t buttons;
    const uint16_t leds;
    const uint16_t potentionmeters;
    const uint16_t flash;
    const uint16_t max_frequency;  // in kHz
    const uint16_t recommended_frequency; // in kHz
    const bool usb;
    const bool sdcard;
} demiurge_driver_info_t;

extern void demiurge_driver_init();
extern void demiurge_driver_start();
extern demiurge_driver_info_t *demiurge_driver_info();

// Gates
extern void init_gates();
extern void read_gates();
extern void update_gates();
extern void start_gates();
extern void stop_gates();
extern void direction_gate(int gate, bool output);

// ADCs
extern void init_adc();
extern void read_adc();
extern void start_adc();
extern void stop_adc();

// DACs
extern void init_dac();
extern void update_dac();
extern void start_dac();
extern void stop_dac();

// Timer
extern void init_timer(int samplerate);
extern void start_timer();
extern void stop_timer();

// LEDs
extern void update_leds();
extern void init_leds();
extern void start_leds();
extern void stop_leds();

// Buttons
extern void update_buttons();
extern void init_buttons();
extern void start_buttons();
extern void stop_buttons();

// Potentiometers
extern void update_potentiometers();
extern void init_potentiometers();
extern void start_potentiometers();
extern void stop_potentiometers();


// Test Points
extern void init_testpoints();
extern void start_testpoints();
extern void stop_testpoints();
extern void set_testpoint(int point);
extern void clear_testpoint(int point);

// USB
extern void update_usb();
extern void init_usb();
extern void start_usb();
extern void stop_usb();

// SDcard
extern void update_sdcard();
extern void init_sdcard();
extern void start_sdcard();
extern void stop_sdcard();

// Flash Memory
extern void update_flash();
extern void init_flash();
extern void start_flash();
extern void stop_flash();

#endif

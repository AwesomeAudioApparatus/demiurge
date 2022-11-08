/*
  Copyright 2019-2022, Awesome Audio Apparatus.

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

typedef struct
{
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
    const bool display;
    const bool button_navigation;
} demiurge_driver_info_t;

void demiurge_driver_init();

void demiurge_driver_start();

void demiurge_driver_stop();

demiurge_driver_info_t *demiurge_driver_info();

// Gates
void init_gates();

void read_gates();

void update_gates();

void start_gates();

void stop_gates();

void direction_gate(int gate, bool output);

// ADCs
void init_adc(float *scales, float *offsets);

void read_adc();

void start_adc();

void stop_adc();

// DACs
void init_dac(float *scales, float *offsets);

void update_dac();

void start_dac();

void stop_dac();

// Timer
void init_timer(int samplerate);

void start_timer();

void stop_timer();

// LEDs
void update_leds();

void init_leds();

void start_leds();

void stop_leds();

void set_led(uint8_t led, int fade);

void set_rgb(uint8_t position, uint8_t r, uint8_t g, uint8_t b);

/* Get the memory position for the R,G,B LEDs of a given position (1-n) */
uint8_t *rgb_leds(int position);

// Buttons
void update_buttons();

void init_buttons();

void start_buttons();

void stop_buttons();

// Potentiometers
void update_potentiometers();

void init_potentiometers();

void start_potentiometers();

void stop_potentiometers();

// Display
void update_display();

void init_display();

void start_display();

void stop_display();

// Button Navigation
void read_navigation();

void init_navigation();

void start_navigation();

void stop_navigation();

// Test Points
void init_testpoints();

void start_testpoints();

void stop_testpoints();

void set_testpoint(int point);

void clear_testpoint(int point);

void toggle_testpoint(int point);

#endif

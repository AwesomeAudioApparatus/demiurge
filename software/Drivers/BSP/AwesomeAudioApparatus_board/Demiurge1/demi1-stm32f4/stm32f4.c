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

#include "demiurge-hardware.h"

static demiurge_driver_info_t driver_info = {
    .name = "Awesome Audio Apparatus - Demiurge 1",
    .inputs = 4,
    .outputs = 2,
    .gates = 1,
    .buttons = 4,
    .leds = 4,
    .potentionmeters = 4,
    .usb = true,
    .flash = 16,
    .sdcard = true,
    .max_frequency = 48,
    .recommended_frequency = 24,
    .display = false,
    .button_navigation = false
};

float adc_scales[] = {
    -1 / 204.8, -1 / 204.8, -1 / 204.8, -1 / 204.8,  // CV1-CV4
    -1 / 204.8, -1 / 204.8, -1 / 204.8, -1 / 204.8   // RV1-RV4
};

float adc_offsets[] = {
    10.0, 10.0, 10.0, 10.0,    // CV1-CV4
    10.0, 10.0, 10.0, 10.0     // RV1-RV4
};

float calibration_cv[] = {
    0.0, 0.0, 0.0, 0.0,    // CV1-CV4
    0.0, 0.0, 0.0, 0.0     // RV1-RV4
};

float dac_scales[] = {204.7f, 204.7f};
float dac_offsets[] = {10.0f, 10.0f};

void demiurge_driver_init()
{
    init_testpoints();
    init_adc(adc_scales, adc_offsets);
    init_buttons();
    init_dac(dac_scales, dac_offsets);
    init_gates();
    init_leds();
    init_potentiometers();

    init_timer(demiurge_samplerate);
}

void demiurge_driver_start()
{
    start_testpoints();

    set_testpoint(0);
    clear_testpoint(0);

    start_adc();
    start_buttons();
    start_dac();
    start_gates();
    start_leds();
    start_potentiometers();

    set_testpoint(0);
    clear_testpoint(0);
    set_testpoint(0);
    clear_testpoint(0);

    start_timer();
}

void demiurge_driver_stop()
{
    stop_timer();

    set_testpoint(0);
    clear_testpoint(0);

    stop_potentiometers();
    stop_buttons();
    stop_adc();
    stop_dac();
    stop_gates();
    stop_leds();

    set_testpoint(0);
    clear_testpoint(0);

    stop_testpoints();
}

demiurge_driver_info_t *demiurge_driver_info()
{
    return &driver_info;
}

// Display - doesn't exist
void update_display()
{}

void init_display()
{}

void start_display()
{}

void stop_display()
{}

// Button Navigation - doesn't exist
void read_navigation()
{}

void init_navigation()
{}

void start_navigation()
{}

void stop_navigation()
{}

void demiurge_potentiometer_mode(int position, float min, float max)
{
    float k = (max - min) / 4095;
    adc_scales[position + DEMIURGE_POTENTIOMETER_OFFSET - 1] = -k;
    adc_offsets[position + DEMIURGE_POTENTIOMETER_OFFSET - 1] = max;
}

void demiurge_input_mode(int position, inout_type type)
{
    switch (type)
    {
        case CV:
            // Set ADC to a 0-10V range
            adc_scales[position + DEMIURGE_AUDIOINPUT_OFFSET - 1] = -1 / 204.8;
            adc_offsets[position + DEMIURGE_AUDIOINPUT_OFFSET - 1] = 10.0f + calibration_cv[position];
            break;
        case Audio:
            // Set ADC to a -10V to +10V range
            adc_scales[position + DEMIURGE_AUDIOINPUT_OFFSET - 1] = -1 / 204.8;
            adc_offsets[position + DEMIURGE_AUDIOINPUT_OFFSET - 1] = 10.0f;
            break;
        case Trig:
            break;
        case Gate:
            break;
    }
}

void demiurge_output_mode(int position, inout_type type)
{
    switch (type)
    {
        case CV:
            // Set DAC to a 0-10V range
            dac_scales[position + DEMIURGE_AUDIOOUTPUT_OFFSET - 1] = 204.7f;
            dac_offsets[position + DEMIURGE_AUDIOOUTPUT_OFFSET - 1] = 10.0f;
            break;
        case Audio:
            // Set DAC to a -10V to +10V range
            dac_scales[position + DEMIURGE_AUDIOOUTPUT_OFFSET - 1] = 204.7f;
            dac_offsets[position + DEMIURGE_AUDIOOUTPUT_OFFSET - 1] = 10.0f;
            break;
        case Trig:
            break;
        case Gate:
            break;
    }
}


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

#include "demiurge-board.h"

extern void vco_setup();
extern void vco_loop();

extern float adc_scales[];
extern float adc_offsets[];

int main(void)
{
    demiurge_samplerate = 48000;    // 48k samples/sec
    adc_scales[4] = -10.0f/4095.0f; // Volt/octave scaling
    adc_offsets[4] = 8.0f;          // to get from 1/15 Hz to 10kHz
    adc_scales[6] = -10.0f/4095.0f;
    adc_offsets[6] = 8.0f;
    demiurge_driver_init();         // Initialize the hardware driver
    demiurge_init();                // Initialize the Demiurge platform

    vco_setup();                    // Set up the functionality
    demiurge_start();               // Start the audio processing

    while(1)
    {
        vco_loop();                 // Give VCO time for LEDs and Buttons
    }
}



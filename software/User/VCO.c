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

#include <demiurge.h>

#define TAG "VCO"

static control_pair_t pair1;  // frequency
static control_pair_t pair2;  // frequency offset
static control_pair_t pair3;  // amplitude
static offset_t offset_freq;
static audio_outport_t out1;
static audio_outport_t out2;
static oscillator_t oscillator;

/* Simple VCO with triangle wave on both outputs. */
void vco_setup() {
   // Initialize the hardware configuration
   control_pair_init(&pair1, 1);     // FREQUENCY = CV+Pot at the top
   control_pair_init(&pair2, 2);     // FREQ TUNING = CV+Pot at the second position from the top
   control_pair_init(&pair3, 3);     // AMPLITUDE = CV+Pot at the second position from the top
   audio_outport_init(&out1, 1);     // Audio Out on left output channel
   audio_outport_init(&out2, 2);     // Audio Out on right output channel

   // Initialize internal blocks
   offset_init(&offset_freq);                // FREQ TUNING needs to adjust offset
   oscillator_init(&oscillator);             // Initialize the oscillator

   offset_configure_input(&offset_freq, &pair1.me);   // Top Pot+CV is the changing frequency
   offset_configure_control(&offset_freq, &pair2.me); // Second Pot+CV is the tuning of that frequency.

   // Set up the Oscillator to TRIANGLE wave form
   oscillator_configure_mode(&oscillator, TRIANGLE);
   oscillator_configure_frequency(&oscillator, &offset_freq.me);     // offset_freq block is controlling the frequency
   oscillator_configure_amplitude(&oscillator, &pair3.me);        // pair3 block is controlling the amplitude

   // Connect Oscillator to outputs
   audio_outport_configure_input(&out1, &oscillator.me);
   audio_outport_configure_input(&out2, &oscillator.me);
}

void vco_loop() {

}

#undef TAG

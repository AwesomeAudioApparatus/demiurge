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


static control_pair_t pair1;  // frequency
static control_pair_t pair2;  // amplitude

static control_pair_t pair3;  // frequency
static control_pair_t pair4;  // amplitude

static audio_outport_t out1;
static audio_outport_t out2;
static oscillator_t oscillator1;
static oscillator_t oscillator2;

/* Simple 2 VCO with sine wave */
void vco_setup() {
   // Initialize the hardware configuration
   control_pair_init(&pair1, 1);     // FREQUENCY = CV+Pot at the top
   control_pair_init(&pair2, 2);     // AMPLITUDE = CV+Pot at the second position from the top
   control_pair_init(&pair3, 3);     // FREQUENCY = CV+Pot at the third position from top
   control_pair_init(&pair4, 4);     // AMPLITUDE = CV+Pot at the bottom
   audio_outport_init(&out1, 1);     // Audio Out on left output channel
   audio_outport_init(&out2, 2);     // Audio Out on right output channel

   // Initialize the oscillators
   oscillator_init(&oscillator1);
   oscillator_init(&oscillator2);

   // Set up the oscillators to SINE wave form

   oscillator_configure_mode(&oscillator1, SAW);
   oscillator_configure_mode(&oscillator2, SAW);

   oscillator_configure_frequency(&oscillator1, &pair1.me);
   oscillator_configure_frequency(&oscillator2, &pair3.me);

   oscillator_configure_amplitude(&oscillator1, &pair2.me);
   oscillator_configure_amplitude(&oscillator2, &pair4.me);

   // Connect Oscillator to outputs
   audio_outport_configure_input(&out1, &oscillator1.me);
   audio_outport_configure_input(&out2, &oscillator2.me);
}

void vco_loop() {
   // handle buttons and LEDs
}

#undef TAG

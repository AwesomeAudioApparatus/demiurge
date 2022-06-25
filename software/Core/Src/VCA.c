/*
  Copyright 2019-2020, Awesome Audio Apparatus.

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

#include "demiurge.h"

static audio_inport_t in1;      // Declaration of Audio Input Ports
static audio_inport_t in2;

static control_pair_t pair1;    // Declaration of CV+Potentiometer Input pairs
static control_pair_t pair2;

static audio_outport_t out1;    // Declaration of Audio Output Ports
static audio_outport_t out2;

static mixer_t mixer;           // Declaration of a Mixer block

/*
 * A two port Mixer, with CV control
 */
void setup() {
   // Initialize the hardware configuration
   control_pair_init(&pair1, 1);       // CV+Pot at the top of Demiurge
   control_pair_init(&pair2, 2);       // CV+Pot at the second position from the top of Demiurge
   audio_inport_init(&in1, 3);         // Audio In on third input from the top
   audio_inport_init(&in2, 4);         // Audio In on fourth input from the top
   audio_outport_init(&out1, 1);       // Audio Out on left output channel
   audio_outport_init(&out2, 2);       // Audio Out on right output channel

   // Initialize Mixer with 2 channels.
   mixer_init(&mixer, 2);

   // Connect in1 on mixer channel 1, with pair1 as the volume control
   mixer_configure_input(&mixer, 1, &in1.me, &pair1.me);

   // Connect in2 on mixer channel 2, with pair2 as the volume control
   mixer_configure_input(&mixer, 2, &in2.me, &pair2.me);

   // Connect mixer output to out1
   audio_outport_configure_input(&out1, &mixer.me);

   // Connect mixer output to out2
   audio_outport_configure_input(&out2, &mixer.me);
}

void loop() {
}

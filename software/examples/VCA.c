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
#include "demiurge-board.h"

static audio_inport_t input1;      // Declaration of Audio Input Ports
static audio_inport_t input2;

static control_pair_t pair1;    // Declaration of CV+Potentiometer Input pairs
static control_pair_t pair2;

static audio_outport_t out1;    // Declaration of Audio Output Ports
static audio_outport_t out2;

static scale_t scale1;
static scale_t scale2;

/*
 * Dual VCA.
 */
void vca_prepare() {
    demiurge_samplerate = 50000;     // 30000 samples/second
    demiurge_set_inport_audio(1);    // set the first jack as Audio input.
    demiurge_set_inport_cv(2);       // set the second jack as Control Voltage input
    demiurge_set_inport_audio(3);    // set the third jack as Audio input.
    demiurge_set_inport_cv(4);       // set the fourth jack as Control Voltage input

    demiurge_set_potentiometer(2, 0.0f, 10.0f);  // Potentiometers to a 0-10 range
    demiurge_set_potentiometer(4, 0.0f, 10.0f);

    demiurge_set_outport_audio(1);   // Set the outputs to Audio output.
    demiurge_set_outport_audio(2);
}

void vca_setup() {

   control_pair_init(&pair1, 2);       // CV+Pot at the second position from the top of Demiurge
   control_pair_init(&pair2, 4);       // CV+Pot at the fourth position from the top of Demiurge
   audio_inport_init(&input1, 1);         // Audio In on first input from the top
   audio_inport_init(&input2, 3);         // Audio In on third input from the top
   audio_outport_init(&out1, 1);       // Audio Out on left output channel
   audio_outport_init(&out2, 2);       // Audio Out on right output channel

   scale_init(&scale1);
   scale_configure(&scale1, &input1.me, &pair1.me);  //
   scale1.scale = 0.1f;

   scale_init(&scale2);
   scale_configure(&scale2, &input2.me, &pair2.me);
   scale2.scale = 0.1f;

   // Connect mixer output to out1
   audio_outport_configure_input(&out1, &scale1.me);

   // Connect mixer output to out2
   audio_outport_configure_input(&out2, &scale2.me);
}

void vca_loop() {
}

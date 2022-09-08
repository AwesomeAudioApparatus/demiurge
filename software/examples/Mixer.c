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
#include "demiurge-hardware.h"

static audio_inport_t input1;      // Declaration of Audio Input Ports
static audio_inport_t input2;
static audio_inport_t input3;
static audio_inport_t input4;

static potentiometer_t pot1;    // Declaration of Potentiometers
static potentiometer_t pot2;    // Declaration of Potentiometers
static potentiometer_t pot3;    // Declaration of Potentiometers
static potentiometer_t pot4;    // Declaration of Potentiometers

static audio_outport_t out1;    // Declaration of Audio Output Ports
static audio_outport_t out2;

static mixer_t mixer;

/*
 * 4 channel mixer, no CV controls.
 */
void mixer_prepare() {
    demiurge_samplerate = 48000;     // 48000 samples/second

    demiurge_set_inport_audio(1);
    demiurge_set_inport_audio(2);
    demiurge_set_inport_audio(3);
    demiurge_set_inport_audio(4);

    demiurge_set_potentiometer(1, -1.0f, 1.0f);
    demiurge_set_potentiometer(1, -1.0f, 1.0f);
    demiurge_set_potentiometer(1, -1.0f, 1.0f);
    demiurge_set_potentiometer(1, -1.0f, 1.0f);
}

void mixer_setup() {

   audio_inport_init(&input1, 1);   // Audio In on first input
   audio_inport_init(&input1, 2);   // Audio In on second input
   audio_inport_init(&input1, 3);   // Audio In on third input
   audio_inport_init(&input1, 4);   // Audio In on fourth input

   potentiometer_init(&pot1, 1);
   potentiometer_init(&pot1, 2);
   potentiometer_init(&pot1, 3);
   potentiometer_init(&pot1, 4);

   audio_outport_init(&out1, 1);    // Audio Out on left output channel
   audio_outport_init(&out2, 2);    // Audio Out on right output channel

   mixer_init(&mixer,  4);           // 4 channels in mixer

   mixer_configure_input(&mixer, 1, &input1.me, &pot1.me);
   mixer_configure_input(&mixer, 2, &input2.me, &pot2.me);
   mixer_configure_input(&mixer, 3, &input3.me, &pot3.me);
   mixer_configure_input(&mixer, 4, &input4.me, &pot4.me);

   // Connect mixer output to out1 & out2
   audio_outport_configure_input(&out1, &mixer.me);
   audio_outport_configure_input(&out2, &mixer.me);
}

void mixer_loop() {
}

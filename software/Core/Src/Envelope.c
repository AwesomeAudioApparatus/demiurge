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

static control_pair_t attack;
static control_pair_t decay;
static control_pair_t sustain;
static control_pair_t release;

static adsr_t adsr;
static gate_inport_t gate;
static inverter_t invert;

static cv_outport_t out1;
static cv_outport_t out2;

/*
 * Envelope generator.
 */
void envelope_setup() {
   control_pair_init(&attack, 1);
   control_pair_init(&decay, 2);
   control_pair_init(&sustain, 3);
   control_pair_init(&release, 4);
   adsr_init(&adsr);
   gate_inport_init(&gate, 1);
   inverter_init(&invert);
   cv_outport_init(&out1, 1);
   cv_outport_init(&out2, 2);

   adsr_configure_trig(&adsr, &gate.me);
   adsr_configure_gate(&adsr, &gate.me);
   adsr_configure_attack(&adsr, &attack.me);
   adsr_configure_decay(&adsr, &decay.me);
   adsr_configure_sustain(&adsr, &sustain.me);
   adsr_configure_release(&adsr, &release.me);

   inverter_configure_input(&invert, &adsr.me);
   cv_outport_configure_input(&out1, &invert.me);
   cv_outport_configure_input(&out2, &adsr.me);
}

void envelope_loop() {

}

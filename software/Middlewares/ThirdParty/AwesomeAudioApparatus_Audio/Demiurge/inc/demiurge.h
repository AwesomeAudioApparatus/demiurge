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

#ifndef _DEMIURGE_DEMIURGE_H_
#define _DEMIURGE_DEMIURGE_H_

#include "demiurge-hardware.h"

#include "adsr.h"
#include "audio_inport.h"
#include "audio_outport.h"
#include "averager.h"
#include "button.h"
#include "calculator.h"
#include "clipping.h"
#include "control_pair.h"
#include "cv_inport.h"
#include "cv_outport.h"
#include "demi_asserts.h"
#include "fixed_signal.h"
#include "gate_inport.h"
#include "gate_outport.h"
#include "inverter.h"
#include "led.h"
#include "lg.h"
#include "lg10.h"
#include "lg2.h"
#include "mixer.h"
#include "octave_per_volt.h"
#include "offset.h"
#include "oscillator.h"
#include "pan.h"
#include "passthru.h"
#include "potentiometer.h"
#include "pushbutton.h"
#include "rgb.h"
#include "scale.h"
#include "signal.h"
#include "threshold.h"
#include "volume.h"

extern uint32_t micros_per_tick;
extern uint64_t demiurge_current_time;
extern float outputs[DEMIURGE_NUM_OUTPUTS];
extern float inputs[DEMIURGE_NUM_INPUTS];
extern bool buttons[DEMIURGE_NUM_BUTTONS];

extern bool gates_in[DEMIURGE_NUM_GATES];
extern bool gates_out[DEMIURGE_NUM_GATES];

/* User application interface */
void setup();

void loop();

void set_gate_to_input(int position);

void set_gate_to_output(int position);

void demiurge_init();

void demiurge_start();

void demiurge_stop();

void demiurge_registerSink(signal_t *processor);

void demiurge_unregisterSink(signal_t *processor);

#endif

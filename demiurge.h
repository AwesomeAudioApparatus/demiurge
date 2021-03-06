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

#ifndef _DEMIURGE_DEMIURGE_H_
#define _DEMIURGE_DEMIURGE_H_

// Demiurge Core
#include "signal.h"
#include "demi_asserts.h"
#include "clipping.h"
#include "octave_per_volt.h"

// Demiurge Processing Blocks
#include "adsr.h"
#include "audio_inport.h"
#include "audio_outport.h"
#include "averager.h"
#include "calculator.h"
#include "control_pair.h"
#include "cv_inport.h"
#include "cv_outport.h"
#include "fixed_signal.h"
#include "gate_inport.h"
#include "gate_outport.h"
#include "inverter.h"
#include "led.h"
#include "lg.h"
#include "lg2.h"
#include "lg10.h"
#include "mixer.h"
#include "scale.h"
#include "offset.h"
#include "oscillator.h"
#include "pan.h"
#include "passthru.h"
#include "potentiometer.h"
#include "pushbutton.h"
#include "threshold.h"
#include "volume.h"

#ifndef DEMIURGE_MAX_SINKS
#define DEMIURGE_MAX_SINKS 6
#endif

uint64_t demiurge_current_time();

void demiurge_start(uint64_t sample_rate);

void demiurge_registerSink(signal_t *processor);

void demiurge_unregisterSink(signal_t *processor);

float demiurge_input(int number);

float demiurge_output(int number);

void demiurge_set_output(int number, float value);

bool demiurge_gpio(int i);

void demiurge_print_overview(const char *tag, signal_t *signal);

#endif

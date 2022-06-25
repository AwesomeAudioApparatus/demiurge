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

#ifndef _DEMIURGE_ADSR_H_
#define _DEMIURGE_ADSR_H_

#include "signal.h"
#include "threshold.h"

typedef struct {
   signal_t me;
   signal_t *attack;
   signal_t *decay;
   signal_t *sustain;
   signal_t *release;
   signal_t *gate;
   signal_t *trig;
   int stateMachine;
   int64_t doneAt;
   int64_t startedAt;
   bool currentTrig;
   bool currentGate;
   threshold_t gateThreshold;
   threshold_t trigThreshold;
} adsr_t;

void adsr_init(adsr_t *handle);
void adsr_configure_attack(adsr_t *handle, signal_t *control);
void adsr_configure_decay(adsr_t *handle, signal_t *control);
void adsr_configure_sustain(adsr_t *handle, signal_t *control);
void adsr_configure_release(adsr_t *handle, signal_t *control);
void adsr_configure_gate(adsr_t *handle, signal_t *control);
void adsr_configure_trig(adsr_t *handle, signal_t *control);
float adsr_read(signal_t *handle, uint64_t time);

#endif

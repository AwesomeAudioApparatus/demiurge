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
#include "signal.h"
#include "adsr.h"
#include "clipping.h"

void adsr_init(adsr_t *handle) {
   handle->me.read_fn = adsr_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
   handle->stateMachine = 0;
   handle->doneAt = 0;
   handle->startedAt = 0;
   threshold_init(&handle->trigThreshold,0x2000, 0x800);
   threshold_init(&handle->gateThreshold,0x2000, 0x800);
}

void adsr_configure_gate(adsr_t *handle, signal_t *control) {
   handle->gate = control;
}

void adsr_configure_attack(adsr_t *handle, signal_t *control) {
   handle->attack = control;
}

void adsr_configure_decay(adsr_t *handle, signal_t *control) {
   handle->decay = control;
}

void adsr_configure_sustain(adsr_t *handle, signal_t *control) {
   handle->sustain = control;
}

void adsr_configure_release(adsr_t *handle, signal_t *control) {
   handle->release = control;
}

void adsr_configure_trig(adsr_t *handle, signal_t *control) {
   handle->trig = control;
}

float adsr_slopeTime(float voltage) {
   // Logarithmic response, so that;
   // -10V = 1 microsecond, 0V = 1 millisecond, +10V = 1 second
   // TODO:
   float millis = powf(10, voltage / 3.33333f);
   return millis * 1000;
}

// TODO: without floating point, this code becomes VERY different, so let's postpone that/
float adsr_read(signal_t *handle, uint64_t time) {
   adsr_t *adsr = (adsr_t *) handle->data;
   if (time > handle->last_calc) {
      handle->last_calc = time;
      float output;

      bool trigIn = threshold_compute(&adsr->trigThreshold, adsr->trig->read_fn(adsr->trig, time));
      bool gateIn = threshold_compute(&adsr->gateThreshold, adsr->gate->read_fn(adsr->gate, time));
#ifdef DEMIURGE_DEV
      handle->extra1 = trigIn;
      handle->extra2 = gateIn;
#endif
      if (!adsr->currentTrig && trigIn) {
         // RISE
         adsr->stateMachine = 1;
         adsr->startedAt = time;
      }
      adsr->currentTrig = trigIn;

      if (adsr->currentGate) {
         // Ongoing
         switch (adsr->stateMachine) {
            case 1:  // Attack
            {
               float attackIn = adsr->attack->read_fn(adsr->attack, time);
               float k = 20.0 / adsr_slopeTime(attackIn);
               float m = -10.0;
               output = (time - adsr->startedAt) * k + m;
               if (output >= 10.0) {
                  adsr->startedAt = time;
                  adsr->stateMachine++;
               }
               break;
            }
            case 2:  // Decay
            {
               float sustainIn = adsr->sustain->read_fn(adsr->sustain, time);
               float decayIn = adsr->decay->read_fn(adsr->decay, time);
               float k = -(10 - sustainIn) / adsr_slopeTime(decayIn);
               float m = 10;
               output = (time - adsr->startedAt) * k + m;
               if (output >= 10.0) {
                  adsr->startedAt = time;
                  adsr->stateMachine++;
               }
               break;
            }
            case 3:  // Sustain
            {
               output = adsr->sustain->read_fn(adsr->sustain, time);
               break;
            }
            default:
               output = -10;
         }
      } else {
         float releaseIn = adsr->release->read_fn(adsr->release, time);
         float sustainIn = adsr->sustain->read_fn(adsr->sustain, time);
         float k = -sustainIn / adsr_slopeTime(releaseIn);
         float m = sustainIn;
         output = (time - adsr->startedAt) * k + m;
         if (gateIn) {
            // Start new cycle
            adsr->stateMachine = 0;
         }
      }
      adsr->currentGate = gateIn;

      output = handle->post_fn(output);
      handle->cached = output;
#ifdef DEMIURGE_DEV
      handle->extra4 = adsr->stateMachine;
#endif
      return output;
   }
   return handle->cached;
}


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

#include "clipping.h"
#include "gate_inport.h"
#include "demiurge.h"

void gate_inport_init(gate_inport_t *handle, int position) {
   configASSERT(position >= 0 && position <= 4)
   handle->me.read_fn = gate_inport_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_gate;
   handle->position = position - 1;
}

float gate_inport_read(signal_t *handle, uint64_t time) {
   if (time > handle->last_calc) {
      handle->last_calc = time;
      gate_inport_t *port = (gate_inport_t *) handle->data;
#ifdef DEMIURGE_DEV
      handle->extra1 = port->position+1;
#endif
      // if position == 0, then use digital input, otherwise use analog inputs.
      float result;
      if (port->position) {
         float input = inputs[port->position];
         result = handle->post_fn(input);
      } else {
         bool state = demiurge_gpio(32);
         result = state ? DEMIURGE_GATE_HIGH : DEMIURGE_GATE_LOW;
      }
#ifdef DEMIURGE_DEV
      handle->extra2 = result;
#endif
      handle->cached = result;
   }
   return handle->cached;
}

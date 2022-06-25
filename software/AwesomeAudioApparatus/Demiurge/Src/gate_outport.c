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

#include "gate_outport.h"
#include "clipping.h"
#include "demiurge.h"

void gate_outport_init(gate_outport_t *handle, int position) {
   configASSERT(position > 0 && position <= 2)
   handle->me.read_fn = gate_outport_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_gate;
   handle->position = position;
   handle->registered = false;
}

void gate_outport_configure_input(gate_outport_t *handle, signal_t *input) {
   handle->input = input;
   if (!handle->registered){
      demiurge_registerSink(&handle->me);
      handle->registered = true;
   }
}

float gate_outport_read(signal_t *handle, uint64_t time) {
   if( time > handle->last_calc ) {
      gate_outport_t *port = (gate_outport_t *) handle->data;
      handle->last_calc = time;
      signal_t *upstream = port->input;
      signal_fn fn = upstream->read_fn;
      float result = handle->post_fn(fn(upstream, time));
      handle->cached = result;
      return result;
   }
   return handle->cached;
}


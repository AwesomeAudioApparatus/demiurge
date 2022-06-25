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

#include "inverter.h"
#include "clipping.h"


void inverter_init(inverter_t *handle) {
   handle->me.read_fn = inverter_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
   handle->midpoint = NULL;
   handle->input = NULL;
   handle->scale = NULL;
}

void inverter_configure_input(inverter_t *handle, signal_t *input) {
   handle->input = input;
}

void inverter_configure_midpoint(inverter_t *handle, signal_t *midpoint) {
   handle->midpoint = midpoint;
}

void inverter_configure_scale(inverter_t *handle, signal_t *scale) {
   handle->scale = scale;
}

float inverter_read(signal_t *handle, uint64_t time) {
   if (time > handle->last_calc) {
      handle->last_calc = time;
      inverter_t *inverter = (inverter_t *) handle->data;
      float out;
      signal_t *midpointSignal = inverter->midpoint;
      signal_t *scaleSignal = inverter->scale;
      signal_t *input = inverter->input;
      if (midpointSignal == NULL) {
         out = 0 - input->read_fn(input, time);
      } else {
         float midpoint = midpointSignal->read_fn(midpointSignal, time);
         out = midpoint - input->read_fn(input, time);
      }
      if (scaleSignal != NULL) {
         out = out * scaleSignal->read_fn(scaleSignal, time);
      }
      out = handle->post_fn(out);
      handle->cached = out;
      return out;
   }
   return handle->cached;
}

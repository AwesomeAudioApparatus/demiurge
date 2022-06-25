/*
  Copyright 2020, Awesome Audio Apparatus.

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

#include <math.h>
#include "lg.h"
#include "signal.h"
#include "clipping.h"

void lg_init(lg_t *handle) {
   handle->me.read_fn = lg_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
}

void lg_configure_input(lg_t *handle, signal_t *input) {
   handle->input = input;
}

float lg_read(signal_t *handle, uint64_t time) {
   if (time > handle->last_calc) {
      handle->last_calc = time;
      lg_t *lg = (lg_t *) handle->data;
      float input = lg->input->read_fn(lg->input, time);
      float new_output = handle->post_fn(logf(input));
      handle->cached = new_output;
      return new_output;
   }
   return handle->cached;
}

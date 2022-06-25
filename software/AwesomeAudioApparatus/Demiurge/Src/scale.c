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

#include "clipping.h"
#include "scale.h"
#include "signal.h"

void scale_init(scale_t *handle) {
   handle->me.read_fn = scale_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
   handle->scale = 1.0f;
   handle->scale_control = NULL;
}

void scale_configure(scale_t *handle, signal_t *input, signal_t *control) {
   handle->input = input;
   handle->scale_control = control;
}

void scale_configure_input(scale_t *handle, signal_t *input) {
   handle->input = input;
}

void scale_configure_control(scale_t *handle, signal_t *control) {
   handle->scale_control = control;
}

float scale_read(signal_t *handle, uint64_t time) {
   if (time > handle->last_calc) {
      handle->last_calc = time;
      scale_t *scale = (scale_t *) handle->data;
      float input = scale->input->read_fn(scale->input, time);
      float new_output;

      if (scale->scale_control != NULL) {
         float new_scale = handle->read_fn(handle, time);
         new_output = input * new_scale;
      } else {
         new_output = input * scale->scale;
      }
      new_output = handle->post_fn(new_output);
      handle->cached = new_output;
      return new_output;
   }
   return handle->cached;
}

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

#include "offset.h"
#include "clipping.h"
#include "signal.h"


void offset_init(offset_t *handle) {
   handle->me.read_fn = offset_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
   handle->offset = 0;
   handle->offset_control = NULL;
   handle->input = NULL;
}

void offset_configure_input(offset_t *handle, signal_t *input) {
   handle->input = input;
}

void offset_configure_control(offset_t *handle, signal_t *control) {
   handle->offset_control = control;
}

void offset_configure(offset_t *handle, signal_t *input, signal_t *offset_control) {
   handle->input = input;
   handle->offset_control = offset_control;
}

float offset_read(signal_t *handle, uint64_t time){
   if (time > handle->last_calc) {
      handle->last_calc = time;
      offset_t *offset = (offset_t *) handle->data;

      signal_t *input = offset->input;
      float input_value = input->read_fn(input, time);

      float  new_output;
      if (offset->offset_control != NULL) {
         signal_t *ctrl = offset->input;
         float new_offset = ctrl->read_fn(ctrl, time);
#ifdef DEMIURGE_DEV
         handle->extra1 = input_value;
         handle->extra2 = new_offset;
#endif
         new_output = handle->post_fn(input_value + new_offset);
      } else {
         new_output = handle->post_fn(input_value + offset->offset);
      }
      handle->cached = new_output;
      return new_output;
   }
   return handle->cached;
}

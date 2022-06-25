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
#include "demi_asserts.h"
#include "volume.h"

void volume_init(volume_t *handle) {
   handle->me.read_fn = volume_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
}

void volume_configure(volume_t *handle, signal_t *input, signal_t *control) {
   configASSERT(input != NULL && control != NULL)
   handle->input = input;
   handle->control = control;
}

void volume_configure_input(volume_t *handle, signal_t *input) {
   configASSERT(input != NULL)
   handle->input = input;
}

void volume_configure_control(volume_t *handle, signal_t *control) {
   configASSERT(control != NULL)
   handle->control = control;
}

float volume_read(signal_t *handle, uint64_t time) {
   if (time > handle->last_calc) {
      handle->last_calc = time;
      volume_t *vol = (volume_t *) handle->data;
      signal_t *ctrl = vol->control;
      float gain = ctrl->read_fn(ctrl, time);
      signal_t *input = vol->input;
      float in = input->read_fn(input, time);
      float factor = 0.05 * gain + 0.5;
      float result = handle->post_fn(in * factor);
      handle->cached = result;
      return result;
   }
   return handle->cached;
}

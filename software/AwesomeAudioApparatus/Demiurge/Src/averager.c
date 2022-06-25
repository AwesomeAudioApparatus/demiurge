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

#include "averager.h"
#include "signal.h"
#include "clipping.h"


void averager_init(averager_t *handle) {
   handle->me.read_fn = averager_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
   handle->update = 0.1;
   handle->keep = 0.9;
   handle->averaging_control = NULL;
}

void averager_configure_input(averager_t *handle, signal_t *input) {
   handle->input = input;
}
void averager_configure_avg(averager_t *handle, signal_t *avg_control) {
   handle->averaging_control = avg_control;
}

void averager_set_keep(averager_t *handle, float new_keep) {
   handle->keep = new_keep;
   handle->update = 1 - new_keep;
}

void averager_set_update(averager_t *handle,float new_update) {
   handle->keep = 1 - new_update;
   handle->update = new_update;
}

float averager_read(signal_t *handle, uint64_t time){
   if (time > handle->last_calc) {
      handle->last_calc = time;
      averager_t *averager = (averager_t *) handle->data;
      float input = averager->input->read_fn(averager->input, time);

      float old_avg = averager->rolling_avg;
      float new_output;

      if (averager->averaging_control != NULL) {
         float keep = handle->read_fn(handle, time) / 20 + 10;
         new_output = old_avg * keep + input * (1-keep);
      } else {
         new_output = old_avg * averager->keep + old_avg * averager->update;
      }
      new_output = handle->post_fn(new_output);
      averager->rolling_avg = new_output;
      handle->cached = new_output;
      return new_output;
   }
   return handle->cached;
}

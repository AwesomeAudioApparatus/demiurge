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

#include "control_pair.h"
#include "clipping.h"
#include "demi_asserts.h"

void control_pair_init(control_pair_t *handle, int position) {
   configASSERT(position > 0 && position <= 4)
   handle->me.read_fn = control_pair_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_cv;
   potentiometer_init(&handle->potentiometer, position);
   cv_inport_init(&handle->cv, position);
}

float control_pair_read(signal_t *handle, uint64_t time) {
   control_pair_t *control = (control_pair_t *) handle->data;
   if (time > handle->last_calc) {
      handle->last_calc = time;

      signal_t *pot = &control->potentiometer.me;
      float pot_in = pot->read_fn(pot, time);

      signal_t *cv = &control->cv.me;
      float cv_in = cv->read_fn(cv, time);
      float result = handle->post_fn((pot_in + cv_in) / 2.0f);
      handle->cached = result;
      return result;
   }
   return handle->cached;
}

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

#include "demiurge.h"
#include "cv_inport.h"
#include "clipping.h"


void cv_inport_init(cv_inport_t *handle, int position){
   configASSERT(position > 0 && position <= 4)
   handle->me.read_fn = cv_inport_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
   handle->position = position + DEMIURGE_CVINPUT_OFFSET - 1;
}

float cv_inport_read(signal_t *handle, uint64_t time) {
   cv_inport_t *cv = (cv_inport_t *) handle->data;
   if (time > handle->last_calc) {
      handle->last_calc = time;
      float in = inputs[cv->position];
      float result = handle->post_fn(in);
      handle->cached = result;
      return result;
   }
   return handle->cached;
}

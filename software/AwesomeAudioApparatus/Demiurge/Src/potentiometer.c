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

#include "potentiometer.h"
#include "demiurge.h"


void potentiometer_init(potentiometer_t *handle, int position) {
   configASSERT(position > 0 && position <= 4 )
   handle->me.read_fn = potentiometer_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
   handle->position = position + DEMIURGE_POTENTIOMETER_OFFSET - 1;
#ifdef DEMIURGE_DEV
   handle->me.extra8 = handle->position;
#endif
}

float potentiometer_read(signal_t *signal, uint64_t time) {
   if (time > signal->last_calc) {
      signal->last_calc = time;
      potentiometer_t *handle = (potentiometer_t *) signal->data;
#ifdef DEMIURGE_DEV
      configASSERT(handle->position > 0 && handle->position <= 8)
#endif
      float in = inputs[handle->position];
#ifdef DEMIURGE_DEV
      signal->extra1 = in;
      signal->extra2 = handle->position;
#endif
      float result = signal->post_fn(in);
      signal->cached = result;
      return result;
   }
   return signal->cached;
}

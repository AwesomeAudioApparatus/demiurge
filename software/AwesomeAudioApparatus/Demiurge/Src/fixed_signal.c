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
#include "fixed_signal.h"

void fixed_signal_init(fixed_signal_t *handle, float value) {
   handle->me.read_fn = fixed_signal_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
#ifdef DEMIURGE_DEV
   handle->me.extra1 = value;
#endif
}

float fixed_signal_read(signal_t *handle, uint64_t time) {
   return handle->post_fn(handle->extra1);
}

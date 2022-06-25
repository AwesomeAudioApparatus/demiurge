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

#include "led.h"
#include "demiurge.h"

void led_init(led_t *handle, int position) {
   configASSERT(position > 0 && position <= 4)
   handle->me.read_fn = led_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
   handle->position = position - 1;
   handle->registered = false;
   handle->input = NULL;
}

void led_configure_input(led_t *handle, signal_t *input) {
   handle->input = input;
   if (!handle->registered) {
      demiurge_registerSink(&handle->me);
      handle->registered = true;
   }
}

void led_release(led_t *handle) {
   if (!handle->registered) {
      demiurge_unregisterSink(&handle->me);
      handle->registered = false;
   }
}

float led_read(signal_t *handle, uint64_t time) {
   if (time > handle->last_calc) {
      handle->last_calc = time;
      led_t *led = (led_t *) handle->data;
      signal_t *upstream = led->input;
      float out = upstream->read_fn(upstream, time);
      float result = handle->post_fn(out);
      leds[led->position] = result;
      handle->cached = result;
      return 0.0f;
   }
   return 0.0f;
}

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

#include "audio_outport.h"
#include "clipping.h"
#include "demiurge.h"

void audio_outport_init(audio_outport_t *handle, int position) {
   configASSERT(position > 0 && position <= 2)
   handle->me.read_fn = audio_outport_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_audio;
   handle->position = position - 1;
   handle->registered = false;
}

void audio_outport_configure_input(audio_outport_t *handle, signal_t *input) {
   if (!handle->registered) {
      handle->input = input;
      demiurge_registerSink(&handle->me);
      handle->registered = true;
   }
}

float audio_outport_read(signal_t *handle, uint64_t time) {
   if (time > handle->last_calc) {
      handle->last_calc = time;
      audio_outport_t *port = (audio_outport_t *) handle->data;
      signal_t *upstream = port->input;
      signal_fn fn = upstream->read_fn;
      float raw = fn(upstream, time);
      float result = handle->post_fn(raw);
#ifdef DEMIURGE_DEV
      handle->extra1 = raw;
      handle->extra2 = result;
#endif
      outputs[port->position] = result;
      return 0.0f;
   }
   return 0.0f;
}


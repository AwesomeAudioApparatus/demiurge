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

#include <malloc.h>
#include "clipping.h"
#include "demi_asserts.h"
#include "mixer.h"
#include "signal.h"


void mixer_init( mixer_t *handle, int channels) {
   handle->me.read_fn = mixer_read;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
   handle->channels = channels;
   handle->inputs = (signal_t **) calloc(channels, sizeof(signal_t *));
   handle->volumes = (volume_t **) calloc(channels, sizeof(volume_t *));
   for( int i=0; i < channels; i++ ){
      handle->inputs[i] = NULL;
      handle->volumes[i] = NULL;
   }
   
}
void mixer_configure_input(mixer_t *handle, int number, signal_t *source, signal_t *control) {
   int index = number - 1;
   configASSERT(handle->inputs[index] == NULL )
   volume_t *v = (volume_t *) malloc(sizeof(volume_t));
   handle->volumes[index] = v;
   volume_init(handle->volumes[index]);
   handle->inputs[index] = &v->me;
   volume_configure(v, source, control);
}

float mixer_read(signal_t *handle, uint64_t time) {
   if (time > handle->last_calc) {
      handle->last_calc = time;
      mixer_t *mixer = (mixer_t *) handle->data;
      float output = 0;
      int counter = 0;
      for (int i=0; i < mixer->channels; i++) {
         signal_t *inp = mixer->inputs[i];
         if (inp != NULL) {
            output = output + inp->read_fn(inp, time);
            counter++;
         }
      }
      output = handle->post_fn(output / counter);
      handle->cached = output;
      return output;
   }
   return handle->cached;
}

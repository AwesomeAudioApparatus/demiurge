/*
  Copyright 2019-2022, Awesome Audio Apparatus.

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

#include "audio_inport.h"

#include "clipping.h"
#include "demiurge.h"


void audio_inport_init(audio_inport_t *handle, int position)
{
    configASSERT(position > 0 && position <= DEMIURGE_NUM_AUDIOINPUTS)
    handle->me.read_fn = audio_inport_read;
    handle->me.data = handle;
#ifdef DEMIURGE_POST_FUNCTION
    handle->me.post_fn = clip_audio;
#endif
    handle->position = position - 1 + DEMIURGE_AUDIOINPUT_OFFSET;
}

float audio_inport_read(signal_t *handle, uint64_t time)
{
    audio_inport_t *port = (audio_inport_t *) handle->data;
    if (time > handle->last_calc)
    {
        handle->last_calc = time;
        float result = inputs[port->position];
#ifdef DEMIURGE_POST_FUNCTION
        float result = handle->post_fn(result);
#endif
        handle->cached = result;
        return result;
    }
    return handle->cached;
}

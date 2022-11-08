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

#include "potentiometer.h"

#include "demiurge.h"

void potentiometer_init(potentiometer_t *handle, int position)
{
    configASSERT(position > 0 && position <= DEMIURGE_NUM_POTS)
    handle->me.read_fn = potentiometer_read;
    handle->me.data = handle;
#ifdef DEMIURGE_POST_FUNCTION
    handle->me.post_fn = clip_none;
#endif
    handle->input = &inputs[position + DEMIURGE_POTENTIOMETER_OFFSET - 1];
}

float potentiometer_read(signal_t *signal, uint64_t time)
{
    if (time > signal->last_calc)
    {
        signal->last_calc = time;
        potentiometer_t *handle = (potentiometer_t *) signal->data;
        float in = *handle->input;
#ifdef DEMIURGE_DEV
        signal->extra1 = in;
#endif
#ifdef DEMIURGE_POST_FUNCTION
        in = signal->post_fn(in);
#endif
        signal->cached = in;
        return in;
    }
    return signal->cached;
}

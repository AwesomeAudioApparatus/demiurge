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

#include "pushbutton.h"

#include "demiurge.h"

void pushbutton_init(pushbutton_t *handle, int position)
{
    configASSERT(position > 0 && position <= DEMIURGE_NUM_BUTTONS)
    handle->me.read_fn = pushbutton_read;
    handle->me.data = handle;
#ifdef DEMIURGE_POST_FUNCTION
    handle->me.post_fn = clip_gate;
#endif
    handle->position = position - 1;
}

float pushbutton_read(signal_t *handle, uint64_t time)
{
    if (time > handle->last_calc)
    {
        pushbutton_t *button = (pushbutton_t *) handle->data;
        float result = buttons[button->position];
#ifdef DEMIURGE_POST_FUNCTION
        result = handle->post_fn(result);
#endif
        handle->cached = result;
        return result;
    }
    return handle->cached;
}

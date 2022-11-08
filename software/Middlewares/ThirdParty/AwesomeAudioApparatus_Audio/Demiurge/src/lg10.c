/*
  Copyright 2020, Awesome Audio Apparatus.

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

#include "lg10.h"

#include <math.h>

#include "clipping.h"

void lg10_init(lg10_t *handle)
{
    handle->me.read_fn = lg10_read;
    handle->me.data = handle;
#ifdef DEMIURGE_POST_FUNCTION
    handle->me.post_fn = clip_none;
#endif
}

void lg10_configure_input(lg10_t *handle, signal_t *input)
{
    handle->input = input;
}

float lg10_read(signal_t *handle, uint64_t time)
{
    if (time > handle->last_calc)
    {
        handle->last_calc = time;
        lg10_t *lg10 = (lg10_t *) handle->data;
        float input = lg10->input->read_fn(lg10->input, time);
        float new_output = log10f(input);
#ifdef DEMIURGE_POST_FUNCTION
        new_output = handle->post_fn(new_output);
#endif

        handle->cached = new_output;
        return new_output;
    }
    return handle->cached;
}

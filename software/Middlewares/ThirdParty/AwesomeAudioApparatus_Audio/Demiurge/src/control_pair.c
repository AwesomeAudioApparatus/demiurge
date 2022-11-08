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

#include "control_pair.h"

#include "clipping.h"
#include "demiurge.h"
#include "demi_asserts.h"

void control_pair_init(control_pair_t *handle, int position)
{
    configASSERT(position > 0 && position <= DEMIURGE_NUM_CVINPUTS)
    handle->me.read_fn = control_pair_read;
    handle->me.data = handle;
#ifdef DEMIURGE_POST_FUNCTION
    handle->me.post_fn = clip_cv;
#endif
    handle->cv_input = &inputs[position - 1 + DEMIURGE_CVINPUT_OFFSET];
    handle->pot_input = &inputs[position - 1 + DEMIURGE_POTENTIOMETER_OFFSET];
}

float control_pair_read(signal_t *handle, uint64_t time)
{
    control_pair_t *control = (control_pair_t *) handle->data;
    if (time > handle->last_calc)
    {
        handle->last_calc = time;
        float cv_in = *control->cv_input;
        float pot_in = *control->pot_input;
#ifdef DEMIURGE_POST_FUNCTION
        float result = handle->post_fn((pot_in + cv_in));
#else
        float result = pot_in + cv_in;
#endif
        handle->cached = result;
        return result;
    }
    return handle->cached;
}

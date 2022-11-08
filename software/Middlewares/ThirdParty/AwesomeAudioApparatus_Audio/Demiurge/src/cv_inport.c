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

#include "cv_inport.h"

#include "clipping.h"
#include "demiurge.h"


void cv_inport_init(cv_inport_t *handle, int position)
{
    configASSERT(position > 0 && position <= DEMIURGE_NUM_CVINPUTS)
    handle->me.read_fn = cv_inport_read;
    handle->me.data = handle;
#ifdef DEMIURGE_POST_FUNCTION
    handle->me.post_fn = clip_none;
#endif
    handle->input = &inputs[position - 1 + DEMIURGE_CVINPUT_OFFSET];
}

float cv_inport_read(signal_t *handle, uint64_t time)
{
    cv_inport_t *cv = (cv_inport_t *) handle->data;
#ifdef DEMIURGE_POST_FUNCTION
    if (time > handle->last_calc) {
       handle->last_calc = time;
       float result = cv->input;
       result = handle->post_fn(result);
       handle->cached = result;
       return result;
    }
    return handle->cached;
#endif
    return *cv->input;
}

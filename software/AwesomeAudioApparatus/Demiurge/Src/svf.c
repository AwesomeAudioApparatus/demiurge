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

#include "svf.h"
#include "clipping.h"

// TODO: Work in-progress. Not correctly implemented yet. Need to figure out sequence/ordering in diagram at;
// https://www.musicdsp.org/en/latest/Filters/92-state-variable-filter-double-sampled-stable.html

void svf_init(svf_t *handle) {
    handle->me.read_fn = svf_read;
    handle->me.data = handle;
    handle->me.post_fn = clip_none;
    handle->frequency = NULL;
    handle->input = NULL;
    handle->Q = NULL;
}

void svf_configure_input(svf_t *handle, signal_t *input) {
    handle->input = input;
}

void svf_configure_frequency(svf_t *handle, signal_t *freq) {
    handle->frequency = freq;
}

void svf_configure_Q(svf_t *handle, signal_t *Q) {
    handle->Q = Q;
}
float svf_read(signal_t *handle, uint64_t time) {
    if (time > handle->last_calc) {
        handle->last_calc = time;
        svf_t *svf = (svf_t *) handle->data;
        signal_t *frequencySignal = svf->frequency;
        signal_t *QSignal = svf->Q;
        signal_t *input = svf->input;

        float lastOut = handle->cached;
        float freq = frequencySignal->read_fn(frequencySignal, time);
        float f = 2 * arm_sin_f32(PI * freq/demiurge_samplerate);
        float q = 1 / QSignal->read_fn(QSignal, time);
        float inp = input->read_fn(input, time);
        float q2 = svf->q2;
        float q1 =  q2 * q;
        float highpass = inp + lastOut + q1;
        float bandpass = highpass * f + q2;

        float out = q2 * f + svf->secondLastOut;
        float notch = out + highpass;
        svf->secondLastOut = lastOut;
        out = handle->post_fn(out);

        svf->q2 = bandpass;
        handle->cached = out;
        return out;
    }
    return handle->cached;
}

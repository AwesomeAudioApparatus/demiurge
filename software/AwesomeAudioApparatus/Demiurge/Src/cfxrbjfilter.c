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

#include "cfxrbjfilter.h"

// TODO: Work in-progress. Function check and testing are not done.

void cfxrbjfilter_init(cfxrbjfilter_t *handle, int position) {
//    configASSERT(position > 0 && position <= 4)
    handle->me.read_fn = cfxrbjfilter_read;
    handle->me.data = handle;
    handle->me.post_fn = clip_none;
    handle->input = NULL;
    handle->a1a0 = 0;
    handle->a2a0 = 0;
    handle->b0a0 = 0;
    handle->b1a0 = 0;
    handle->b2a0 = 0;
    handle->ou1 = 0;
    handle->ou2 = 0;
    handle->in1 = 0;
    handle->in2 = 0;
    handle->q_is_bandwidth = false;
}

void cfxrbjfilter_configure_input(cfxrbjfilter_t *handle, signal_t *input) {
    handle->input = input;
}

void cfxrbjfilter_compute(signal_t *handle, uint64_t time) {
    if (time > handle->last_calc) {
        handle->last_calc = time;
        cfxrbjfilter_t *filter = ((cfxrbjfilter_t *) handle->data);
        signal_t *input = filter->input;
        signal_t *typesignal = filter->type;
        int const type = (int) filter->type->read_fn(filter->type, time);
        double const frequency = filter->frequency->read_fn(filter->frequency, time);
        double const q = filter->Q->read_fn(filter->Q, time);
        double const db_gain = filter->gain->read_fn(filter->gain, time);
        bool q_is_bandwidth = filter->q_is_bandwidth;
        if (type != filter->oldtype) {
            computeCoefficients(filter, type, frequency, q, db_gain, q_is_bandwidth);
        }
        float in = input->read_fn(input, time);
        float const yn = filter->b0a0 * in + filter->b1a0 * filter->in1 + filter->b2a0 * filter->in2 -
                         filter->a1a0 * filter->ou1 - filter->a2a0 * filter->ou2;

        // push in/out buffers
        filter->in2 = filter->in1;
        filter->in1 = in;
        filter->ou2 = filter->ou1;
        filter->ou1 = yn;
    }
}

void computeCoefficients(cfxrbjfilter_t *filter, uint64_t type, const double frequency, const double q,
                         const double db_gain, bool q_is_bandwidth) {
    // temp pi
    double const temp_pi = 3.1415926535897932384626433832795;

    // temp coef vars
    double alpha, a0, a1, a2, b0, b1, b2;

    // peaking, lowshelf and hishelf
    if (type >= 6) {
        double const A = pow(10.0, (db_gain / 40.0));
        double const omega = 2.0 * temp_pi * frequency / demiurge_samplerate;
        double const tsin = sin(omega);
        double const tcos = cos(omega);

        if (q_is_bandwidth)
            alpha = tsin * sinh(log(2.0) / 2.0 * q * omega / tsin);
        else
            alpha = tsin / (2.0 * q);

        double const beta = sqrt(A) / q;

        // peaking
        if (type == 6) {
            b0 = 1.0 + alpha * A;
            b1 = -2.0 * tcos;
            b2 = 1.0 - alpha * A;
            a0 = 1.0 + alpha / A;
            a1 = -2.0 * tcos;
            a2 = 1.0 - alpha / A;
        }

        // lowshelf
        if (type == 7) {
            b0 = A * ((A + 1.0) - (A - 1.0) * tcos + beta * tsin);
            b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * tcos);
            b2 = A * ((A + 1.0) - (A - 1.0) * tcos - beta * tsin);
            a0 = (A + 1.0) + (A - 1.0) * tcos + beta * tsin;
            a1 = -2.0 * ((A - 1.0) + (A + 1.0) * tcos);
            a2 = (A + 1.0) + (A - 1.0) * tcos - beta * tsin;
        }

        // hishelf
        if (type == 8) {
            b0 = A * ((A + 1.0) + (A - 1.0) * tcos + beta * tsin);
            b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * tcos);
            b2 = A * ((A + 1.0) + (A - 1.0) * tcos - beta * tsin);
            a0 = (A + 1.0) - (A - 1.0) * tcos + beta * tsin;
            a1 = 2.0 * ((A - 1.0) - (A + 1.0) * tcos);
            a2 = (A + 1.0) - (A - 1.0) * tcos - beta * tsin;
        }
    } else {
        // other filters
        double const omega = 2.0 * temp_pi * frequency / demiurge_samplerate;
        double const tsin = sin(omega);
        double const tcos = cos(omega);

        if (q_is_bandwidth)
            alpha = tsin * sinh(log(2.0) / 2.0 * q * omega / tsin);
        else
            alpha = tsin / (2.0 * q);

        // lowpass
        if (type == 0) {
            b0 = (1.0 - tcos) / 2.0;
            b1 = 1.0 - tcos;
            b2 = (1.0 - tcos) / 2.0;
            a0 = 1.0 + alpha;
            a1 = -2.0 * tcos;
            a2 = 1.0 - alpha;
        }

        // hipass
        if (type == 1) {
            b0 = (1.0 + tcos) / 2.0;
            b1 = -(1.0 + tcos);
            b2 = (1.0 + tcos) / 2.0;
            a0 = 1.0 + alpha;
            a1 = -2.0 * tcos;
            a2 = 1.0 - alpha;
        }

        // bandpass csg
        if (type == 2) {
            b0 = tsin / 2.0;
            b1 = 0.0;
            b2 = -tsin / 2;
            a0 = 1.0 + alpha;
            a1 = -2.0 * tcos;
            a2 = 1.0 - alpha;
        }

        // bandpass czpg
        if (type == 3) {
            b0 = alpha;
            b1 = 0.0;
            b2 = -alpha;
            a0 = 1.0 + alpha;
            a1 = -2.0 * tcos;
            a2 = 1.0 - alpha;
        }

        // notch
        if (type == 4) {
            b0 = 1.0;
            b1 = -2.0 * tcos;
            b2 = 1.0;
            a0 = 1.0 + alpha;
            a1 = -2.0 * tcos;
            a2 = 1.0 - alpha;
        }

        // allpass
        if (type == 5) {
            b0 = 1.0 - alpha;
            b1 = -2.0 * tcos;
            b2 = 1.0 + alpha;
            a0 = 1.0 + alpha;
            a1 = -2.0 * tcos;
            a2 = 1.0 - alpha;
        }
    }

    // set filter coeffs
    filter->b0a0 = (float) (b0 / a0);
    filter->b1a0 = (float) (b1 / a0);
    filter->b2a0 = (float) (b2 / a0);
    filter->a1a0 = (float) (a1 / a0);
    filter->a2a0 = (float) (a2 / a0);
}
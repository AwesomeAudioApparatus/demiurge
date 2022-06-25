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

#ifndef _DEMIURGE_cfxrbjfilter_H_
#define _DEMIURGE_cfxrbjfilter_H_

#include "signal.h"

typedef struct {
    signal_t me;
    signal_t *input;
    signal_t *frequency;
    signal_t *Q;
    signal_t *gain;

    /**
     * number |  type
     *   8    | high shelf
     *   7    | low shelf
     *   6    | peaking
     *   5    | allpass
     *   4    | notch
     *   3    | bandpass (czpg)
     *   2    | band pass (csg)
     *   1    | high pass
     *   0    | low pass
     */
    signal_t *type;
    bool q_is_bandwidth;

    // filter coeffs
    float b0a0;
    float b1a0;
    float b2a0;
    float a1a0;
    float a2a0;

    // in/out history
    float ou1;
    float ou2;
    float in1;
    float in2;
    int oldtype;

} cfxrbjfilter_t;

void cfxrbjfilter_init(cfxrbjfilter_t *handle, int position);

void cfxrbjfilter_configure_input(cfxrbjfilter_t *handle, signal_t *input);

void cfxrbjfilter_configure_frequency(cfxrbjfilter_t *handle, signal_t *input);

void cfxrbjfilter_configure_q(cfxrbjfilter_t *handle, signal_t *input);

void cfxrbjfilter_configure_type(cfxrbjfilter_t *handle, signal_t *input);

float cfxrbjfilter_read(signal_t *handle, uint64_t time);

void computeCoefficients(cfxrbjfilter_t *filter, uint64_t type, const double frequency, const double q,
                         const double db_gain, bool q_is_bandwidth);
#endif

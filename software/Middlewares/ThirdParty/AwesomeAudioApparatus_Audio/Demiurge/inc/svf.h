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

#ifndef _DEMIURGE_SVF_H_
#define _DEMIURGE_SVF_H_

#include "signal.h"

typedef struct {
    float q2;
    signal_t me;
    signal_t *input;
    signal_t *frequency;
    signal_t *Q;
    float secondLastOut;

    float lowpass_value;
    float highpass_value;
    float bandpass_value;
    float notch_value;

    signal_t lowpass_out;
    signal_t highpass_out;
    signal_t bandpass_out;
    signal_t notch_out;
} svf_t;

void svf_init(svf_t *handle);

void svf_configure_input(svf_t *handle, signal_t *input);

signal_t *svf_lowpass(svf_t *handle);

signal_t *svf_hipass(svf_t *handle);

signal_t *svf_bandpass(svf_t *handle);

signal_t *svf_notch(svf_t *handle);

void svf_configure_frequency(svf_t *handle, signal_t *midpoint);

void svf_configure_Q(svf_t *handle, signal_t *scale);

float svf_read(signal_t *handle, uint64_t time);
float svf_lowpass_read(signal_t *handle, uint64_t time);
float svf_highpass_read(signal_t *handle, uint64_t time);
float svf_bandpass_read(signal_t *handle, uint64_t time);
float svf_notch_read(signal_t *handle, uint64_t time);

#endif

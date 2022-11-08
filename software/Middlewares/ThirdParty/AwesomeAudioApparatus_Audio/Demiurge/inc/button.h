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

#ifndef _DEMIURGE_BUTTON_H_
#define _DEMIURGE_BUTTON_H_

#include "signal.h"

#define BUTTON_MODE_PRESS 0
#define BUTTON_MODE_TOGGLE 1
#define BUTTON_MODE_STEPS 2

typedef struct
{
    bool *input;
    int debounce;
    int count;
    bool last_input;
    int mode;
    float *output_levels;
    int step;
    int number_of_steps;
    float out;
    signal_t me;
} button_t;

void button_init(button_t *handle, int position);

void button_configure_for_press(button_t *handle, float off, float on);

void button_configure_for_toggle(button_t *handle, float first, float second);

void button_configure_for_steps(button_t *handle, int number_of_steps, float *stepValues);

float button_read(signal_t *handle, uint64_t time);

#endif

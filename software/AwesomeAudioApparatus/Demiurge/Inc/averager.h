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

#ifndef _DEMIURGE_AVERAGE_H_
#define _DEMIURGE_AVERAGE_H_

#include "signal.h"

typedef struct {
   float rolling_avg;
   float keep;
   float update;
   signal_t me;
   signal_t *input;
   signal_t *averaging_control;
} averager_t;

void averager_init(averager_t *handle);

void averager_configure_input(averager_t *handle, signal_t *input);

void averager_configure_avg(averager_t *handle, signal_t *average_control);

void averager_set_keep(averager_t *handle, float new_keep);

void averager_set_update(averager_t *handle,float new_update);

float averager_read(signal_t *handle, uint64_t time);

#endif

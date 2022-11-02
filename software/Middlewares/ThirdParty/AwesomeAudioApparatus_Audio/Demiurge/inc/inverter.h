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

#ifndef _DEMIURGE_INVERTER_H_
#define _DEMIURGE_INVERTER_H_

#include "signal.h"

typedef struct {
   signal_t me;
   signal_t *input;
   signal_t *scale;
   signal_t *midpoint;
} inverter_t;

void inverter_init(inverter_t *handle);
void inverter_configure_input(inverter_t *handle, signal_t *input);
void inverter_configure_midpoint(inverter_t *handle, signal_t *midpoint);
void inverter_configure_scale(inverter_t *handle, signal_t *scale);
float inverter_read(signal_t *handle, uint64_t time);

#endif

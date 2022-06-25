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

#ifndef _DEMIURGE_SCALE_H_
#define _DEMIURGE_SCALE_H_

#include "signal.h"

typedef struct {
   float scale;
   signal_t me;
   signal_t *input;
   signal_t *scale_control;
} scale_t;

void scale_init(scale_t *handle);
void scale_configure( scale_t *handle, signal_t *input, signal_t *control);
void scale_configure_input( scale_t *handle, signal_t *input);
void scale_configure_control( scale_t *handle, signal_t *control);
float scale_read(signal_t *handle, uint64_t time);

#endif

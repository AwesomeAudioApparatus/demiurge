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

#ifndef DEMIURGE_VOLUME_H
#define DEMIURGE_VOLUME_H


#include "signal.h"

typedef struct {
   signal_t me;
   signal_t *input;
   signal_t *control;
} volume_t;

void volume_init(volume_t *handle);
void volume_configure(volume_t *handle, signal_t *input, signal_t *control);
void volume_configure_input(volume_t *handle, signal_t *input);
void volume_configure_control(volume_t *handle, signal_t *control);
float volume_read(signal_t *handle, uint64_t time);

#endif

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

#ifndef _DEMIURGE_MIXER_H_
#define _DEMIURGE_MIXER_H_

#include "passthru.h"
#include "signal.h"
#include "volume.h"

typedef struct {
   int channels;
   signal_t me;
   signal_t **inputs;
   volume_t **volumes;
} mixer_t;

void mixer_init( mixer_t *handle, int channels);
void mixer_configure_input(mixer_t *handle, int number, signal_t *source, signal_t *control);
float mixer_read(signal_t *handle, uint64_t time);

#endif

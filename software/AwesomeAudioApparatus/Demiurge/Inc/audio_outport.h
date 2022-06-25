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

#ifndef _DEMIURGE_AUDIOOUTPORT_H_
#define _DEMIURGE_AUDIOOUTPORT_H_

#include "signal.h"

typedef struct {
   int position;
   signal_t me;
   signal_t *input;
   bool registered;
} audio_outport_t;

void audio_outport_init(audio_outport_t *handle, int position);
void audio_outport_configure_input(audio_outport_t *handle, signal_t *input);
float audio_outport_read(signal_t *handle, uint64_t time);

#endif

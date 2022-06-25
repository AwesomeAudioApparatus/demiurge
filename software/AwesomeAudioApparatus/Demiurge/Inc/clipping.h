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

#include <stdint.h>

#ifndef _DEMIURGE_CLIPPING_H
#define _DEMIURGE_CLIPPING_H

#ifndef DEFAULT_SAMPLE_RATE
#define DEFAULT_SAMPLE_RATE 48000
#endif

extern uint32_t demiurge_samplerate;

float clip_none(float);

float clip_gate(float value);

float clip_cv(float value);

float clip_audio(float value);

#endif

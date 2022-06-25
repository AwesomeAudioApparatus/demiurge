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

#ifndef DEMIURGE_OCTAVEPERVOLT_H
#define DEMIURGE_OCTAVEPERVOLT_H

#include <math.h>

typedef struct {
   union {
      struct {
         uint32_t mantissa: 23;
         uint32_t exp: 8;
         uint32_t sign: 1;
      };
      uint32_t intval;
      float fval;
   };
} float_raw;

void octave_init();

float octave_frequency_of(float voltage);

float octave_voltage_of(float frequency);

#endif

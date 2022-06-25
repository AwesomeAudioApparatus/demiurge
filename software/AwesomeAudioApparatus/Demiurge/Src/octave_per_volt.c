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
#include <main.h>
#include "octave_per_volt.h"

#ifndef OCTAVE_LOOKUP_SIZE
#define OCTAVE_LOOKUP_SIZE 4096
#endif

static float octave_freq[OCTAVE_LOOKUP_SIZE];
static double octave_step;

void octave_init()
{
   octave_step = 20.0f / OCTAVE_LOOKUP_SIZE;
   double volt = 0;
   for( int i=0 ; i < OCTAVE_LOOKUP_SIZE; i++ )
   {
      octave_freq[i] =  (float) (27.5 * pow(2.0, volt));
      volt = volt + octave_step;
   }
}

float octave_frequency_of(float voltage) {
   if( voltage > 10 || voltage < 0 )
   {
      return 400;
   }
//   float result = 27.5f * pow(2, voltage);
   int pos = voltage / octave_step;
   return octave_freq[pos];
}

float octave_voltage_of(float frequency) {
   return log2f(frequency / 27.5);
}

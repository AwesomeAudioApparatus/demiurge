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

#ifndef DEMIURGE_OCTAVEPERVOLT_H
#define DEMIURGE_OCTAVEPERVOLT_H

#include <math.h>

#define TWO_PI (2 * M_PI)

void octave_init();

float octave_frequency_of(float voltage);

float octave_voltage_of(float frequency);

/** Octave per Volt uses a lookup table to ensure speed in run time, as
 * the 2^Volt is too slow. The table is populated with a -10V to +10V
 * range, giving 4096 output_levels for ~1/15 Hz to 65kHz, which is way wider range
 * than needed.
 *
 * It is up to the rest of the system to keep the input voltage to this
 * function "block" within the reasonable range for the application.
 */
float digitize(float frequency);

#endif

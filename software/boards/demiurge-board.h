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

#ifndef BOARDS_BOARD_H_
#define BOARDS_BOARD_H_

#if BOARD == demi1-ch32
#include "demi1-ch32.h"
#endif

#include "demiurge.h"
#include "demiurge-spi.h"

void demiurge_set_inport_cv(int position);      // Set ADC to a 0-10V range
void demiurge_set_inport_audio(int position);   // Set ADC to a -10V to +10V range
void demiurge_set_potentiometer(int position, float min, float max);

void demiurge_set_outport_cv(int position);     // Set DAC to a 0-10V range
void demiurge_set_outport_audio(int position);  // Set DAC to a -10V to +10V range

#endif

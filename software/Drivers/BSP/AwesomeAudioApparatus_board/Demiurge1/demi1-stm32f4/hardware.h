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

#include "stm32f4xx.h"
#include "stm32f4xx_hal_def.h"
#include "demiurge-spi.h"

#ifndef _DEMI1_STM32_
#define _DEMI1_STM32_

#define MCU STM32F4

#ifndef DEMIURGE_MAX_SINKS
#define DEMIURGE_MAX_SINKS 7
#endif

#ifndef DEMIURGE_NUM_OUTPUTS
#define DEMIURGE_NUM_OUTPUTS 2
#endif

#ifndef DEMIURGE_NUM_INPUTS
#define DEMIURGE_NUM_INPUTS 8
#endif

#ifndef DEMIURGE_NUM_LEDS
#define DEMIURGE_NUM_LEDS 4
#endif

#ifndef DEMIURGE_NUM_BUTTONS
#define DEMIURGE_NUM_BUTTONS 4
#endif

#ifndef DEMIURGE_NUM_GATES
#define DEMIURGE_NUM_GATES 1
#endif

#ifndef DEMIURGE_NUM_AUDIOINPUTS
#define DEMIURGE_NUM_AUDIOINPUTS 4
#endif

#ifndef DEMIURGE_NUM_AUDIOOUTPUTS
#define DEMIURGE_NUM_AUDIOOUTPUTS 2
#endif

#ifndef DEMIURGE_NUM_CVINPUTS
#define DEMIURGE_NUM_CVINPUTS 4
#endif

#ifndef DEMIURGE_NUM_CVOUTPUTS
#define DEMIURGE_NUM_CVOUTPUTS 2
#endif

#ifndef DEMIURGE_NUM_POTS
#define DEMIURGE_NUM_POTS 4
#endif

#ifndef DEMIURGE_AUDIOINPUT_OFFSET
#define DEMIURGE_AUDIOINPUT_OFFSET 0
#endif

#ifndef DEMIURGE_AUDIOOUTPUT_OFFSET
#define DEMIURGE_AUDIOOUTPUT_OFFSET 0
#endif

#ifndef DEMIURGE_CVINPUT_OFFSET
#define DEMIURGE_CVINPUT_OFFSET 0
#endif

#define DEMIURGE_CVOUTPUT_OFFSET 0

#ifndef DEMIURGE_POTENTIOMETER_OFFSET
#define DEMIURGE_POTENTIOMETER_OFFSET 4
#endif

// If Gate Input is High, what will the signal value be internally
#ifndef DEMIURGE_GATE_HIGH
#define DEMIURGE_GATE_HIGH 10.0f
#endif

// If Gate Input is Low, what will the signal value be internally
#ifndef DEMIURGE_GATE_LOW
#define DEMIURGE_GATE_LOW -10.0f
#endif

#ifndef RED
#define RED 0x800000
#endif
#ifndef GREEN
#define GREEN 0x8000
#endif
#ifndef BLUE
#define BLUE 0x80
#endif
#ifndef WHITE
#define WHITE 0x808080
#endif

#endif

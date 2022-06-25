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

#ifndef _DEMIURGE_SIGNAL_H_
#define _DEMIURGE_SIGNAL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stm32f405xx.h>
#include <arm_math.h>
#include "clipping.h"
#include "demi_asserts.h"

// All signals uses float for representation.
// Audio = -10.0 to 10.0
// CV = -5.0 to 5.0
// Gate = 0V or >=1V
volatile typedef struct signal_struct signal_t;

typedef float (*float_fn)(float);
typedef float (*signal_fn)(signal_t *, uint64_t);

volatile typedef struct signal_struct {
   void *data;
   signal_fn read_fn;
   float_fn post_fn;
   uint64_t last_calc;
   float cached;

   // extraN are debugging variables, used by the demiurge_print_overview and only reasonable
   // way for a block to communicate with the user.
   float extra1;        // also used in fixed_signal
#ifdef DEMIURGE_DEV
   float extra2;
   float extra3;
   float extra4;
   float extra5;
   float extra6;
   float extra7;
   float extra8;
#endif
} signal_t;

#endif

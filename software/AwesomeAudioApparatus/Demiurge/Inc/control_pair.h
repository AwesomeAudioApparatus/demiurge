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

#ifndef _DEMIURGE_CONTROLPAIR_H_
#define _DEMIURGE_CONTROLPAIR_H_

#include "cv_inport.h"
#include "potentiometer.h"
#include "signal.h"

typedef struct {
   signal_t me;
   cv_inport_t cv;
   potentiometer_t potentiometer;
} control_pair_t;

void control_pair_init(control_pair_t *handle, int position);
float control_pair_read(signal_t *handle, uint64_t time);

#endif

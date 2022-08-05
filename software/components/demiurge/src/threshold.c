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

#include "demiurge.h"

//   float  setpoint = 0x2000;   // ~2.5V
//   float  hysteresis = 0x800;  // ~0.625V
void threshold_init(threshold_t *data, float setp, float hyst) {
   data->setpoint = setp;
   data->hysteresis = hyst;
   data->output = false;
}

bool threshold_compute(threshold_t *data, float input) {
   float setpoint = data->setpoint;
   float hysteresis = data->hysteresis;
   bool out;
   if (data->output) {
      out = input < setpoint - hysteresis;
   } else {
      out = input > setpoint + hysteresis;
   }
   data->output = out;
   return out;
}

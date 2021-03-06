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

float IRAM_ATTR clip_none(float value) {
   return value;
}

float IRAM_ATTR clip_gate(float value) {
   if (value >= 0.5) return 10.0;
   return -10.0;
}

float IRAM_ATTR clip_cv(float value) {
   if (value >= 10.0) return 10.0;
   if (value <= -10.0) return -10.0;
   return value;
}

float IRAM_ATTR clip_audio(float value) {
   if (value >= 10.0) return 10.0;
   if (value <= -10.0) return -10.0;
   return value;
}



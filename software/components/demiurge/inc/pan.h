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

#ifndef _DEMIURGE_PAN_H_
#define _DEMIURGE_PAN_H_

#include "signal.h"

typedef struct {
   float factor;
   signal_t me;
   signal_t *hostInput;
   signal_t *control;
} panchannel_t;

typedef struct {
   signal_t me;
   signal_t *input;
   panchannel_t left;
   panchannel_t right;
} pan_t;

void pan_init(pan_t *handle);
void pan_configure_control(pan_t *handle, signal_t *control);
float pan_read(signal_t *handle, uint64_t time);
float panchannel_read(signal_t *handle, uint64_t time);


#endif

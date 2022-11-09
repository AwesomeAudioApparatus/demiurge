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

#ifndef _DEMIURGE_RGB_H_
#define _DEMIURGE_RGB_H_


#include "signal.h"


typedef struct
{
    int position;
    bool registered;
    signal_t me;
    signal_t *input;
    uint8_t *leds;
    void (*convert_fn)(float, uint8_t* rgb);
} rgb_t;

void rgb_init(rgb_t *handle, int position);

void rgb_configure_input(rgb_t *handle, signal_t *input);

void rgb_configure_conversion( rgb_t *handle, void (*convert_fn)(float, uint8_t* rgb) );

float rgb_read(signal_t *handle, uint64_t time);

#endif

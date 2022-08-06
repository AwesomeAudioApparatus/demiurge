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

#ifndef _DEMI1_CH32_
#define _DEMI1_CH32_

#define MCU CH32V307

#define USB_PRESENT
#define SDCARD_PRESENT
#define FLASH_CHIP W25Q128
#define FLASH_SIZE_BYTES 16777216  // 16MBytes of flash

#define HSE_VALUE    ((uint32_t)16000000) // Value of the External oscillator in Hz

#include "ch32v30x.h"
#include "ch32v30x_conf.h"

#endif

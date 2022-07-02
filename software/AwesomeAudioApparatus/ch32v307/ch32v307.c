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

#include "demiurge-spi.h"
#include "ch32v30x.h"

static demiurge_driver_info_t driver_info = {
    .name = "Awesome Audio Apparatus - Demiurge 1",
    .inputs = 4,
    .outputs = 2,
    .gates = 1,
    .buttons = 4,
    .leds = 4,
    .potentionmeters = 4,
    .usb = true,
    .flash = 16,
    .sdcard = true,
	.max_frequency = 48,
	.recommended_frequency = 24
};

void demiurge_driver_init()
{
    USART_Printf_Init(57600);
    printf("SystemClk:%d\r\n", SystemCoreClock);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

void demiurge_driver_start()
{
	set_testpoint(0);
	clear_testpoint(0);
	set_testpoint(0);
	clear_testpoint(0);
	set_testpoint(0);
	clear_testpoint(0);
}

demiurge_driver_info_t *demiurge_driver_info()
{
	return &driver_info;
}

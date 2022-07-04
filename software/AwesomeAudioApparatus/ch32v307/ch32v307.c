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
	.recommended_frequency = 24,
	.display = false,
	.button_navigation = false
};

void demiurge_driver_init(uint32_t samplerate)
{
    USART_Printf_Init(230400);
    printf("SystemClk:%d\r\n", SystemCoreClock);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    init_testpoints();
    init_adc();
    init_buttons();
    init_dac();
    init_flash();
    init_gates();
    init_leds();
    init_potentiometers();
    init_sdcard();
    init_usb();

    init_timer(samplerate);

}

void demiurge_driver_start()
{
    start_testpoints();

    set_testpoint(0);
	clear_testpoint(0);

	start_adc();
    start_buttons();
    start_dac();
    start_flash();
    start_gates();
    start_leds();
    start_potentiometers();
    start_sdcard();
    start_usb();

	set_testpoint(0);
	clear_testpoint(0);
	set_testpoint(0);
	clear_testpoint(0);

    start_timer();
}

void demiurge_driver_stop()
{
    stop_timer();

    set_testpoint(0);
	clear_testpoint(0);

	stop_potentiometers();
    stop_buttons();
    stop_usb();
    stop_sdcard();
    stop_adc();
    stop_dac();
    stop_flash();
    stop_gates();
    stop_leds();

	set_testpoint(0);
	clear_testpoint(0);

	stop_testpoints();
}

demiurge_driver_info_t *demiurge_driver_info()
{
	return &driver_info;
}

// Display - doesn't exist
void update_display(){}
void init_display(){}
void start_display(){}
void stop_display(){}

// Button Navigation - doesn't exist
void read_navigation(){}
void init_navigation(){}
void start_navigation(){}
void stop_navigation(){}

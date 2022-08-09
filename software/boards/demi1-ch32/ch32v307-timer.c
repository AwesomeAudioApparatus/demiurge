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

#include "ch32v30x.h"
#include "demiurge-spi.h"

static int flag = 0;

__attribute__((interrupt("WCH-Interrupt-fast"))) void TIM7_IRQHandler() {
    GET_INT_SP();
    demiurge_tick();
    TIM7->INTFR = 0;
    FREE_INT_SP();
}

void init_timer(int samplerate) {

    uint32_t prescale = 0;
    uint32_t period = (144000000 / samplerate) - 1;

    printf("Prescale:%d, period:%d\n", prescale, period);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure={0};
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = period;
    TIM_TimeBaseStructure.TIM_Prescaler = prescale;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    // TODO: What priorities should we have? Surely the Audio stream has higher
    //       priority than system clock for slow stuff.
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
    TIM7->DMAINTENR |= TIM_UIE;
    NVIC_EnableIRQ(TIM7_IRQn);
}

void start_timer() {
    TIM_Cmd(TIM7, ENABLE);
}

void stop_timer() {
    TIM_Cmd(TIM7, DISABLE);
}

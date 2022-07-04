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

float outputs[2];
static uint16_t dac_counter = 0;
static uint16_t dac_state = 0;

void update_dac() {
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
    uint16_t ch1 = (uint16_t) ((outputs[0] + 10.0f) * 204.7f);
    uint16_t ch2 = (uint16_t) ((outputs[1] + 10.0f) * 204.7f);

    DAC->RD12BDHR = ch1 + (ch2 << 16);
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
}

void init_dac() {

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );

    DAC_InitTypeDef initialization={0};
    initialization.DAC_Trigger=DAC_Trigger_None;
    initialization.DAC_WaveGeneration=DAC_WaveGeneration_None;
    initialization.DAC_OutputBuffer=DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1,&initialization);
    DAC_Init(DAC_Channel_2,&initialization);

}

void start_dac() {
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, ENABLE);
}

void stop_dac() {
    DAC_Cmd(DAC_Channel_1, DISABLE);
    DAC_Cmd(DAC_Channel_2, DISABLE);
}

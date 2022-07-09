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
float dac1_scale;
float dac2_scale;
float dac1_offset;
float dac2_offset;

void update_dac() {
    int32_t ch1 = (int32_t) ((outputs[0] + dac1_offset) * dac1_scale);
    int32_t ch2 = (int32_t) ((outputs[1] + dac2_offset) * dac2_scale);
    if( ch1 > 4095 ) ch1 = 4095;
    if( ch2 > 4095 ) ch2 = 4095;
    if( ch1 < 0 ) ch1 = 0;
    if( ch2 < 0 ) ch2 = 0;
    DAC->RD12BDHR = ch1 + (ch2 << 16);
}

void init_dac(float *scales, float *offsets) {

	dac1_scale = scales[0];
	dac2_scale = scales[1];
	dac1_offset = offsets[0];
	dac2_offset = offsets[1];

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

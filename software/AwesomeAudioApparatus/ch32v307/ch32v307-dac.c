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
static uint16_t dac_test = 0;

void update_dac() {
//   uint16_t ch1 = (uint16_t) ((10.0f - outputs[0]) * 204.7f);
//   uint16_t ch2 = (uint16_t) ((10.0f - outputs[1]) * 204.7f);
//   DAC_SetDualChannelData(DAC_Align_12b_R, ch1,ch2);

   dac_test++;
   DAC_SetDualChannelData(DAC_Align_12b_R, dac_test, dac_test);
   if( dac_test > 2047)
	   dac_test = 0;
}

void init_dac() {
	DAC_InitTypeDef DAC_InitType={0};

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );


	DAC_InitType.DAC_Trigger=DAC_Trigger_None;
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;
    DAC_Init(DAC_Channel_1,&DAC_InitType);
    DAC_Init(DAC_Channel_2,&DAC_InitType);

    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, ENABLE);
}

void start_dac() {
//   HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
//   HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
}

void stop_dac() {
//   HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
//   HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);
}

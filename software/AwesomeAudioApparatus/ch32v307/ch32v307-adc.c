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

float inputs[8];

void read_adc() {
//   inputs[0] = ((float) hadc1.Instance->DR) / 204.8f;
//   inputs[1] = ((float) hadc2.Instance->DR) / 204.8f;
}

void init_adc() {

	GPIO_InitTypeDef GPIO_InitStructure={0};

	// Analog Inputs on Port A
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 /* used for serial port now |GPIO_Pin_2|GPIO_Pin_3 */;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Analog Inputs on Port B
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Analog Inputs on Port C
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

    // What was this for? The DAC example had this for one of the 2 AI used.
	// GPIO_SetBits(GPIOA,GPIO_Pin_4);
}

void start_adc() {
//   HAL_ADC_Start(&hadc1);
//   HAL_ADC_Start(&hadc2);
}

void stop_adc() {
//   HAL_ADC_Stop(&hadc1);
//   HAL_ADC_Stop(&hadc2);
}

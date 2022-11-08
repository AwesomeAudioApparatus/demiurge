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

#ifndef BOARDS_BOARD_H_
#define BOARDS_BOARD_H_

#include "RTE_Components.h"

#if BOARD == demi1-stm32f4
#include "demi1-stm32f4/hardware.h"
#endif

#include "demiurge.h"
#include "demiurge-spi.h"


typedef enum {CV, Audio, Trig, Gate} inout_type ;

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern DAC_HandleTypeDef hdac;
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;

void MX_User_Init();
void MX_User_Process();

void demiurge_input_mode(int position, inout_type t);
void demiurge_output_mode(int position, inout_type t);
void demiurge_potentiometer_mode(int position, float min, float max);

#endif

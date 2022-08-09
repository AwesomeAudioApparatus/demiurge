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
#include "demi1-ch32.h"

#include "demiurge-spi.h"

void init_testpoints()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void start_testpoints()
{
}

void stop_testpoints()
{
}

void set_testpoint(int point)
{
    switch( point )
    {
    case 0:
        GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);
        break;
    case 1:
        GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);
        break;
    case 2:
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
        break;
    }
}

void clear_testpoint(int point)
{
    switch( point )
    {
    case 0:
        GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET);
        break;
    case 1:
        GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);
        break;
    case 2:
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
        break;
    }
}


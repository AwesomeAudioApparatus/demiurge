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

#include "demiurge-hardware.h"

void init_testpoints()
{
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
        HAL_GPIO_WritePin(GPIOB, 3, GPIO_PIN_SET);
        break;
    case 1:
        HAL_GPIO_WritePin(GPIOB, 4, GPIO_PIN_SET);
        break;
    case 2:
        HAL_GPIO_WritePin(GPIOB, 5, GPIO_PIN_SET);
        break;
    }
}

void clear_testpoint(int point)
{
    switch( point )
    {
    case 0:
        HAL_GPIO_WritePin(GPIOB, 3, GPIO_PIN_RESET);
        break;
    case 1:
        HAL_GPIO_WritePin(GPIOB, 4, GPIO_PIN_RESET);
        break;
    case 2:
        HAL_GPIO_WritePin(GPIOB, 5, GPIO_PIN_RESET);
        break;
    }
}


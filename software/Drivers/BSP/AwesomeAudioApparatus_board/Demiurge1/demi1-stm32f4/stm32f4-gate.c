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

#include "demiurge-hardware.h"

bool gates_in[DEMIURGE_NUM_GATES];
bool gates_out[DEMIURGE_NUM_GATES];
bool gates_dir[DEMIURGE_NUM_GATES];

void init_gates(void)
{
}

void read_gates()
{
    gates_in[0] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET;
}

void update_gates()
{
    if (gates_dir[0])
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, gates_out[0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void start_gates()
{
    // nothing to start
}

void stop_gates()
{
    // nothing to stop
}

static void set_gate_input()
{
    gates_dir[0] = false;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
}

static void set_gate_output()
{
    gates_dir[0] = true;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
}

void direction_gate(int gate, bool output)
{
    if (output)
        set_gate_output();
    else
        set_gate_input();
}

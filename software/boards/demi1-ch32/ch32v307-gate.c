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
#include "hardware.h"
#include <stdbool.h>

#include "demiurge-spi.h"

bool gates_in[DEMIURGE_NUM_GATES];
bool gates_out[DEMIURGE_NUM_GATES];
bool gates_dir[DEMIURGE_NUM_GATES];

void init_gates(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void read_gates() {
	if(!gates_dir[0])
        gates_in[0] = (GPIOA->INDR & GPIO_Pin_7) != 0;
}

void update_gates() {
	if(gates_dir[0])
        GPIO_WriteBit(GPIOA, GPIO_Pin_7, gates_out[0] ? Bit_SET : Bit_RESET);
}

void start_gates()
{

}

void stop_gates()
{

}

static void __set_gate_input(int gate)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    gates_dir[0] = false;
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
}

static void __set_gate_output(int gate)
{
    gates_dir[0] = true;
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void direction_gate(int gate, bool output)
{
    if(output)
        __set_gate_output(gate);
    else
        __set_gate_input(gate);
}

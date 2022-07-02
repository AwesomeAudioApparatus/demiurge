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
#include <stdbool.h>

bool gates_in[1];
bool gates_out[1];
bool gates_dir[1];

void init_gates(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
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


void demiurge_set_gate_input(int gate)
{
	gates_dir[0] = false;
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
}

void demiurge_set_gate_output(int gate)
{
	gates_dir[0] = true;
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
}

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

#define NUMBER_OF_AI 8

float inputs[8];

uint16_t buffer[8];

int16_t calibration_adc;

static void initialize_gpio_ai_pin(GPIO_TypeDef *port, uint16_t pin){
    GPIO_InitTypeDef GPIO_InitStructure={0};
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(port, &GPIO_InitStructure);
}

int16_t initialize_and_calibrate_adc(ADC_TypeDef *adc)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // CV1 -> AD3 -> PB1 -> ADC_IN9
    // CV2 -> AD2 -> PB0 -> ADC_IN8
    // CV3 -> AD1 -> PC5 -> ADC_IN15
    // CV4 -> AD0 -> PC4 -> ADC_IN14
    // RV1 -> AD7 -> PA3 -> ADC_IN3
    // RV2 -> AD6 -> PA2 -> ADC_IN2
    // RV3 -> AD5 -> PA1 -> ADC_IN1
    // RV4 -> AD4 -> PA0 -> ADC_IN0
    initialize_gpio_ai_pin(GPIOC, GPIO_Pin_4);
    initialize_gpio_ai_pin(GPIOC, GPIO_Pin_5);
    initialize_gpio_ai_pin(GPIOB, GPIO_Pin_0);
    initialize_gpio_ai_pin(GPIOB, GPIO_Pin_1);
    initialize_gpio_ai_pin(GPIOA, GPIO_Pin_0);
    initialize_gpio_ai_pin(GPIOA, GPIO_Pin_1);
    initialize_gpio_ai_pin(GPIOA, GPIO_Pin_2);
    initialize_gpio_ai_pin(GPIOA, GPIO_Pin_3);

    ADC_DeInit(adc);
    ADC_InitTypeDef ADC_InitStructure={0};
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = NUMBER_OF_AI;
    ADC_Init(adc, &ADC_InitStructure);

    ADC_Cmd(adc, ENABLE);

    ADC_BufferCmd(adc, DISABLE);
    ADC_ResetCalibration(adc);
    while(ADC_GetResetCalibrationStatus(adc));
    ADC_StartCalibration(adc);
    while(ADC_GetCalibrationStatus(adc));
    int16_t calibration = Get_CalibrationValue(adc);

    ADC_BufferCmd(adc, ENABLE);
    ADC_DMACmd(adc, ENABLE);
    return calibration;
}

void init_adc_dma()
{

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );
    DMA_DeInit(DMA1_Channel1);

    DMA_InitTypeDef initialization={0};
    initialization.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->RDATAR;
    initialization.DMA_MemoryBaseAddr = (uint32_t)buffer;
    initialization.DMA_DIR = DMA_DIR_PeripheralSRC;
    initialization.DMA_BufferSize = NUMBER_OF_AI;

    initialization.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    initialization.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    initialization.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    initialization.DMA_MemoryInc = DMA_MemoryInc_Enable;

    initialization.DMA_Mode = DMA_Mode_Circular;
    initialization.DMA_Priority = DMA_Priority_VeryHigh;
    initialization.DMA_M2M = DMA_M2M_Disable;
    DMA_Init( DMA1_Channel1, &initialization );
}

static void init_adc_timers() {

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure={0};

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 0xFFFE;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//    NVIC_InitTypeDef NVIC_InitStructure = {0};
//    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//    // TODO: What priorities should we have? Surely the Audio stream has higher priority than system clock for slow stuff.
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//
//    NVIC_Init(&NVIC_InitStructure);

    ADC1->CTLR2 &= 0xFFFF8FFF;
    ADC1->CTLR2 |= 0x00180000;	// Set REXTTRIG on TIM3 TRGO

    TIM3->DMAINTENR |= TIM_UIE;
}
uint32_t debug1 = 0;
uint32_t debug2 = 0;
uint32_t debug3 = 0;

void read_adc() {
	debug1 = ADC1->RSQR1;
	debug2 = ADC1->RSQR2;
	debug3 = ADC1->RSQR3;
    inputs[0] = ((float) buffer[0]) / 204.8f - 10.0f;
    inputs[1] = ((float) buffer[1]) / 204.8f - 10.0f;
    inputs[2] = ((float) buffer[2]) / 204.8f - 10.0f;
    inputs[3] = ((float) buffer[3]) / 204.8f - 10.0f;
    inputs[4] = ((float) buffer[4]) / 204.8f - 10.0f;
    inputs[5] = ((float) buffer[5]) / 204.8f - 10.0f;
    inputs[6] = ((float) buffer[6]) / 204.8f - 10.0f;
    inputs[7] = ((float) buffer[7]) / 204.8f - 10.0f;
}

void init_adc() {
    calibration_adc = initialize_and_calibrate_adc(ADC1);
    printf("ADC1 calibration value: %d\n", calibration_adc);

    init_adc_timers();
    init_adc_dma();

    // The out of order in the "Rank" is due to DMA start transfer when
    // the ADC converts the first value. So when the first ADC is completed
    // the DMA is ready to transfer that to buffer[1], and so on until
    // the 8th ADC conversion is completed and the DMA has reset its memory
    // destination pointer back to buffer[0]

    // CV1 -> AD3 -> PB1 -> ADC_IN9  -> buffer[0]
    // CV2 -> AD2 -> PB0 -> ADC_IN8  -> buffer[1]
    // CV3 -> AD1 -> PC5 -> ADC_IN15 -> buffer[2]
    // CV4 -> AD0 -> PC4 -> ADC_IN14 -> buffer[3]
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 8, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 3, ADC_SampleTime_239Cycles5);

    // RV
    // RV1 -> AD7 -> PA3 -> ADC_IN3  -> buffer[4]
    // RV2 -> AD6 -> PA2 -> ADC_IN2  -> buffer[5]
    // RV3 -> AD5 -> PA1 -> ADC_IN1  -> buffer[6]
    // RV4 -> AD4 -> PA0 -> ADC_IN0  -> buffer[7]
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 5, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 6, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 7, ADC_SampleTime_239Cycles5);

}

void start_adc() {
    DMA_Cmd( DMA1_Channel1, ENABLE );
}

void stop_adc() {
    DMA_Cmd( DMA1_Channel1, DISABLE );
}





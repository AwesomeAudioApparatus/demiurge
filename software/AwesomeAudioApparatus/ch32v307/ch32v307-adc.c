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

float inputs[16];

uint16_t buffer1[16];
uint16_t buffer2[16];

int16_t calibration_adc1;
int16_t calibration_adc2;


int16_t ADC_Function_Init(ADC_TypeDef *adc)
{
    ADC_InitTypeDef ADC_InitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(adc);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(adc, &ADC_InitStructure);

    ADC_DMACmd(adc, ENABLE);
    ADC_Cmd(adc, ENABLE);

    ADC_BufferCmd(adc, DISABLE);
    ADC_ResetCalibration(adc);
    while(ADC_GetResetCalibrationStatus(adc));
    ADC_StartCalibration(adc);
    while(ADC_GetCalibrationStatus(adc));
    int16_t calibration = Get_CalibrationValue(adc);

    ADC_BufferCmd(adc, ENABLE);
    return calibration;
}

u16 Get_ADC_Val(u8 ch)
{
    u16 val;

    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
    val = ADC_GetConversionValue(ADC1);

    return val;
}

void DMA_Tx_Init( DMA_Channel_TypeDef* DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef DMA_InitStructure={0};

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );

    DMA_DeInit(DMA_CHx);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = sizeof(buffer1) / sizeof(uint16_t);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init( DMA_CHx, &DMA_InitStructure );
}

u16 Get_ConversionVal(int16_t val, int16_t calibration)
{
    if(val + calibration < 0)
    	return 0;

    if(calibration + val > 4095 || val == 4095)
    	return 4095;

    return val + calibration;
}

void read_adc() {
//   inputs[0] = ((float) hadc1.Instance->DR) / 204.8f;
//   inputs[1] = ((float) hadc2.Instance->DR) / 204.8f;
}

void init_adc() {
    calibration_adc1 = ADC_Function_Init(ADC1);
    printf("ADC1 calibration: %d\n", calibration_adc1);

    DMA_Tx_Init( DMA1_Channel1, (u32)&ADC1->RDATAR, (u32)buffer1, 16);
    DMA_Cmd( DMA1_Channel1, ENABLE );

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void start_adc() {
}

void stop_adc() {
}

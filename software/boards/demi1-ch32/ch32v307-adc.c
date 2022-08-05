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

#include "demiurge-spi.h"

#define NUMBER_OF_AI 8

float inputs[8];

uint16_t buffer[8];

int16_t calibration_adc;

static float scale_ch0;
static float scale_ch1;
static float scale_ch2;
static float scale_ch3;
static float scale_ch4;
static float scale_ch5;
static float scale_ch6;
static float scale_ch7;

static float offset_ch0;
static float offset_ch1;
static float offset_ch2;
static float offset_ch3;
static float offset_ch4;
static float offset_ch5;
static float offset_ch6;
static float offset_ch7;

static void initialize_gpio_ai_pin(GPIO_TypeDef *port, uint16_t pin){
    GPIO_InitTypeDef GPIO_InitStructure={0};
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(port, &GPIO_InitStructure);
}

int16_t initialize_and_calibrate_adc()
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
//    initialize_gpio_ai_pin(GPIOA, GPIO_Pin_2);
//    initialize_gpio_ai_pin(GPIOA, GPIO_Pin_3);

    ADC_DeInit(ADC1);
    ADC_InitTypeDef ADC_InitStructure={0};
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
//    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = NUMBER_OF_AI;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    // TODO: What should we do with calibration? Buffer is disabled, so is it of any use?
    //       And if it is useful, then we should do something with the value.
    ADC_BufferCmd(ADC1, DISABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    int16_t calibration = Get_CalibrationValue(ADC1);
    ADC_BufferCmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, DISABLE);

    return calibration;
}

void init_adc_dma()
{
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );
    DMA_DeInit(DMA1_Channel1);

    DMA_InitTypeDef init={0};
    init.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->RDATAR;
    init.DMA_MemoryBaseAddr = (uint32_t)buffer;
    init.DMA_DIR = DMA_DIR_PeripheralSRC;
    init.DMA_BufferSize = NUMBER_OF_AI;

    init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    init.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    init.DMA_MemoryInc = DMA_MemoryInc_Enable;

    init.DMA_Mode = DMA_Mode_Circular;
    init.DMA_Priority = DMA_Priority_VeryHigh;
    init.DMA_M2M = DMA_M2M_Disable;
    DMA_Init( DMA1_Channel1, &init );
}

uint32_t status;

void read_adc() {

    inputs[0] = ((float) buffer[0]) * scale_ch0 + offset_ch0;
    inputs[1] = ((float) buffer[1]) * scale_ch1 + offset_ch1;
    inputs[2] = ((float) buffer[2]) * scale_ch2 + offset_ch2;
    inputs[3] = ((float) buffer[3]) * scale_ch3 + offset_ch3;
    inputs[4] = ((float) buffer[4]) * scale_ch4 + offset_ch4;
    inputs[5] = ((float) buffer[5]) * scale_ch5 + offset_ch5;
    inputs[6] = ((float) buffer[6]) * scale_ch6 + offset_ch6;
    inputs[7] = ((float) buffer[7]) * scale_ch7 + offset_ch7;

    // Trigger new read cycle
    ADC1->CTLR2 |= ((uint32_t)0x00500000);
}

void init_adc(float *scales, float *offsets) {
	scale_ch0 = scales[0];
	scale_ch1 = scales[1];
	scale_ch2 = scales[2];
	scale_ch3 = scales[3];
	scale_ch4 = scales[4];
	scale_ch5 = scales[5];
	scale_ch6 = scales[6];
	scale_ch7 = scales[7];
	offset_ch0 = offsets[0];
	offset_ch1 = offsets[1];
	offset_ch2 = offsets[2];
	offset_ch3 = offsets[3];
	offset_ch4 = offsets[4];
	offset_ch5 = offsets[5];
	offset_ch6 = offsets[6];
	offset_ch7 = offsets[7];
    calibration_adc = initialize_and_calibrate_adc(ADC1);
    printf("ADC1 calibration value: %d\n", calibration_adc);

//    init_adc_timers();
    init_adc_dma();

    ADC1->CTLR2 &= ~ADC_EXTSEL;
    ADC1->CTLR2 |= ADC_EXTSEL_0;    // 111 : triggered by software setting RSWSTART
    ADC1->CTLR2 |= ADC_EXTSEL_1;
    ADC1->CTLR2 |= ADC_EXTSEL_2;
    ADC_ExternalTrigConvCmd(ADC1,ENABLE);
//    ADC1->CTLR2 |= ADC_EXTTRIG;


    // The out of order in the "Rank" is due to DMA start transfer when
    // the ADC converts the first value. So when the first ADC is completed
    // the DMA is ready to transfer that to buffer[1], and so on until
    // the 8th ADC conversion is completed and the DMA has reset its memory
    // destination pointer back to buffer[0]

    // CV1 -> AD3 -> PB1 -> ADC_IN9  -> buffer[0]
    // CV2 -> AD2 -> PB0 -> ADC_IN8  -> buffer[1]
    // CV3 -> AD1 -> PC5 -> ADC_IN15 -> buffer[2]
    // CV4 -> AD0 -> PC4 -> ADC_IN14 -> buffer[3]
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 3, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 4, ADC_SampleTime_239Cycles5);

    // RV1 -> AD7 -> PA3 -> ADC_IN3  -> buffer[4]
    // RV2 -> AD6 -> PA2 -> ADC_IN2  -> buffer[5]
    // RV3 -> AD5 -> PA1 -> ADC_IN1  -> buffer[6]
    // RV4 -> AD4 -> PA0 -> ADC_IN0  -> buffer[7]
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 5, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 6, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 7, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 8, ADC_SampleTime_239Cycles5);

}

void start_adc() {
    ADC_Cmd(ADC1, ENABLE);
    DMA_Cmd( DMA1_Channel1, ENABLE );
    ADC_DMACmd(ADC1, ENABLE);
}

void stop_adc() {
    DMA_Cmd( DMA1_Channel1, DISABLE );
    ADC_BufferCmd(ADC1, DISABLE);
    ADC_DMACmd(ADC1, DISABLE);
}





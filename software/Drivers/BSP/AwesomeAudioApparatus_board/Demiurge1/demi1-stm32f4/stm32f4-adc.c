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

float inputs[8];

uint16_t adc_buffer[8];

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

int16_t initialize_and_calibrate_adc()
{
    return 0;
}

void init_adc_dma()
{
}

uint32_t status;

void read_adc()
{

    inputs[0] = ((float) adc_buffer[0]) * scale_ch0 + offset_ch0;
    inputs[1] = ((float) adc_buffer[1]) * scale_ch1 + offset_ch1;
    inputs[2] = ((float) adc_buffer[2]) * scale_ch2 + offset_ch2;
    inputs[3] = ((float) adc_buffer[3]) * scale_ch3 + offset_ch3;
    inputs[4] = ((float) adc_buffer[4]) * scale_ch4 + offset_ch4;
    inputs[5] = ((float) adc_buffer[5]) * scale_ch5 + offset_ch5;
    inputs[6] = ((float) adc_buffer[6]) * scale_ch6 + offset_ch6;
    inputs[7] = ((float) adc_buffer[7]) * scale_ch7 + offset_ch7;

    // Clear the EOC (End-Of-Conversion) bit
    ADC1->SR = 0;
}

void init_adc(float *scales, float *offsets)
{
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
    printf("ADC1 calibration value: %d\n", (int) calibration_adc);

    // The out of order in the "Rank" is due to DMA start transfer when
    // the ADC converts the first value. So when the first ADC is completed
    // the DMA is ready to transfer that to buffer[1], and so on until
    // the 8th ADC conversion is completed and the DMA has reset its memory
    // destination pointer back to buffer[0]

    // CV1 -> AD3 -> PB1 -> ADC_IN9  -> buffer[0]
    // CV2 -> AD2 -> PB0 -> ADC_IN8  -> buffer[1]
    // CV3 -> AD1 -> PC5 -> ADC_IN15 -> buffer[2]
    // CV4 -> AD0 -> PC4 -> ADC_IN14 -> buffer[3]

    // RV1 -> AD7 -> PA3 -> ADC_IN3  -> buffer[4]
    // RV2 -> AD6 -> PA2 -> ADC_IN2  -> buffer[5]
    // RV3 -> AD5 -> PA1 -> ADC_IN1  -> buffer[6]
    // RV4 -> AD4 -> PA0 -> ADC_IN0  -> buffer[7]

    init_adc_dma();
}

void start_adc()
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adc_buffer, 8);
}

void stop_adc()
{
    HAL_ADC_Stop(&hadc1);
    HAL_ADC_Stop_DMA(&hadc1);
}





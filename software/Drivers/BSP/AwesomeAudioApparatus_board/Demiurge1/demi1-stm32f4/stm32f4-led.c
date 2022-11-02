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

#define AW20036_ADDR (0x3A << 1)

#define I2C_TIMEOUT 100

float leds[4];
uint32_t rgb[4];

// Default is a rather nonsensical Red and Green conversion.
static uint32_t default_convert(float value )
{
    if( value > 0){
        return ((uint32_t) value * 25) << 8;
    }
    return ((uint32_t) value * -25) << 16;
}

static uint32_t (*convert)(float) = default_convert;

static uint8_t readRegister(int regAddress)
{
	uint8_t data = 0;
	HAL_I2C_Mem_Read(&hi2c1, AW20036_ADDR, regAddress, 1, &data, 1, I2C_TIMEOUT);
    return data;
}

static void setRegister(int regAddress, int value)
{
	uint8_t data = value;
	HAL_I2C_Mem_Write(&hi2c1, AW20036_ADDR, regAddress, 1, &data, 1, I2C_TIMEOUT);
}

static void turnoff_AW20036()
{
    // Turn off AW20036 chip
    HAL_GPIO_WritePin(GPIOB, 9, GPIO_PIN_RESET);
}

static void turnon_AW20036()
{
    // Turn on AW20036 chip
    HAL_GPIO_WritePin(GPIOB, 9, GPIO_PIN_SET);
}

void update_leds() {
   // TODO: on AW20036 via I2C
    rgb[0] = convert(leds[0]);
    rgb[1] = convert(leds[1]);
    rgb[2] = convert(leds[2]);
    rgb[3] = convert(leds[3]);
}

static uint8_t page0[] = {
                          0x00,
                          0x01,
                          0x03,
                          0x05,
                          0x09,
                          0x0A,
                          0x0B,
                          0x43,
                          0x44,
                          0x45,
                          0x46,
                          0x47,
                          0x48,
                          0x49,
                          0x4A,
                          0x4B,
                          0x4C,
                          0x4D,
                          0x4E,
                          0x4F,
                          0x50,
                          0x51,
                          0x52,
                          0x53,
                          0x54,
                          0x55,
                          0x56,
                          0x57,
                          0x58,
                          0x59,
                          0x80
};

static uint8_t led_reg[] = {
                            0x00,0x01,0x02,  // LED 1; R, G, B
                            0x0C,0x0D,0x0E,  // LED 2; R, G, B
                            0x18,0x19,0x1A,  // LED 3; R, G, B
                            0x03,0x04,0x05   // LED 4; R, G, B
};

static void test_led(int reg, int max) {
    for( int j=0; j < max; j++){
        setRegister(reg, j);
        HAL_Delay(10);
    }
    setRegister(reg, 0x0);
}

void write_registers() {
    setRegister(0xF0, 0xC0);
    setRegister(0x1, 0x0);
    setRegister(0x3, 0x18);
    setRegister(0x80, 0x2);
    setRegister(0x31, 0x3F);
    setRegister(0x32, 0x3F);
    setRegister(0x33, 0x3F);
    setRegister(0x34, 0x3F);
    setRegister(0x35, 0x3F);
    setRegister(0x36, 0x3F);
    setRegister(0xF0, 0xC1);
    for( int i=0; i < 36; i++)
        setRegister(i, 0x3F);
    setRegister(0xF0, 0xC2);
    for( int i=0; i < sizeof(led_reg); i++) {
        test_led(led_reg[i], 256);
        HAL_Delay(1000);
    }
//    setRegister(0x00, rgb[0] >> 16);
//    setRegister(0x01, rgb[0] >> 8);
//    setRegister(0x02, rgb[0]);
//    setRegister(0x03, rgb[1] >> 16);
//    setRegister(0x04, rgb[1] >> 8);
//    setRegister(0x05, rgb[1]);
//    setRegister(0x06, rgb[2] >> 16);
//    setRegister(0x07, rgb[2] >> 8);
//    setRegister(0x08, rgb[2]);
//    setRegister(0x09, rgb[3] >> 16);
//    setRegister(0x0A, rgb[3] >> 8);
//    setRegister(0x0B, rgb[3]);
//    setRegister(0x0C, 0x80);
//    setRegister(0x0D, 0x80);
//    setRegister(0x0E, 0x80);
//    setRegister(0x0F, 0x80);
//    setRegister(0x10, 0x80);
//    setRegister(0x11, 0x80);
//    setRegister(0x12, 0x80);
//    setRegister(0x13, 0x80);
//    setRegister(0x14, 0x80);
//    setRegister(0x15, 0x80);
//    setRegister(0x16, 0x80);
//    setRegister(0x17, 0x80);
//    setRegister(0x18, 0x80);
//    setRegister(0x19, 0x80);
//    setRegister(0x20, 0x80);
//    setRegister(0x21, 0x80);
//    setRegister(0x22, 0x80);
//    setRegister(0x23, 0x80);

    setRegister(0xF0, 0xC0);
    for( int i=0; i < sizeof(page0); i++)
        printf("Page 0, Address %d = %d\n", page0[i], readRegister(page0[i]));
}

void init_leds()
{
    turnoff_AW20036();
    turnon_AW20036();

    setRegister(0xF0, 0); // addressing page 0

    printf("Chip ID: %d\n", readRegister(0));
    printf("Sleep Mode Ctrl: %d\n", readRegister(1));
    printf("Global Current Config: %d\n", readRegister(3));
    printf("Clock Ctrl: %d\n", readRegister(5));
    printf("Fault Config 1: %d\n", readRegister(9));
    printf("Fault Config 2: %d\n", readRegister(0xa));
    printf("Interrupt Status: %d\n", readRegister(0xb));
    HAL_Delay(10);
}

void start_leds()
{
    write_registers();
}

void stop_leds()
{
}

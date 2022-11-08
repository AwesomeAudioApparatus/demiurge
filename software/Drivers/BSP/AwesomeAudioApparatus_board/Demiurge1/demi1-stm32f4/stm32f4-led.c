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

#define AW20036_ADDR (0x3A << 1)

#define I2C_TIMEOUT 100

static uint8_t leds_page2[36] = {0};

static uint8_t led_reg[] = {
    0x00, 0x01, 0x02,  // LED 1; R, G, B
    0x0C, 0x0D, 0x0E,  // LED 2; R, G, B
    0x18, 0x19, 0x1A,  // LED 3; R, G, B
    0x03, 0x04, 0x05   // LED 4; R, G, B
};

static uint8_t readRegister(int regAddress)
{
    uint8_t data = 0;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, AW20036_ADDR, regAddress, 1, &data, 1, I2C_TIMEOUT);
    if (status != HAL_OK)
    {
        uint32_t errorCode = hi2c1.ErrorCode;
        printf("I2C error: %d, %ld", status, errorCode);
    }
    return data;
}

static void setRegister(int regAddress, int value)
{
    uint8_t data = value;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, AW20036_ADDR, regAddress, 1, &data, 1, I2C_TIMEOUT);
    if (status != HAL_OK)
    {
        uint32_t errorCode = hi2c1.ErrorCode;
        printf("I2C error: %d, %ld", status, errorCode);
    }
}

static void turnoff_AW20036()
{
    // Turn off AW20036 chip
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
}

static void turnon_AW20036()
{
    // Turn on AW20036 chip
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
}

void update_leds()
{
    if (hi2c1.State == HAL_I2C_STATE_READY)
    {
        HAL_StatusTypeDef status = HAL_I2C_Mem_Write_DMA(&hi2c1, AW20036_ADDR, 0,
                                                         1, leds_page2,sizeof(leds_page2));
        if (status != HAL_OK)
        {
            printf("I2C problem: %d, %ld", status, hi2c1.ErrorCode);
        }
    }
    else
    {
        toggle_testpoint(2);
    }
}

static uint8_t page0_of_interest[] = {
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

static void test_led(int reg, int color)
{
    setRegister(0xF0, 0xC2);
    setRegister(reg, color);
    HAL_Delay(100);
    setRegister(reg, 0x0);
    HAL_Delay(100);
}

void write_registers()
{
    setRegister(0xF0, 0xC0);
    setRegister(0x1, 0x0);
    setRegister(0x3, 0x18);  // High 4 bits= IMAX, Low = ALLON
    //    Global Max Current (IMAX) Setting
    //    0000: 10mA 1000: 3.3mA
    //    0001: 20mA 1001: 6.7mA
    //    0010: 30mA 1010: 10mA
    //    0011: 40mA 1011: 13.3mA
    //    0100: 60mA 1100: 20mA
    //    0101: 80mA 1101: 26.7mA
    //    0110: 120mA 1110: 40mA
    //    0111: 160mA 1111: 53.3mA

    setRegister(0x4, 0x1);  // clear all
    setRegister(0x80, 0x2);
    setRegister(0x31, 0x3F);
    setRegister(0x32, 0x3F);
    setRegister(0x33, 0x3F);
    setRegister(0x34, 0x3F);
    setRegister(0x35, 0x3F);
    setRegister(0x36, 0x3F);

    setRegister(0xF0, 0xC1);    // DIM page
    for (int i = 0; i < 36; i++)
        setRegister(i, 0x2f);   // half dimming

    setRegister(0xF0, 0xC2);    // FADE page
    for (int i = 0; i < 36; i++)
        setRegister(i, 0x00);   // off

    setRegister(0xF0, 0xC3);    // Pattern page
    for (int i = 0; i < 36; i++)
        setRegister(i, 0x0);    // Pattern 0. Not enabled in reg 0x43 Page 0.

    setRegister(0xF0, 0xC0);
    for (int i = 0; i < sizeof(page0_of_interest); i++)
        printf("Page 0, Address %d = %d\n", page0_of_interest[i], readRegister(page0_of_interest[i]));
}

void init_leds()
{
    turnoff_AW20036();
    HAL_Delay(10);
    turnon_AW20036();
    HAL_Delay(10);

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

void set_led(uint8_t led, int fade)
{
    leds_page2[led] = fade;
}

void set_rgb(uint8_t position, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t *ptr = &leds_page2[position*3];
    *ptr = r;
    *(++ptr) = g;
    *(++ptr) = b;
}

void start_leds()
{
    write_registers();

    for (int i = 0; i < sizeof(led_reg); i++)
    {
        test_led(led_reg[i], 0xFF);
    }
    HAL_Delay(100);
    for (int i = 0; i < 12; i++)
    {
        set_led(led_reg[i], 0);
    }
    setRegister(0xF0, 0xC1);
    setRegister(0x31, 0x0);
    setRegister(0x32, 0x0);
    setRegister(0x33, 0x0);
    setRegister(0x34, 0x0);
    setRegister(0x35, 0x0);
    setRegister(0x36, 0x0);
    setRegister(0xF0, 0xC2);
    printf("LEDs started\n");
}

void stop_leds()
{
}

uint8_t *rgb_leds(int position)
{
    return &leds_page2[led_reg[ (position-1) * 3]];
}
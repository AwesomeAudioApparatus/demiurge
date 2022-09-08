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

#include "demiurge.h"
#include "demiurge-spi.h"

#define AW20036_ADDR (0x3A << 1)

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
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, AW20036_ADDR, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, regAddress);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, AW20036_ADDR, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
        I2C_AcknowledgeConfig(I2C1, DISABLE);

    uint8_t value = I2C_ReceiveData(I2C1);
    I2C_GenerateSTOP(I2C1, ENABLE);
    return value;
}

static void setRegister(int regAddress, int value)
{
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, AW20036_ADDR, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, regAddress);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C1, value);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
}

static void turnoff_AW20036()
{
    // Turn off AW20036 chip
    GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET);
    Delay_Ms(10);
}

static void turnon_AW20036()
{
    // Turn on AW20036 chip
    GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
    Delay_Ms(10);
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
    setRegister(0x1, 0x3F);
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
    u32 clockspeed = 2000;
    GPIO_InitTypeDef gpio_init = {0};
    I2C_InitTypeDef  i2c_init = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    gpio_init.GPIO_Pin = GPIO_Pin_6;
    gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_init);

    gpio_init.GPIO_Pin = GPIO_Pin_7;
    gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_init);

    gpio_init.GPIO_Pin = GPIO_Pin_8;
    gpio_init.GPIO_Mode = GPIO_Mode_IPD;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_init);

    gpio_init.GPIO_Pin = GPIO_Pin_9;
    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_init);

    i2c_init.I2C_ClockSpeed = clockspeed;
    i2c_init.I2C_Mode = I2C_Mode_I2C;
    i2c_init.I2C_DutyCycle = I2C_DutyCycle_16_9;
    i2c_init.I2C_Ack = I2C_Ack_Enable;
    i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &i2c_init);

    turnon_AW20036();

    I2C_Cmd(I2C1, ENABLE);
    I2C_AcknowledgeConfig(I2C1, ENABLE);

    setRegister(0xF0, 0); // addressing page 0

    printf("Chip ID: %d\n", readRegister(0));
    printf("Sleep Mode Ctrl: %d\n", readRegister(1));
    printf("Global Current Config: %d\n", readRegister(3));
    printf("Clock Ctrl: %d\n", readRegister(5));
    printf("Fault Config 1: %d\n", readRegister(9));
    printf("Fault Config 2: %d\n", readRegister(0xa));
    printf("Interrupt Status: %d\n", readRegister(0xb));
    Delay_Ms(10);
}

void start_leds()
{
    write_registers();
}

void stop_leds()
{
}

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
#include "lfs.h"

#include "demiurge-spi.h"

#define SECTOR_SIZE 256
#define NUMBER_OF_SECTORS 65536

/* Winbond SPIFalsh ID */
#define W25Q80                   0XEF13
#define W25Q16                   0XEF14
#define W25Q32                   0XEF15
#define W25Q64                   0XEF16
#define W25Q128                  0XEF17

/* Winbond SPIFlash Instruction List */
#define W25X_WriteEnable         0x06
#define W25X_WriteDisable        0x04
#define W25X_ReadStatusReg       0x05
#define W25X_WriteStatusReg      0x01
#define W25X_ReadData            0x03
#define W25X_FastReadData        0x0B
#define W25X_FastReadDual        0x3B
#define W25X_PageProgram         0x02
#define W25X_BlockErase          0xD8
#define W25X_SectorErase         0x20
#define W25X_ChipErase           0xC7
#define W25X_PowerDown           0xB9
#define W25X_ReleasePowerDown    0xAB
#define W25X_DeviceID            0xAB
#define W25X_ManufactDeviceID    0x90
#define W25X_JedecDeviceID       0x9F

uint8_t SPI_FLASH_BUF[4096];

static inline void enable_flash() {
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
}

static inline void disable_flash() {
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
}


/*********************************************************************
 * @fn      SPI2_ReadWriteByte
 *
 * @brief   SPI2 read or write one byte.
 *
 * @param   TxData - write one byte data.
 *
 * @return  Read one byte data.
 */
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{
    uint8_t i = 0;

    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
    {
        i++;
        if(i > 2000)
            return 0;
    }

    SPI_I2S_SendData(SPI2, TxData);
    i = 0;

    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {
        i++;
        if(i > 2000)
            return 0;
    }

    return SPI_I2S_ReceiveData(SPI2);
}

/*********************************************************************
 * @fn      SPI_Flash_ReadSR
 *
 * @brief   Read W25Qxx status register.
 *        ����BIT7  6   5   4   3   2   1   0
 *        ����SPR   RV  TB  BP2 BP1 BP0 WEL BUSY
 *
 * @return  byte - status register value.
 */
uint8_t SPI_Flash_ReadSR(void)
{
    uint8_t byte = 0;

    enable_flash();
    SPI2_ReadWriteByte(W25X_ReadStatusReg);
    byte = SPI2_ReadWriteByte(0Xff);
    disable_flash();

    return byte;
}

/*********************************************************************
 * @fn      SPI_FLASH_Write_SR
 *
 * @brief   Write W25Qxx status register.
 *
 * @param   sr - status register value.
 *
 * @return  none
 */
void SPI_FLASH_Write_SR(uint8_t sr)
{
    enable_flash();
    SPI2_ReadWriteByte(W25X_WriteStatusReg);
    SPI2_ReadWriteByte(sr);
    disable_flash();
}

/*********************************************************************
 * @fn      SPI_Flash_Wait_Busy
 *
 * @brief   Wait flash free.
 *
 * @return  none
 */
void SPI_Flash_Wait_Busy(void)
{
    while((SPI_Flash_ReadSR() & 0x01) == 0x01)
        ;
}

/*********************************************************************
 * @fn      SPI_FLASH_Write_Enable
 *
 * @brief   Enable flash write.
 *
 * @return  none
 */
void SPI_FLASH_Write_Enable(void)
{
    enable_flash();
    SPI2_ReadWriteByte(W25X_WriteEnable);
    disable_flash();
}

/*********************************************************************
 * @fn      SPI_FLASH_Write_Disable
 *
 * @brief   Disable flash write.
 *
 * @return  none
 */
void SPI_FLASH_Write_Disable(void)
{
    enable_flash();
    SPI2_ReadWriteByte(W25X_WriteDisable);
    disable_flash();
}

/*********************************************************************
 * @fn      SPI_Flash_ReadID
 *
 * @brief   Read flash ID.
 *
 * @return  Temp - FLASH ID.
 */
uint16_t SPI_Flash_ReadID(void)
{
    uint16_t Temp = 0;

    enable_flash();
    SPI2_ReadWriteByte(W25X_ManufactDeviceID);
    SPI2_ReadWriteByte(0x00);
    SPI2_ReadWriteByte(0x00);
    SPI2_ReadWriteByte(0x00);
    Temp |= SPI2_ReadWriteByte(0xFF) << 8;
    Temp |= SPI2_ReadWriteByte(0xFF);
    disable_flash();

    return Temp;
}

/*********************************************************************
 * @fn      SPI_Flash_Erase_Sector
 *
 * @brief   Erase one sector(4Kbyte).
 *
 * @param   Dst_Addr - 0 ���� 2047
 *
 * @return  none
 */
void SPI_Flash_Erase_Sector(uint32_t Dst_Addr)
{
    Dst_Addr *= 4096;
    SPI_FLASH_Write_Enable();
    SPI_Flash_Wait_Busy();
    enable_flash();
    SPI2_ReadWriteByte(W25X_SectorErase);
    SPI2_ReadWriteByte((uint8_t)((Dst_Addr) >> 16));
    SPI2_ReadWriteByte((uint8_t)((Dst_Addr) >> 8));
    SPI2_ReadWriteByte((uint8_t)Dst_Addr);
    disable_flash();
    SPI_Flash_Wait_Busy();
}

/*********************************************************************
 * @fn      SPI_Flash_Read
 *
 * @brief   Read data from flash.
 *
 * @param   pBuffer -
 *          ReadAddr -Initial address(24bit).
 *          size - Data length.
 *
 * @return  none
 */
void SPI_Flash_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t size)
{
    uint16_t i;

    enable_flash();
    SPI2_ReadWriteByte(W25X_ReadData);
    SPI2_ReadWriteByte((uint8_t)((ReadAddr) >> 16));
    SPI2_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    SPI2_ReadWriteByte((uint8_t)ReadAddr);

    for(i = 0; i < size; i++)
    {
        pBuffer[i] = SPI2_ReadWriteByte(0XFF);
    }

    disable_flash();
}

/*********************************************************************
 * @fn      SPI_Flash_Write_Page
 *
 * @brief   Write data by one page.
 *
 * @param   pBuffer -
 *          WriteAddr - Initial address(24bit).
 *          size - Data length.
 *
 * @return  none
 */
void SPI_Flash_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t size)
{
    uint16_t i;

    SPI_FLASH_Write_Enable();
    enable_flash();
    SPI2_ReadWriteByte(W25X_PageProgram);
    SPI2_ReadWriteByte((uint8_t)((WriteAddr) >> 16));
    SPI2_ReadWriteByte((uint8_t)((WriteAddr) >> 8));
    SPI2_ReadWriteByte((uint8_t)WriteAddr);

    for(i = 0; i < size; i++)
    {
        SPI2_ReadWriteByte(pBuffer[i]);
    }

    disable_flash();
    SPI_Flash_Wait_Busy();
}

/*********************************************************************
 * @fn      SPI_Flash_Write_NoCheck
 *
 * @brief   Write data to flash.(need Erase)
 *          All data in address rang is 0xFF.
 *
 * @param   pBuffer -
 *          WriteAddr - Initial address(24bit).
 *          size - Data length.
 *
 * @return  none
 */
void SPI_Flash_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t size)
{
    uint16_t pageremain;

    pageremain = 256 - WriteAddr % 256;

    if(size <= pageremain)
        pageremain = size;

    while(1)
    {
        SPI_Flash_Write_Page(pBuffer, WriteAddr, pageremain);

        if(size == pageremain)
        {
            break;
        }
        else
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;
            size -= pageremain;

            if(size > 256)
                pageremain = 256;
            else
                pageremain = size;
        }
    }
}

/*********************************************************************
 * @fn      SPI_Flash_Write
 *
 * @brief   Write data to flash.(no need Erase)
 *
 * @param   pBuffer -
 *          WriteAddr - Initial address(24bit).
 *          size - Data length.
 *
 * @return  none
 */
void SPI_Flash_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t size)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;

    secpos = WriteAddr / 4096;
    secoff = WriteAddr % 4096;
    secremain = 4096 - secoff;

    if(size <= secremain)
        secremain = size;

    while(1)
    {
        SPI_Flash_Read(SPI_FLASH_BUF, secpos * 4096, 4096);

        for(i = 0; i < secremain; i++)
        {
            if(SPI_FLASH_BUF[secoff + i] != 0XFF)
                break;
        }

        if(i < secremain)
        {
            SPI_Flash_Erase_Sector(secpos);

            for(i = 0; i < secremain; i++)
            {
                SPI_FLASH_BUF[i + secoff] = pBuffer[i];
            }

            SPI_Flash_Write_NoCheck(SPI_FLASH_BUF, secpos * 4096, 4096);
        }
        else
        {
            SPI_Flash_Write_NoCheck(pBuffer, WriteAddr, secremain);
        }

        if(size == secremain)
        {
            break;
        }
        else
        {
            secpos++;
            secoff = 0;

            pBuffer += secremain;
            WriteAddr += secremain;
            size -= secremain;

            if(size > 4096)
            {
                secremain = 4096;
            }
            else
            {
                secremain = size;
            }
        }
    }
}

/*********************************************************************
 * @fn      SPI_Flash_Erase_Chip
 *
 * @brief   Erase all FLASH pages.
 *
 * @return  none
 */
void SPI_Flash_Erase_Chip(void)
{
    SPI_FLASH_Write_Enable();
    SPI_Flash_Wait_Busy();
    enable_flash();
    SPI2_ReadWriteByte(W25X_ChipErase);
    disable_flash();
    SPI_Flash_Wait_Busy();
}

/*********************************************************************
 * @fn      SPI_Flash_PowerDown
 *
 * @brief   Enter power down mode.
 *
 * @return  none
 */
void SPI_Flash_PowerDown(void)
{
    enable_flash();
    SPI2_ReadWriteByte(W25X_PowerDown);
    disable_flash();
    Delay_Us(3);
}

/*********************************************************************
 * @fn      SPI_Flash_WAKEUP
 *
 * @brief   Power down wake up.
 *
 * @return  none
 */
void SPI_Flash_WAKEUP(void)
{
    enable_flash();
    SPI2_ReadWriteByte(W25X_ReleasePowerDown);
    disable_flash();
    Delay_Us(3);
}

int littlefs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
	printf("Read %d bytes to block %d at %d\n", size, block, off);
    uint32_t readAddr = block * SECTOR_SIZE + off;
    SPI_Flash_Read(buffer, readAddr, size);
    return 0;
}

int littlefs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
	printf("Write %d bytes to block %d at %d\n", size, block, off);
    uint32_t writeAddr = block * SECTOR_SIZE + off;
    SPI_Flash_Write(buffer, writeAddr, size);
    return 0;
}

int littlefs_erase(const struct lfs_config *c, lfs_block_t block)
{
    printf("Erase block %d\n", block );
    SPI_Flash_Erase_Sector(block);
    return 0;
}

int littlefs_sync(const struct lfs_config *c)
{
    return 0;
}

struct lfs_config demiurge_flash_fs_config = {
    // block device operations
    .read  = littlefs_read,
    .prog  = littlefs_prog,
    .erase = littlefs_erase,
    .sync  = littlefs_sync,

    // block device configuration
    .read_size = 256,
    .prog_size = 256,
    .block_size = 4096,
    .block_count = 4096,
    .cache_size = 4096,
    .lookahead_size = 256,
    .block_cycles = 100,
};

lfs_t demiurge_flash_fs;


void init_flash()
{
	Delay_Init();
    printf("Initializing flash disk...\n");

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure = {0};

    // HOLD and WP pins
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_0);
    GPIO_SetBits(GPIOC, GPIO_Pin_1);

    // SPI2 CS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);

    // SPI2 SCK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // SPI2 MISO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // SPI2 MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_InitTypeDef  SPI_InitStructure = {0};

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2, ENABLE);
    printf("Waiting for flash chip...");
    Delay_Ms(10);
    SPI_Flash_Wait_Busy();
    printf("ready\n");
    Delay_Ms(10);
    uint16_t model = SPI_Flash_ReadID();
    printf("Model: %d\n", model);
    Delay_Ms(10);

    int err = lfs_mount(&demiurge_flash_fs, &demiurge_flash_fs_config);
    Delay_Ms(10);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        printf("Formatting flash disk...");
        lfs_format(&demiurge_flash_fs, &demiurge_flash_fs_config);
        lfs_mount(&demiurge_flash_fs, &demiurge_flash_fs_config);
        printf("Done!\n");
    }
    printf("Flash disk initialized.\n");
}

void start_flash()
{
}

void stop_flash()
{
}


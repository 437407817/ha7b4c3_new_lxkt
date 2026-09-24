#ifndef __BSP_QSPI_FLASH_H
#define __BSP_QSPI_FLASH_H
#include "stm32h7xx_hal.h"
#include <stdio.h>

/*========硬件引脚配置 H743 QUADSPI BK1========*/
#define QSPI_FLASH                         QUADSPI
#define QSPI_FLASH_CLK_ENABLE()            __HAL_RCC_QSPI_CLK_ENABLE()

/*========硬件引脚配置 H743 QUADSPI BK1【适配你当前原理图】========*/
#define QSPI_FLASH                         QUADSPI
#define QSPI_FLASH_CLK_ENABLE()            __HAL_RCC_QSPI_CLK_ENABLE()

#define QSPI_FLASH_CLK_GPIO_PORT           GPIOF
#define QSPI_FLASH_CLK_PIN                 GPIO_PIN_10
#define QSPI_FLASH_CLK_GPIO_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_FLASH_CLK_GPIO_AF             GPIO_AF9_QUADSPI

#define QSPI_FLASH_BK1_IO0_PORT            GPIOF
#define QSPI_FLASH_BK1_IO0_PIN             GPIO_PIN_8
#define QSPI_FLASH_BK1_IO0_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_FLASH_BK1_IO0_AF              GPIO_AF10_QUADSPI

#define QSPI_FLASH_BK1_IO1_PORT            GPIOF
#define QSPI_FLASH_BK1_IO1_PIN             GPIO_PIN_9
#define QSPI_FLASH_BK1_IO1_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_FLASH_BK1_IO1_AF              GPIO_AF10_QUADSPI

#define QSPI_FLASH_BK1_IO2_PORT            GPIOF
#define QSPI_FLASH_BK1_IO2_PIN             GPIO_PIN_7
#define QSPI_FLASH_BK1_IO2_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_FLASH_BK1_IO2_AF              GPIO_AF9_QUADSPI

#define QSPI_FLASH_BK1_IO3_PORT            GPIOF
#define QSPI_FLASH_BK1_IO3_PIN             GPIO_PIN_6
#define QSPI_FLASH_BK1_IO3_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_FLASH_BK1_IO3_AF              GPIO_AF9_QUADSPI

#define QSPI_FLASH_CS_GPIO_PORT            GPIOG
#define QSPI_FLASH_CS_PIN                  GPIO_PIN_6
#define QSPI_FLASH_CS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()
#define QSPI_FLASH_CS_GPIO_AF              GPIO_AF10_QUADSPI

#define SFLASH_DOUBLE_CHIP                 0

/*======== W25N01G W25N01GV 硬件参数========*/
#define W25N01G_JEDEC_ID            0xEFAA21U
#define W25N01G_PAGE_SIZE           2048U     /* 页大小 2KB */
#define W25N01G_BLOCK_SIZE          (128U*1024U) /* 块128KB */
#define W25N01G_TOTAL_SIZE          (128U*1024U*1024U) /*1Gbit=128MB*/

/*======== W25N01G 指令集========*/
#define W25N01G_RESET_CMD           0xFFU      /*软件复位*/
#define W25N01G_JEDEC_ID_CMD        0x9FU      /*读JEDEC ID，后面需要Dummy1字节*/
#define W25N01G_SET_FEATURE         0x1FU
#define W25N01G_GET_FEATURE         0x0FU
#define W25N01G_WRITE_ENABLE        0x06U
#define W25N01G_WRITE_DISABLE       0x04U
#define W25N01G_BLOCK_ERASE         0xD8U      /*块擦除*/
#define W25N01G_PAGE_DATA_READ      0x13U      /*页读到内部缓存*/
#define W25N01G_READ_FROM_BUFFER    0x03U      /*从缓存读数据 SPI 1‑line*/
#define W25N01G_LOAD_PROG_DATA      0x02U      /*加载编程数据到缓存*/
#define W25N01G_PROGRAM_EXECUTE     0x10U      /*执行编程，写入NAND页*/

/*寄存器地址*/
#define W25N01G_PROTECTION_REG_ADDR 0xA0U
#define W25N01G_STATUS_REG_ADDR      0xC0U
#define W25N01G_BUSY_MASK           0x01U

typedef enum
{
    FLASH_DEV_UNKNOWN =0,
    FLASH_DEV_W25N01G
}FlashDevType_e;

typedef struct
{
    FlashDevType_e devType;
    uint32_t jedecId;
    uint32_t pageSize;
    uint32_t blockSize;
    uint32_t totalSize;
}QspiFlashDev_t;

extern QSPI_HandleTypeDef QSPIHandle;
extern QspiFlashDev_t g_qspiFlashDev;

void QSPI_FLASH_Init(void);
uint32_t QSPI_W25N01G_ReadJedecID(void);
uint8_t QSPI_W25N01G_Reset(void);
uint8_t QSPI_W25N01G_WriteEnable(void);
uint8_t QSPI_W25N01G_WaitBusy(uint32_t timeout);
uint8_t QSPI_W25N01G_UnlockProtect(void);
uint8_t QSPI_W25N01G_BlockErase(uint32_t blockNum);
uint8_t QSPI_W25N01G_PageReadToCache(uint32_t pageNum);
uint8_t QSPI_W25N01G_ReadFromCache(uint8_t *pBuf,uint16_t colAddr,uint16_t len);
uint8_t QSPI_W25N01G_LoadProgramData(uint8_t *pBuf,uint16_t colAddr,uint16_t len);
uint8_t QSPI_W25N01G_ProgramExecute(uint32_t pageNum);

#endif
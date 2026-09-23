#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H

#include "./stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "./spi/bsp_spi.h"

#define SPI_FLASH_CS_LOW()          SPI_a_FLASH_CS_LOW()
#define SPI_FLASH_CS_HIGH()         SPI_a_FLASH_CS_HIGH()

/* 存储芯片型号 JEDEC ID 定义 -----------------------------------------------*/
/* 存储芯片型号 JEDEC ID 定义 -----------------------------------------------*/
#define W25Q40_JEDEC_ID             0xEF1004  // 新增 W25Q40 / W25X40 JEDEC ID
#define W25Q16_JEDEC_ID             0xEF4015
#define W25Q64_JEDEC_ID             0xEF4017
#define W25Q128_JEDEC_ID            0xEF4018
#define W25Q256_JEDEC_ID            0xEF4019
#define W25N01GV_JEDEC_ID           0xEFAA21

#define FLASH_ID_W25N01GV   0xEFAA21  // W25N01GV 官方正确 ID
//#define W25Q40_FLASH_ID    0xEF1004    // 添加 W25Q40 / W25X40 JEDEC ID
//#define FLASH_ID_W25N01GV   W25N01GV_JEDEC_ID  // W25N01GV (SPI NAND)

/* Flash 介质类型标识 */
typedef enum {
    FLASH_TYPE_UNKNOWN = 0,
    FLASH_TYPE_NOR,
    FLASH_TYPE_NAND
} FlashType_e;

/* Flash 芯片参数结构体 */
typedef struct {
    uint32_t JedecID;       // JEDEC ID
    FlashType_e Type;       // NOR 或 NAND
    uint32_t SectorSize;    // 逻辑擦除/写块单元 (NOR: 4KB, NAND: 128KB)
    uint32_t PageSize;      // 物理页大小 (NOR: 256B, NAND: 2048B)
    uint32_t TotalSize;     // 总容量 (Bytes)
} FlashDevice_t;

/* 常用通用指令集 -----------------------------------------------------------*/
#define W25X_WriteEnable            0x06
#define W25X_WriteDisable           0x04
#define W25X_JedecDeviceID          0x9F
#define Dummy_Byte                  0xFF

/* NOR Flash 专用指令 */
#define W25X_ReadStatusReg          0x05
#define W25X_ReadData               0x03
#define W25X_PageProgram            0x02
#define W25X_SectorErase            0x20
#define W25X_BlockErase             0xD8
#define W25X_ChipErase              0xC7
#define W25X_Enter4ByteMode         0xB7

/* NAND Flash (W25N01GV) 专用指令 */
#define W25N_GET_FEATURE            0x0F
#define W25N_SET_FEATURE            0x1F
#define W25N_PAGE_DATA_READ         0x13
#define W25N_READ_FROM_BUFFER       0x03
#define W25N_LOAD_PROGRAM_DATA      0x02
#define W25N_PROGRAM_EXECUTE        0x10
#define W25N_BLOCK_ERASE            0xD8

#define STAT_REG_PROTECTION_ADDR    0xA0
#define STAT_REG_STATUS_ADDR        0xC0
#define WIP_Flag                    0x01

/* 超时与调试输出 */
#define SPIT_FLAG_TIMEOUT           ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT           ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))
#define FLASH_SPI_ERROR(fmt,arg...) SYSTEM_DEBUG(fmt,##arg)

/* 外部可调用全局变量 */
extern FlashDevice_t g_FlashDev;

/* 通用 API 声明 -----------------------------------------------------------*/
void SPI_FLASH_Init(void);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);

#endif /* __BSP_SPI_FLASH_H */
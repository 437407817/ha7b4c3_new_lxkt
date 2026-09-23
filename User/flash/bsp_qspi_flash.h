#ifndef __BSP_QSPI_FLASH_H
#define __BSP_QSPI_FLASH_H

#include "stm32h7xx.h"
#include <stdio.h>

/* QSPI 接口引脚定义 (严格匹配原理图: CS->PG6, CLK->PF10, IO0->PF8, IO1->PF9, IO2->PF7, IO3->PF6) */
#define QSPI_FLASH                   QUADSPI
#define QSPI_FLASH_CLK_ENABLE()      __HAL_RCC_QSPI_CLK_ENABLE()
#define QSPI_FLASH_CLK_DISABLE()     __HAL_RCC_QSPI_CLK_DISABLE()

#define QSPI_CS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()
#define QSPI_CLK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK1_D0_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK1_D1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK1_D2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define QSPI_BK1_D3_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()

#define QSPI_FLASH_FORCE_RESET()     __HAL_RCC_QSPI_FORCE_RESET()
#define QSPI_FLASH_RELEASE_RESET()   __HAL_RCC_QSPI_RELEASE_RESET()

#define QSPI_CS_PIN                  GPIO_PIN_6
#define QSPI_CS_GPIO_PORT            GPIOG
#define QSPI_CS_GPIO_AF              GPIO_AF10_QUADSPI  /* PG6 必须是 AF10 */

#define QSPI_CLK_PIN                 GPIO_PIN_10
#define QSPI_CLK_GPIO_PORT           GPIOF
#define QSPI_CLK_GPIO_AF             GPIO_AF9_QUADSPI   /* PF10 必须是 AF9 */

#define QSPI_BK1_D0_PIN              GPIO_PIN_8
#define QSPI_BK1_D0_GPIO_PORT        GPIOF
#define QSPI_BK1_D0_GPIO_AF          GPIO_AF9_QUADSPI

#define QSPI_BK1_D1_PIN              GPIO_PIN_9
#define QSPI_BK1_D1_GPIO_PORT        GPIOF
#define QSPI_BK1_D1_GPIO_AF          GPIO_AF9_QUADSPI

#define QSPI_BK1_D2_PIN              GPIO_PIN_7
#define QSPI_BK1_D2_GPIO_PORT        GPIOF
#define QSPI_BK1_D2_GPIO_AF          GPIO_AF9_QUADSPI

#define QSPI_BK1_D3_PIN              GPIO_PIN_6
#define QSPI_BK1_D3_GPIO_PORT        GPIOF
#define QSPI_BK1_D3_GPIO_AF          GPIO_AF9_QUADSPI

/* W25N01GV (1G-bit / 128MB) 结构定义 */
#define QSPI_FLASH_SIZE              26            /* 2^27 Bytes = 128MB */
#define QSPI_PAGE_SIZE               2048          /* 页大小 2KB */
#define QSPI_BLOCK_SIZE              (128 * 1024)  /* 块大小 128KB */

/* W25N01GV 指令集 */
#define RESET_CMD                    0xFF
#define READ_ID_CMD                  0x9F
#define WRITE_ENABLE_CMD             0x06
#define WRITE_DISABLE_CMD            0x04

#define READ_STATUS_REG_CMD          0x0F          
#define WRITE_STATUS_REG_CMD         0x1F          

#define STATUS_REG_PROTECTION        0xA0          
#define STATUS_REG_CONFIGURATION     0xB0          
#define STATUS_REG_STATUS            0xC0          

#define PAGE_DATA_READ_CMD           0x13          
#define FAST_READ_QUAD_OUTPUT_CMD    0xEB          

#define QUAD_IN_PAGE_PROG_CMD        0x32          
#define PROGRAM_EXECUTE_CMD          0x10          

#define BLOCK_ERASE_CMD              0xD8          

/* 状态位定义 */
#define W25N_STATUS_BUSY             0x01          
#define W25N_STATUS_WEL              0x02          

/* 错误码 */
#define QSPI_OK                      0x00
#define QSPI_ERROR                   0x01
#define QSPI_BUSY                    0x02
#define QSPI_TIMEOUT                 0x03

/* 全局句柄 */
extern QSPI_HandleTypeDef QSPIHandle;

/* 函数声明 */
uint8_t BSP_QSPI_Init(void);
uint8_t QSPI_FLASH_Reset(void);
uint8_t QSPI_FLASH_ReadID(uint8_t *id_buf);
uint8_t QSPI_FLASH_ReadStatusReg(uint8_t reg_addr, uint8_t *regvalue);
uint8_t QSPI_FLASH_WriteStatusReg(uint8_t reg_addr, uint8_t regvalue);

#endif /* __BSP_QSPI_FLASH_H */
#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32_FH_xxx.h"
#include <stdio.h>





/* 更改为 SPI1 */
#define SPI_a                             SPI1
#define SPI_a_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPI_a_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI_a_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPI_a_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPI_a_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE() 

/* 根据原理图修正为 GPIOA 引脚 */
#define SPI_a_SCK_PIN                     GPIO_PIN_5
#define SPI_a_SCK_GPIO_PORT               GPIOA
#define SPI_a_SCK_AF                      GPIO_AF5_SPI1

#define SPI_a_MISO_PIN                    GPIO_PIN_6
#define SPI_a_MISO_GPIO_PORT              GPIOA
#define SPI_a_MISO_AF                     GPIO_AF5_SPI1

#define SPI_a_MOSI_PIN                    GPIO_PIN_7
#define SPI_a_MOSI_GPIO_PORT              GPIOA
#define SPI_a_MOSI_AF                     GPIO_AF5_SPI1

#define FLASH_CS_PIN                      GPIO_PIN_4               
#define FLASH_CS_GPIO_PORT                GPIOA    

#define SPI_a_FLASH_CS_LOW()              HAL_GPIO_WritePin(FLASH_CS_GPIO_PORT, FLASH_CS_PIN, GPIO_PIN_RESET)
#define SPI_a_FLASH_CS_HIGH()             HAL_GPIO_WritePin(FLASH_CS_GPIO_PORT, FLASH_CS_PIN, GPIO_PIN_SET)



#define SPI_a_FORCE_RESET()               __HAL_RCC_SPI5_FORCE_RESET()
#define SPI_a_RELEASE_RESET()             __HAL_RCC_SPI5_RELEASE_RESET()

  



#define SPI_a_FLASH_CS_LOW()      HAL_GPIO_WritePin(FLASH_CS_GPIO_PORT, FLASH_CS_PIN, GPIO_PIN_RESET)
#define SPI_a_FLASH_CS_HIGH()     HAL_GPIO_WritePin(FLASH_CS_GPIO_PORT, FLASH_CS_PIN, GPIO_PIN_SET)

void MX_SPI_A_Init(void);

uint8_t SPI_FLASH_SendByte(uint8_t byte);
void SPI_FLASH_SendnByte(uint8_t *pData, uint32_t data_number);
uint8_t SPI_FLASH_ReadByte(void);
void SPI_FLASH_ReadnByte(uint8_t *pData, uint32_t data_number);



#endif /* __SPI_FLASH_H */


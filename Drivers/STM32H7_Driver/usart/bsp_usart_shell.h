#ifndef __BSP_USART_SHELL_H
#define	__BSP_USART_SHELL_H

#include "./stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "./user_config.h"

/* COM define */
#define USE_COM_LOG                         0U
#define USE_BSP_COM_FEATURE                 1U


#define EXCHINGE_UASRT_SHELL_WORKBOARD  1
//串口波特率
#define USART_SHELL_BAUDRATE                    115200


#if !EXCHINGE_UASRT_SHELL_WORKBOARD
/************************ USART1 配置（替换原USART6） ************************/
#define USART_SHELL                             USART1                  // 外设名称改为USART1
#define USART_SHELL_CLK_ENABLE()                __USART1_CLK_ENABLE();  // USART1 外设时钟使能

// USART1 RX引脚：PA10（复用AF7）
#define USART_SHELL_RX_GPIO_PORT                GPIOA                   // RX引脚端口：GPIOA
#define USART_SHELL_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()    // GPIOA 时钟使能
#define USART_SHELL_RX_PIN                      GPIO_PIN_10             // RX引脚：PA10
#define USART_SHELL_RX_AF                       GPIO_AF7_USART1         // RX引脚复用功能：AF7（F429 USART1固定AF7）

// USART1 TX引脚：PA9（复用AF7）
#define USART_SHELL_TX_GPIO_PORT                GPIOA                   // TX引脚端口：GPIOA
#define USART_SHELL_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()    // GPIOA 时钟使能
#define USART_SHELL_TX_PIN                      GPIO_PIN_9              // TX引脚：PA9
#define USART_SHELL_TX_AF                       GPIO_AF7_USART1         // TX引脚复用功能：AF7

// USART1 中断配置
#define USART_SHELL_IRQHandler                  USART1_IRQHandler       // 中断服务函数名
#define USART_SHELL_IRQ                         USART1_IRQn             // 中断向量号

#define USART_SHELL_RCC_PERIPHCLK     					RCC_PERIPHCLK_USART1
#define USART_SHELL_RCC_CLKSOURCE     					RCC_USART16CLKSOURCE_D2PCLK2

#else
#define USART_SHELL                             UART4                  // 外设名称改为USART1
#define USART_SHELL_CLK_ENABLE()                __UART4_CLK_ENABLE();  // USART1 外设时钟使能

// USART1 RX引脚：PA10（复用AF7）
#define USART_SHELL_RX_GPIO_PORT                GPIOD                   // RX引脚端口：GPIOA
#define USART_SHELL_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()    // GPIOA 时钟使能
#define USART_SHELL_RX_PIN                      GPIO_PIN_0             // RX引脚：PA10
#define USART_SHELL_RX_AF                       GPIO_AF8_UART4         // RX引脚复用功能：AF7（F429 USART1固定AF7）

// USART1 TX引脚：PA9（复用AF7）
#define USART_SHELL_TX_GPIO_PORT                GPIOD                   // TX引脚端口：GPIOA
#define USART_SHELL_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()    // GPIOA 时钟使能
#define USART_SHELL_TX_PIN                      GPIO_PIN_1              // TX引脚：PA9
#define USART_SHELL_TX_AF                       GPIO_AF8_UART4         // TX引脚复用功能：AF7

// USART1 中断配置
#define USART_SHELL_IRQHandler                  UART4_IRQHandler       // 中断服务函数名
#define USART_SHELL_IRQ                         UART4_IRQn             // 中断向量号

#define USART_SHELL_RCC_PERIPHCLK     					RCC_PERIPHCLK_UART4
#define USART_SHELL_RCC_CLKSOURCE     					RCC_USART234578CLKSOURCE_D2PCLK1

#endif





extern UART_HandleTypeDef huart_shell_Handle;
//#define  huart_shell  huart_shell_Handle

//#define USE_UART 1
#define TEST_SHELL_UART 0
#define USE_IT_1 1



//void USART_SHELL_Config(void);

//void USART_Config_SHELL(void);
void USART_Shell_ComDrvInit(void);

#if TEST_SHELL_UART
 #if !(USE_LETTER_SHELL)
void HAL_UART_Shell_RxCpltCallback(UART_HandleTypeDef *huart);
#endif
#endif

void USART_Shell_SendChar(uint8_t ch);
uint8_t USART_Shell_ReceiveChar(void);
void USART_Shell_UartDeInit(void);

void Usart_SendString(uint8_t *str);

void Test_USART_SHELL_while(void);


#endif /* __BSP_USART_A_H */

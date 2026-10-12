#ifndef __BSP_USART_SD_H
#define	__BSP_USART_SD_H

#include "stm32_FH_xxx.h"
#include <stdio.h>
#include "./sys/bsp_systime.h"




//#if (EMBED_FIRE_INTERFACE==1)
//#define USART_485_B                             USART2
//#define USART_485_B_CLK                         RCC_APB1Periph_USART2
//#define USART_485_B_BAUDRATE                    115200

//#define USART_485_B_RX_GPIO_PORT                GPIOD
//#define USART_485_B_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
//#define USART_485_B_RX_PIN                      GPIO_Pin_6
//#define USART_485_B_RX_AF                       GPIO_AF_USART2
//#define USART_485_B_RX_SOURCE                   GPIO_PinSource6

//#define USART_485_B_TX_GPIO_PORT                GPIOD
//#define USART_485_B_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
//#define USART_485_B_TX_PIN                      GPIO_Pin_5
//#define USART_485_B_TX_AF                       GPIO_AF_USART2
//#define USART_485_B_TX_SOURCE                   GPIO_PinSource5

//#define _485_B_RE_GPIO_PORT					   GPIOB
//#define _485_B_RE_GPIO_CLK					   RCC_AHB1Periph_GPIOB
//#define _485_B_RE_PIN							   GPIO_Pin_8

//#define _485_B_INT_IRQ                 		   USART2_IRQn
//#define _485_B_IRQHandler                        USART2_IRQHandler
//#else
//#define USART_485_B                             UART5
//#define USART_485_B_CLK                         RCC_APB1Periph_UART5
//#define USART_485_B_BAUDRATE                    9600

//#define USART_485_B_RX_GPIO_PORT                GPIOD
//#define USART_485_B_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
//#define USART_485_B_RX_PIN                      GPIO_Pin_2
//#define USART_485_B_RX_AF                       GPIO_AF_UART5
//#define USART_485_B_RX_SOURCE                   GPIO_PinSource2

//#define USART_485_B_TX_GPIO_PORT                GPIOC
//#define USART_485_B_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
//#define USART_485_B_TX_PIN                      GPIO_Pin_12
//#define USART_485_B_TX_AF                       GPIO_AF_UART5
//#define USART_485_B_TX_SOURCE                   GPIO_PinSource12

//#define _485_B_RE_GPIO_PORT					   GPIOD
//#define _485_B_RE_GPIO_CLK					   RCC_AHB1Periph_GPIOD
//#define _485_B_RE_PIN							   GPIO_Pin_3

//#define _485_B_INT_IRQ                 		   UART5_IRQn
//#define _485_B_IRQHandler                        UART5_IRQHandler
//#endif





	/// 不精确的延时

/*控制收发引脚*/
//进入接收模式,必须要有延时等待485处理完数据
//#define _485_B_RX_EN()			 GPIO_ResetBits(_485_B_RE_GPIO_PORT,_485_B_RE_PIN); //DelayNclock(200);
////进入发送模式,必须要有延时等待485处理完数据
//#define _485_B_TX_EN()			 GPIO_SetBits(_485_B_RE_GPIO_PORT,_485_B_RE_PIN);  DelayNclock(200);//Delay(1);



void USART_485_B_Config(void);

//void _485_SendByte(  uint8_t ch );
//void _485_SendStr_length( uint8_t *str,uint32_t strlen );
//void _485_SendString(  uint8_t *str);

void bsp_485_B_IRQHandler(void);

void sd_usart_cmd_Option(void);

void sd_usart_cmd_SetTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);

void sd_usart_cmd_SetItem(void);
char* formatDateTitle(char* time,char* workmode);
void sd_usart_cmd_SetTitle(char* time,char* workmode);
void sd_usart_cmd_ReadData(void);

void sd_usart_cmd_ReadTime(void);

void sd_usart_cmd_SetTitle_bynum(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second,char* workmode);



char* formatDateTimeToString(uint8_t hour, uint8_t minute, uint8_t second);
void sd_usart_cmd_SetTitle_bystr(char* workmode);
char* formatSD_Date(char* runtime,char* workmode,uint8_t batgroup,uint16_t* workdata,uint8_t showmode);
void sd_usart_cmd_SetData(char* runtime,char* workmode,uint8_t batgroup,uint16_t* workdata,uint8_t showmode);

#endif /* __USART1_H */

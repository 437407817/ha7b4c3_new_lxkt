#ifndef __BSP_USART_COMMON_COM485_H
#define	__BSP_USART_COMMON_COM485_H

#include "./stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "./user_config.h"

#include "./usart/bsp_usart_shell.h"



//串口波特率
#define USART_COM01_COM485_BAUDRATE                    115200


/************************ USART1 配置（替换原USART6） ************************/
#define USART_COM01_COM485                             USART2
#define USART_COM01_COM485_CLK_ENABLE()                __USART2_CLK_ENABLE();
			  
#define USART_COM01_COM485_RX_GPIO_PORT                GPIOD
#define USART_COM01_COM485_RX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()
#define USART_COM01_COM485_RX_PIN                      GPIO_PIN_6
#define USART_COM01_COM485_RX_AF                       GPIO_AF7_USART2
			  
#define USART_COM01_COM485_TX_GPIO_PORT                GPIOD
#define USART_COM01_COM485_TX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()
#define USART_COM01_COM485_TX_PIN                      GPIO_PIN_5
#define USART_COM01_COM485_TX_AF                       GPIO_AF7_USART2
			  
#define USART_COM01_COM485_IRQHandler                  USART2_IRQHandler
#define USART_COM01_COM485_IRQ                 		    USART2_IRQn


#define USART_COM01_COM485_RCC_PERIPHCLK                       RCC_PERIPHCLK_USART2
#define USART_COM01_COM485_RCC_CLKSOURCE                       RCC_USART234578CLKSOURCE_D2PCLK1


#define USE_UART_RX_COMMON_DMA 			0
#define USE_COM01_COM485_IT_1				1


//#define USE_LVGL 0


#if 0
//引脚定义
/*******************************************************/
#define USART_COM485                             USART6
#define USART_COM485_CLK_ENABLE()                __USART6_CLK_ENABLE();
			  
#define USART_COM485_RX_GPIO_PORT                GPIOC
#define USART_COM485_RX_GPIO_CLK_ENABLE()        __GPIOC_CLK_ENABLE()
#define USART_COM485_RX_PIN                      GPIO_PIN_7
#define USART_COM485_RX_AF                       GPIO_AF8_USART6
			  
#define USART_COM485_TX_GPIO_PORT                GPIOC
#define USART_COM485_TX_GPIO_CLK_ENABLE()        __GPIOC_CLK_ENABLE()
#define USART_COM485_TX_PIN                      GPIO_PIN_6
#define USART_COM485_TX_AF                       GPIO_AF8_USART6
			  
#define USART_COM485_IRQHandler                  USART6_IRQHandler
#define USART_COM485_IRQ                 		    USART6_IRQn
/************************************************************/
//DMA
//#define SENDBUFF_SIZE                     		10//发送的数据量
//#define DEBUG_USART_COM485_DMA_CLK_ENABLE()      		__DMA2_CLK_ENABLE()	
//#define DEBUG_USART_COM485_DMA_CHANNEL           		DMA_CHANNEL_4
//#define DEBUG_USART_COM485_DMA_STREAM            		DMA2_Stream7
#endif
/************************ USART2 配置（PD5 TX / PD6 RX） ************************/
//#define USART_COM485                             USART2                  // 外设名称改为USART2
//#define USART_COM485_CLK_ENABLE()                __USART2_CLK_ENABLE();  // USART2 外设时钟使能

//// USART2 RX 引脚：PD6（复用AF7）
//#define USART_COM485_RX_GPIO_PORT                GPIOD                   // RX引脚端口：GPIOD
//#define USART_COM485_RX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()    // GPIOD 时钟使能
//#define USART_COM485_RX_PIN                      GPIO_PIN_6              // RX引脚：PD6
//#define USART_COM485_RX_AF                       GPIO_AF7_USART2         // RX引脚复用功能：AF7（F429 USART2 固定AF7）

//// USART2 TX 引脚：PD5（复用AF7）
//#define USART_COM485_TX_GPIO_PORT                GPIOD                   // TX引脚端口：GPIOD
//#define USART_COM485_TX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()    // GPIOD 时钟使能
//#define USART_COM485_TX_PIN                      GPIO_PIN_5              // TX引脚：PD5
//#define USART_COM485_TX_AF                       GPIO_AF7_USART2         // TX引脚复用功能：AF7

//// USART2 中断配置
//#define USART_COM485_IRQHandler                  USART2_IRQHandler       // 中断服务函数名
//#define USART_COM485_IRQ                         USART2_IRQn             // 中断向量号
//DMA
//#define SENDBUFF_SIZE                     		10//发送的数据量
//#define DEBUG_USART_COM485_DMA_CLK_ENABLE()      		__DMA2_CLK_ENABLE()	
//#define DEBUG_USART_COM485_DMA_CHANNEL           		DMA_CHANNEL_4
//#define DEBUG_USART_COM485_DMA_STREAM            		DMA2_Stream7
//前向声明
typedef struct tag_UartComInstance UartComInstance;
// 定义串口发送函数指针类型
typedef void (*UsartComSendPtr)(UartComInstance *, uint8_t *, uint16_t);

typedef struct
{
    // 带参数发送函数指针
    void (*U485ComSendDmaSaveDataFunc)(char *buf, uint16_t num);
    // 无参数发送函数指针
    void (*U485ComSendAllFunc)(UartComInstance *pInst, uint8_t *array, uint16_t num);;

    // 保存传入的参数，调用的时候用
//    char *pBuf;
//    uint16_t bufLen;
}U485ComUsartSend_Callback_t;


struct tag_UartComInstance
{
    UART_HandleTypeDef *huart_handle;
    U485ComUsartSend_Callback_t sendCbStore;   //每个实例自带存储
};


//extern UART_HandleTypeDef huart_COM01_COM485_Handle;

extern UartComInstance com01_com485Inst;



//(UartComInstance *pInst, uint8_t *array, uint16_t num)





#define DISENABLE_IDLE_IT_STATUS (0)
#define ENABLE_IDLE_IT_STATUS (1)

#define DISENABLE_RXNE_IT_STATUS (0)
#define ENABLE_RXNE_IT_STATUS (1)



#define _485_A_TX_EN()  while(0){}
#define _485_A_RX_EN() while(0){}

//extern UART_HandleTypeDef huart_COM485_Handle;
//extern 	UsartSendPtr this_com485_Usart_Send;
////#define  huart_COM485  huart_COM485_Handle

//#define USE_UART 1
#define TEST_COM485_UART 1
#define USE_COM485_IT_1 1

#if USE_UART_COMMON_DMA_TX	
#define USE_COM485_DMA_SEND 1 
#else
#define USE_COM485_DMA_SEND 0  // 配置开关：1为DMA模式，0为阻塞模式
#endif
	
	
	
//	void UART_COMMON_Instance_SendArray(UartComInstance *pInst, uint8_t *array, uint16_t num);
//void UART_COMMON_Instance_SendArray_DMA(UartComInstance *pInst, uint8_t *array, uint16_t num);
	void USART_COMMON_COM485_232_ComDrvInit(void);
	
//void Usart_COMMON_COM485_send_Config_Init(void);
	
	void UART_COMMON_Instance_SetSendCallback(UartComInstance *pInst, const U485ComUsartSend_Callback_t *pSrcCb);
	
void UART_COMMON_Instance_SendArray_DMA(UartComInstance *pInst, uint8_t *array, uint16_t num);

void UART_COMMON_Instance_SendArray(UartComInstance *pInst, uint8_t *array, uint16_t num);

void UART_COMMON_SendAllFunc(void);

#endif /* __BSP_USART_A_H */

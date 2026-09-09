/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.h
  * @brief   This file contains all the function prototypes for
  *          the dma.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software COM0ponent.
  * If no LICENSE file COM0es with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_USART_COMMON_DMA_H__
#define __BSP_USART_COMMON_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_FH_xxx_hal.h"
#include <stdio.h>
#include "./user_config.h"
/* DMA memory to memory transfer handles -------------------------------------*/


/* USER CODE BEGIN Includes */
 #include "./buffer/p_data_queue_outer.h"
 #include "./buffer/queue3.h"
 #include "./pro_COM/usart485verify.h"
 
 #include "./usart/bsp_usart_dma.h"
 #include "./usart/bsp_usart_COMMON_COM485.h"
//#include "./usart/p_data_queue.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

//#define TX_COM0PLETE_TIMEOUT 3000 
//#define TX_WAITTING_TIMEOUT 5000  


#define TX_WAITTING_TIMEOUT 1  

//#define MAX_BUF_R_SIZE        5
////#define MAX_BUF_R_SIZE        PACKET_DATA_LEN_MAX * 3 //防止2帧重叠
//#define MAX_RING_BUFF_SIZE MAX_BUF_R_SIZE*8
////#define MAX_RING_BUFF_SIZE    MAX_BUF_SIZE

#define MAX_BUF_COM01_R_SIZE 					5  //如果用SCB_CleanInvalidateDCache_by_Addr，需要MAX_BUF_R_SIZE比接收数据最大值大1
#define MAX_RING_BUFF_COM01_SIZE 			MAX_BUF_COM01_R_SIZE*10


#define DMA_COM01_USARTx 														USART_COM01_COM485
#define USARTx_DMA_COM01_IRQHandler                 USART_COM01_COM485_IRQHandler
#define USARTx_DMA_COM01_IRQ                 		    USART_COM01_COM485_IRQ
#define huart_DMA_COM01_Handle                      huart_COM01_COM485_Handle 

#define DMA_COM01_STREAMx_TX 														DMA1_Stream1
#define DMA_COM01_STREAM_IRQ_TX 												DMA1_Stream1_IRQn
#define DMA_COM01_REQUEST_USART_TX 											USART_COM01_DMA_REQUEST_USART_TX

#define DMA_COM01_tx_Streamx_IRQHandler							DMA1_Stream1_IRQHandler


#define DMA_COM01_STREAMx_RX 														DMA2_Stream1
#define DMA_COM01_STREAM_IRQ_RX 												DMA2_Stream1_IRQn
#define DMA_COM01_REQUEST_USART_RX 											USART_COM01_DMA_REQUEST_USART_RX

#define DMA_COM01_rx_Streamx_IRQHandler							DMA2_Stream1_IRQHandler



#define USE_UART_COMMON_COM01_DMA 1
#define USE_UART_COMMON_COM01_DMA_RX 0
#define USE_UART_COMMON_COM01_DMA_TX 1


//*********************************





#define MAX_BUF_COM02_R_SIZE 					5  //如果用SCB_CleanInvalidateDCache_by_Addr，需要MAX_BUF_R_SIZE比接收数据最大值大1
#define MAX_RING_BUFF_COM02_SIZE 			MAX_BUF_COM02_R_SIZE*10


#define DMA_COM02_USARTx 														USART_COM02_COM485
#define USARTx_DMA_COM02_IRQHandler                 USART_COM02_COM485_IRQHandler
#define USARTx_DMA_COM02_IRQ                 		    USART_COM02_COM485_IRQ
#define huart_DMA_COM02_Handle                      huart_COM02_COM485_Handle 

#define DMA_COM02_STREAMx_TX 														DMA1_Stream2
#define DMA_COM02_STREAM_IRQ_TX 												DMA1_Stream2_IRQn
#define DMA_COM02_REQUEST_USART_TX 											USART_COM02_DMA_REQUEST_USART_TX

#define DMA_COM02_tx_Streamx_IRQHandler							DMA1_Stream2_IRQHandler


#define DMA_COM02_STREAMx_RX 														DMA2_Stream2
#define DMA_COM02_STREAM_IRQ_RX 												DMA2_Stream2_IRQn
#define DMA_COM02_REQUEST_USART_RX 											USART_COM02_DMA_REQUEST_USART_RX

#define DMA_COM02_rx_Streamx_IRQHandler							DMA2_Stream2_IRQHandler


#define USE_UART_COMMON_COM02_DMA 1
#define USE_UART_COMMON_COM02_DMA_RX 1
#define USE_UART_COMMON_COM02_DMA_TX 1


//*********************************


#define MAX_BUF_COM03_R_SIZE 					5  //如果用SCB_CleanInvalidateDCache_by_Addr，需要MAX_BUF_R_SIZE比接收数据最大值大1
#define MAX_RING_BUFF_COM03_SIZE 			MAX_BUF_COM03_R_SIZE*10


typedef struct
{
    STR_RCV_DMA_que_data obj;
    uint8_t ringMem[MAX_RING_BUFF_COM03_SIZE];
    uint8_t rcvMem[MAX_BUF_COM03_R_SIZE];
} STR_RCV_DMA_que_data_COM03_t;


#define DMA_COM03_USARTx 														USART_COM03_COM485
#define USARTx_DMA_COM03_IRQHandler                 USART_COM03_COM485_IRQHandler
#define USARTx_DMA_COM03_IRQ                 		    USART_COM03_COM485_IRQ
#define huart_DMA_COM03_Handle                      huart_COM03_COM485_Handle 

#define DMA_COM03_STREAMx_TX 														DMA1_Stream3
#define DMA_COM03_STREAM_IRQ_TX 												DMA1_Stream3_IRQn
#define DMA_COM03_REQUEST_USART_TX 											USART_COM03_DMA_REQUEST_USART_TX

#define DMA_COM03_tx_Streamx_IRQHandler							DMA1_Stream3_IRQHandler


#define DMA_COM03_STREAMx_RX 														DMA2_Stream3
#define DMA_COM03_STREAM_IRQ_RX 												DMA2_Stream3_IRQn
#define DMA_COM03_REQUEST_USART_RX 											USART_COM03_DMA_REQUEST_USART_RX

#define DMA_COM03_rx_Streamx_IRQHandler							DMA2_Stream3_IRQHandler



#define USE_UART_COMMON_COM03_DMA 1
#define USE_UART_COMMON_COM03_DMA_RX 1
#define USE_UART_COMMON_COM03_DMA_TX 1






#define MAX_BUF_COM04_R_SIZE 					5  //如果用SCB_CleanInvalidateDCache_by_Addr，需要MAX_BUF_R_SIZE比接收数据最大值大1
#define MAX_RING_BUFF_COM04_SIZE 			MAX_BUF_COM04_R_SIZE*10



#define DMA_COM04_USARTx 														USART_COM04_COM485
#define USARTx_DMA_COM04_IRQHandler                 USART_COM04_COM485_IRQHandler
#define USARTx_DMA_COM04_IRQ                 		    USART_COM04_COM485_IRQ
#define huart_DMA_COM04_Handle                      huart_COM04_COM485_Handle 

#define DMA_COM04_STREAMx_TX 														DMA1_Stream4
#define DMA_COM04_STREAM_IRQ_TX 												DMA1_Stream4_IRQn
#define DMA_COM04_REQUEST_USART_TX 											USART_COM04_DMA_REQUEST_USART_TX

#define DMA_COM04_tx_Streamx_IRQHandler							DMA1_Stream4_IRQHandler


#define DMA_COM04_STREAMx_RX 														DMA2_Stream4
#define DMA_COM04_STREAM_IRQ_RX 												DMA2_Stream4_IRQn
#define DMA_COM04_REQUEST_USART_RX 											USART_COM04_DMA_REQUEST_USART_RX

#define DMA_COM04_rx_Streamx_IRQHandler							DMA2_Stream4_IRQHandler



#define USE_UART_COMMON_COM04_DMA 1
#define USE_UART_COMMON_COM04_DMA_RX 1
#define USE_UART_COMMON_COM04_DMA_TX 1





#define MAX_BUF_COM05_R_SIZE 					5  //如果用SCB_CleanInvalidateDCache_by_Addr，需要MAX_BUF_R_SIZE比接收数据最大值大1
#define MAX_RING_BUFF_COM05_SIZE 			MAX_BUF_COM05_R_SIZE*10



#define DMA_COM05_USARTx 														USART_COM05_COM485
#define USARTx_DMA_COM05_IRQHandler                 USART_COM05_COM485_IRQHandler
#define USARTx_DMA_COM05_IRQ                 		    USART_COM05_COM485_IRQ
#define huart_DMA_COM05_Handle                      huart_COM05_COM485_Handle 

#define DMA_COM05_STREAMx_TX 														DMA1_Stream5
#define DMA_COM05_STREAM_IRQ_TX 												DMA1_Stream5_IRQn
#define DMA_COM05_REQUEST_USART_TX 											USART_COM05_DMA_REQUEST_USART_TX

#define DMA_COM05_tx_Streamx_IRQHandler							DMA1_Stream5_IRQHandler


#define DMA_COM05_STREAMx_RX 														DMA2_Stream5
#define DMA_COM05_STREAM_IRQ_RX 												DMA2_Stream5_IRQn
#define DMA_COM05_REQUEST_USART_RX 											USART_COM05_DMA_REQUEST_USART_RX

#define DMA_COM05_rx_Streamx_IRQHandler							DMA2_Stream5_IRQHandler



#define USE_UART_COMMON_COM05_DMA 1
#define USE_UART_COMMON_COM05_DMA_RX 1
#define USE_UART_COMMON_COM05_DMA_TX 1




#define MAX_BUF_COM06_R_SIZE 					5  //如果用SCB_CleanInvalidateDCache_by_Addr，需要MAX_BUF_R_SIZE比接收数据最大值大1
#define MAX_RING_BUFF_COM06_SIZE 			MAX_BUF_COM06_R_SIZE*10



#define DMA_COM06_USARTx 														USART_COM06_COM485
#define USARTx_DMA_COM06_IRQHandler                 USART_COM06_COM485_IRQHandler
#define USARTx_DMA_COM06_IRQ                 		    USART_COM06_COM485_IRQ
#define huart_DMA_COM06_Handle                      huart_COM06_COM485_Handle 

#define DMA_COM06_STREAMx_TX 														DMA1_Stream6
#define DMA_COM06_STREAM_IRQ_TX 												DMA1_Stream6_IRQn
#define DMA_COM06_REQUEST_USART_TX 											USART_COM06_DMA_REQUEST_USART_TX

#define DMA_COM06_tx_Streamx_IRQHandler							DMA1_Stream6_IRQHandler


#define DMA_COM06_STREAMx_RX 														DMA2_Stream6
#define DMA_COM06_STREAM_IRQ_RX 												DMA2_Stream6_IRQn
#define DMA_COM06_REQUEST_USART_RX 											USART_COM06_DMA_REQUEST_USART_RX

#define DMA_COM06_rx_Streamx_IRQHandler							DMA2_Stream6_IRQHandler



#define USE_UART_COMMON_COM06_DMA 1
#define USE_UART_COMMON_COM06_DMA_RX 1
#define USE_UART_COMMON_COM06_DMA_TX 1














//typedef struct
//{
//    STR_RCV_DMA_que_data obj;
//    uint8_t ringMem[MAX_RING_BUFF_COM01_SIZE];
//    uint8_t rcvMem[MAX_BUF_COM01_R_SIZE];
//} STR_RCV_DMA_que_data_COM01_t;

//typedef struct
//{
//    STR_RCV_DMA_que_data obj;
//    uint8_t ringMem[MAX_RING_BUFF_COM02_SIZE];
//    uint8_t rcvMem[MAX_BUF_COM02_R_SIZE];
//} STR_RCV_DMA_que_data_COM02_t;


//typedef struct
//{
//    STR_RCV_DMA_que_data obj;
//    uint8_t ringMem[MAX_RING_BUFF_COM03_SIZE];
//    uint8_t rcvMem[MAX_BUF_COM03_R_SIZE];
//} STR_RCV_DMA_que_data_COM03_t;

//typedef struct
//{
//    STR_RCV_DMA_que_data obj;
//    uint8_t ringMem[MAX_RING_BUFF_COM04_SIZE];
//    uint8_t rcvMem[MAX_BUF_COM04_R_SIZE];
//} STR_RCV_DMA_que_data_COM04_t;


//typedef struct
//{
//    STR_RCV_DMA_que_data obj;
//    uint8_t ringMem[MAX_RING_BUFF_COM05_SIZE];
//    uint8_t rcvMem[MAX_BUF_COM05_R_SIZE];
//} STR_RCV_DMA_que_data_COM05_t;

//typedef struct
//{
//    STR_RCV_DMA_que_data obj;
//    uint8_t ringMem[MAX_RING_BUFF_COM06_SIZE];
//    uint8_t rcvMem[MAX_BUF_COM06_R_SIZE];
//} STR_RCV_DMA_que_data_COM06_t;






#include "./usart/bsp_usart_COM485.h"
//#define DMA_USARTx 														USART_COM485
//#define USARTx_DMA_IRQHandler                 USART_COM485_IRQHandler
//#define USARTx_DMA_IRQ                 		    USART_COM485_IRQ
//#define huart_DMA_Handle 												huart_COM485_Handle



#define TESTUsartCOMMONDMASendSaveAndSend 1

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */





extern STR_RCV_DMA_que_data RcvDmaQueData;

extern str_DMA_usart_send GV_usartdmaCOMMON_COM01_Send;
extern str_DMA_usart_send GV_usartdmaCOMMON_COM02_Send;
extern str_DMA_usart_send GV_usartdmaCOMMON_COM03_Send;
extern str_DMA_usart_send GV_usartdmaCOMMON_COM04_Send;

extern str_DMA_usart_send GV_usartdmaCOMMON_COM05_Send;
extern str_DMA_usart_send GV_usartdmaCOMMON_COM06_Send;



void USART_COMMON_TX_RX_DMA_ConfigALL(void);
void Usart_COMMON_DMA_SendFUN_ALL(void);


void HAL_UART_COMMON_TxCpltCallback(UART_HandleTypeDef *huart);




#ifdef __cplusplus
}
#endif

#endif /* __DMA_H__ */


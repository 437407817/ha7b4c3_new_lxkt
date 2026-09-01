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
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
 #include "./pro_com/usart485verify.h"
 
 #include "./usart/bsp_usart_dma.h"
 
//#include "./usart/p_data_queue.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

//#define TX_COMPLETE_TIMEOUT 3000 
//#define TX_WAITTING_TIMEOUT 5000  


#define TX_WAITTING_TIMEOUT 1  

//#define MAX_BUF_R_SIZE        5
////#define MAX_BUF_R_SIZE        PACKET_DATA_LEN_MAX * 3 //防止2帧重叠
//#define MAX_RING_BUFF_SIZE MAX_BUF_R_SIZE*8
////#define MAX_RING_BUFF_SIZE    MAX_BUF_SIZE

#define MAX_BUF_COM01_R_SIZE 					5  //如果用SCB_CleanInvalidateDCache_by_Addr，需要MAX_BUF_R_SIZE比接收数据最大值大1
#define MAX_RING_BUFF_COM01_SIZE 			MAX_BUF_COM01_R_SIZE*10



#define DMA_COM01_USARTx 														USART8
#define USARTx_DMA_COM01_IRQHandler                 USART1_IRQHandler
#define USARTx_DMA_COM01_IRQ                 		    USART1_IRQn
#define huart_DMA_COM01_Handle                      huart_shell_Handle 

#define DMA_COM01_STREAMx_TX 														DMA1_Stream0
#define DMA_COM01_STREAM_IRQ_TX 												DMA1_Stream0_IRQn
#define DMA_COM01_REQUEST_USART_TX 											DMA_REQUEST_USART1_TX

#define DMA_COM01_STREAMx_RX 														DMA1_Stream1
#define DMA_COM01_STREAM_IRQ_RX 												DMA1_Stream1_IRQn
#define DMA_COM01_REQUEST_USART_RX 											DMA_REQUEST_USART1_RX






#define DMA_COM02_USARTx 														USART5
#define USARTx_DMA_COM02_IRQHandler                 USART1_IRQHandler
#define USARTx_DMA_COM02_IRQ                 		    USART1_IRQn
#define huart_DMA_COM02_Handle                      huart_shell_Handle 

#define DMA_COM02_STREAMx_TX 														DMA1_Stream0
#define DMA_COM02_STREAM_IRQ_TX 												DMA1_Stream0_IRQn
#define DMA_COM02_REQUEST_USART_TX 											DMA_REQUEST_USART1_TX

#define DMA_COM02_STREAMx_RX 														DMA1_Stream1
#define DMA_COM02_STREAM_IRQ_RX 												DMA1_Stream1_IRQn
#define DMA_COM02_REQUEST_USART_RX 											DMA_REQUEST_USART1_RX



#define DMA_COM03_USARTx 														USART5
#define USARTx_DMA_COM03_IRQHandler                 USART1_IRQHandler
#define USARTx_DMA_COM03_IRQ                 		    USART1_IRQn
#define huart_DMA_COM03_Handle                      huart_shell_Handle 

#define DMA_COM03_STREAMx_TX 														DMA1_Stream0
#define DMA_COM03_STREAM_IRQ_TX 												DMA1_Stream0_IRQn
#define DMA_COM03_REQUEST_USART_TX 											DMA_REQUEST_USART1_TX

#define DMA_COM03_STREAMx_RX 														DMA1_Stream1
#define DMA_COM03_STREAM_IRQ_RX 												DMA1_Stream1_IRQn
#define DMA_COM03_REQUEST_USART_RX 											DMA_REQUEST_USART1_RX


#define DMA_COM04_USARTx 														USART5
#define USARTx_DMA_COM04_IRQHandler                 USART1_IRQHandler
#define USARTx_DMA_COM04_IRQ                 		    USART1_IRQn
#define huart_DMA_COM04_Handle                      huart_shell_Handle 

#define DMA_COM04_STREAMx_TX 														DMA1_Stream0
#define DMA_COM04_STREAM_IRQ_TX 												DMA1_Stream0_IRQn
#define DMA_COM04_REQUEST_USART_TX 											DMA_REQUEST_USART1_TX

#define DMA_COM04_STREAMx_RX 														DMA1_Stream1
#define DMA_COM04_STREAM_IRQ_RX 												DMA1_Stream1_IRQn
#define DMA_COM04_REQUEST_USART_RX 											DMA_REQUEST_USART1_RX




#include "./usart/bsp_usart_COM485.h"
//#define DMA_USARTx 														USART_COM485
//#define USARTx_DMA_IRQHandler                 USART_COM485_IRQHandler
//#define USARTx_DMA_IRQ                 		    USART_COM485_IRQ
//#define huart_DMA_Handle 												huart_COM485_Handle

#define USE_UART_COMMON_DMA 1
#define USE_UART_COMMON_DMA_RX 1
//#define USE_UART_DMA_TX 1



/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */





extern STR_RCV_DMA_que_data RcvDmaQueData;








//void HAL_USARTx_DMA_RxCpltCallback(void);
//void HAL_USARTx_DMA_TxCpltCallback(void);
//void HAL_USARTx_DMA_ErrorCallback(void);

//void USART_RX_DMA_Config(UART_HandleTypeDef* uartHandle);
//void USART_RX_DMA_Config(UART_HandleTypeDef* uartHandle);
//void Usart_SendFUN_ALL(void);


//void TEST_USART_RX_DMA_SEGGERprint(void);

//void Usart_SendDMA_SaveFun(char *Sendbuff,uint16_t buff_len);

//void USART_TX_RX_DMA_Config(UART_HandleTypeDef* uartHandle);
//void USART_TX_RX_DMA_ConfigALL(void);


//#if TESTUsartDMASendSaveAndSend
//void Test_Usart_DMA_SendSaveFun(void);
//void TEST_Usart_SendFUN_ALL(void);
//#endif

//#if TEST_DMA_RB_READ
//void TEST_USART_RX_DMA_ALL(void);
//void TEST_USART_RX_DMA_ALL_while(void);
//#endif

//#if TEST_RingBuffer_SAVEANDREAD
//void Test_Usart_QueueSaveFun(void);
//#endif





#ifdef __cplusplus
}
#endif

#endif /* __DMA_H__ */


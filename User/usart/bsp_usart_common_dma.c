/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
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

/* Includes ------------------------------------------------------------------*/

#include "./usart/bsp_usart_common_dma.h"
#include "./sys/sysio.h"

#define DEBUG_DmaUsartPrint 0

//  #include "./usart/bsp_usart.h"
/* USER CODE BEGIN 0 */
DMA_HandleTypeDef hdma_usartx_COM1_rx;
DMA_HandleTypeDef hdma_usartx_COM1_tx;

DMA_HandleTypeDef hdma_usartx_COM2_rx;
DMA_HandleTypeDef hdma_usartx_COM2_tx;




//extern UART_HandleTypeDef huart_DMA_Handle;

STR_RCV_DMA_que_data RcvDmaQue_COM1_Data={0};

STR_RCV_DMA_que_data RcvDmaQue_COM2_Data={0};




// 修改缓冲区定义，增加对齐属性
//void USART_RX_DMA_SEGGERprint_OneByte(void);

	#if USE_OS
#include "FreeRTOS.h"	
#include "semphr.h"
extern SemaphoreHandle_t g_com_uart_send_sem;
//extern SemaphoreHandle_t g_com_nextdata_send_sem;
extern __IO uint32_t NowUse_rtos;
	#endif

str_DMA_usart_send GV_usartdmaCOMMON_COM1_Send = {
    .send_data = {0},  // 数组初始化为全0（字符串结束符+无效数据清0）
    .uart_tx_justSaveOver = 0,  // 初始化为未保存完成
    .uart_tx_thisdatas_sendover = 0,      // 初始化为未发送完成
    .read_out_len = 0,          // 初始读取长度为0
    .daret = 0,                 // 按需初始化（根据实际用途设值）
    .complete_timeout = TX_WAITTING_TIMEOUT,   // 超时阈值3000ms（可按需调整）
    .current_time = 0,          // 初始时间设0（后续用HAL_GetTick()更新）
    .last_tx_complete_time = 0  // 初始无上次触发时间
};
str_DMA_usart_send GV_usartdmaCOMMON_COM2_Send = {
    .send_data = {0},  // 数组初始化为全0（字符串结束符+无效数据清0）
    .uart_tx_justSaveOver = 0,  // 初始化为未保存完成
    .uart_tx_thisdatas_sendover = 0,      // 初始化为未发送完成
    .read_out_len = 0,          // 初始读取长度为0
    .daret = 0,                 // 按需初始化（根据实际用途设值）
    .complete_timeout = TX_WAITTING_TIMEOUT,   // 超时阈值3000ms（可按需调整）
    .current_time = 0,          // 初始时间设0（后续用HAL_GetTick()更新）
    .last_tx_complete_time = 0  // 初始无上次触发时间
};


//static uint8_t g_rcvDataBuf[MAX_BUF_R_SIZE];
//static uint8_t received_data_len = 0;

// 定义环形缓冲区大小（根据需求调整，此处设为128字节）


void USARTx_COMMON_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 2, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
//  /* DMA2_Stream7_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 2, 0);
//  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/**
 * @brief  USART TX DMA通用配置函数
 * @param  uartHandle:    串口句柄指针 &huart1 / &huart4
 * @param  dmaInstance:   DMA流实例，如 DMA1_Stream0
 * @param  dmaIrq:        DMA中断号，如 DMA1_Stream0_IRQn
 * @param  dmaRequest:    DMA请求ID，DMA_REQUEST_USART1_TX / DMA_REQUEST_UART4_TX
 * @note   hdma_usartx_COM1_tx 调用外部DMA句柄变量，调用前注意DeInit释放
 */
void USART_TX_DMA_COMMON_Config(UART_HandleTypeDef* uartHandle,
                         DMA_Stream_TypeDef* dmaInstance,
                         IRQn_Type dmaIrq,
                         uint32_t dmaRequest,
                         DMA_HandleTypeDef* hdma)
{
    hdma->Instance = dmaInstance;
    hdma->Init.Request = dmaRequest;
    hdma->Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma->Init.PeriphInc = DMA_PINC_DISABLE;
    hdma->Init.MemInc = DMA_MINC_ENABLE;
    hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma->Init.Mode = DMA_NORMAL;
    hdma->Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma->Init.FIFOMode = DMA_FIFOMODE_DISABLE;

    if (HAL_DMA_Init(hdma) != HAL_OK)
    {
        Error_Handler();
    }
    __HAL_LINKDMA(uartHandle, hdmatx, *hdma);

    // 开启传输完成、传输错误中断
    __HAL_DMA_ENABLE_IT(hdma, DMA_IT_TC | DMA_IT_TE);

    HAL_NVIC_SetPriority(dmaIrq, 2, 0);
    HAL_NVIC_EnableIRQ(dmaIrq);
}


/**
 * @brief  UART RX DMA通用配置函数
 * @param  uartHandle:    串口句柄指针 &huart1 / &huart4
 * @param  dmaInstance:   DMA流实例，如 DMA1_Stream1
 * @param  dmaIrq:        DMA中断号，如 DMA1_Stream1_IRQn
 * @param  dmaRequest:    DMA请求ID，DMA_REQUEST_USART1_RX / DMA_REQUEST_UART4_RX
 * @param  hdma:          DMA接收句柄指针
 */
void USART_RX_DMA_COMMON_Config(UART_HandleTypeDef* uartHandle,
                                DMA_Stream_TypeDef* dmaInstance,
                                IRQn_Type dmaIrq,
                                uint32_t dmaRequest,
                                DMA_HandleTypeDef* hdma)
{
    hdma->Instance = dmaInstance;
    hdma->Init.Request = dmaRequest;
    hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma->Init.PeriphInc = DMA_PINC_DISABLE;
    hdma->Init.MemInc = DMA_MINC_ENABLE;
    hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma->Init.Mode = DMA_NORMAL;
    hdma->Init.Priority = DMA_PRIORITY_LOW;
    hdma->Init.FIFOMode = DMA_FIFOMODE_DISABLE;

    if (HAL_DMA_Init(hdma) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_LINKDMA(uartHandle, hdmarx, *hdma);

    HAL_NVIC_SetPriority(dmaIrq, 2, 0);
    HAL_NVIC_EnableIRQ(dmaIrq);
}








#if USE_OS
extern SemaphoreHandle_t uart_send_res_sem;
#endif


////DMA传输完成自动调用（发送）
//void HAL_USARTx_DMA_TxCpltCallback(void) {


//      GV_usartdmaSend.uart_tx_thisdatas_sendover  = 1;  // 全部发送完成
//	
////SYSTEM_DEBUG("TX Callback \n");
//		
//	
//	#if USE_OS
//	    // 必须：判断信号量是否有效
//    if(uart_send_res_sem != NULL)
//    {
//        // 【中断上下文 专用】释放二值信号量 / 计数信号量
//        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

//        xSemaphoreGiveFromISR( uart_send_res_sem, &xHigherPriorityTaskWoken);

//        // 强制任务切换（必须加）
//        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//    }
//	#endif
//	
//  
//}


/**
 * @brief DMA发送完成业务处理，提取原 HAL_USARTx_DMA_TxCpltCallback 逻辑
 * @param pDmaSendCtrl 发送控制结构体指针 str_DMA_usart_send*
 * @param uart_sem     RTOS信号量句柄，不需要RTOS传NULL
 */
void UART_COMMON_DmaTxCpltProcess(str_DMA_usart_send *pDmaSendCtrl)
{
    if(pDmaSendCtrl == NULL)
    {
        return;
    }

    pDmaSendCtrl->uart_tx_thisdatas_sendover  = 1;  // 当前帧发送完成
//	SYSTEM_DEBUG("TX Callback \n");

}


/*
 * HAL库原生回调，固定原型，不能修改参数；
 * 根据huart实例，分发到上面通用处理函数，传入对应串口的发送控制结构体与信号量
 */
/**
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
#if USE_OS
    // 如果每个串口独立信号量，这里替换为对应串口信号量；共用就继续 uart_send_res_sem
#endif

    if(huart->Instance == USART1)
    {
        UART_COMMON_DmaTxCpltProcess(&g_usart1_dmaSendCtrl);
    }
    else if(huart->Instance == UART4)
    {
        UART_COMMON_DmaTxCpltProcess(&g_uart4_dmaSendCtrl);
    }
}

**/



// RingBuffer_t g_uartRingBuf;
// uint8_t g_ringBufData[MAX_RING_BUFF_SIZE];


/**
 * @brief  UART TX+RX DMA一体化通用配置
 * @param  uartHandle        串口句柄指针 &huart1 / &huart4
 * @param  dmaTxInstance     TX DMA流实例，如 DMA1_Stream0
 * @param  dmaTxIrq          TX DMA中断号，如 DMA1_Stream0_IRQn
 * @param  dmaTxRequest      TX DMA请求 ID，DMA_REQUEST_USART1_TX / DMA_REQUEST_UART4_TX
 * @param  hdmaTx            TX DMA句柄指针
 * @param  dmaRxInstance     RX DMA流实例，如 DMA1_Stream1
 * @param  dmaRxIrq          RX DMA中断号，如 DMA1_Stream1_IRQn
 * @param  dmaRxRequest      RX DMA请求 ID，DMA_REQUEST_USART1_RX / DMA_REQUEST_UART4_RX
 * @param  hdmaRx            RX DMA句柄指针
 * @param  usartIrq          串口自身中断号，USART1_IRQn / UART4_IRQn
 *
 * @param  queue             环形队列句柄指针 QueueType_t*
 * @param  ringBuf           环形队列底层buffer uint8_t*
 * @param  ringBufSize       环形队列长度
 * @param  dmaRcvBuf         DMA接收缓冲区指针 uint8_t*
 * @param  dmaRcvBufSize     DMA接收缓冲区长度
 */
void USART_TX_RX_DMA_COMMON_Config(UART_HandleTypeDef* uartHandle,
                                   DMA_Stream_TypeDef* dmaTxInstance,
                                   IRQn_Type dmaTxIrq,
                                   uint32_t dmaTxRequest,
                                   DMA_HandleTypeDef* hdmaTx,

                                   DMA_Stream_TypeDef* dmaRxInstance,
                                   IRQn_Type dmaRxIrq,
                                   uint32_t dmaRxRequest,
                                   DMA_HandleTypeDef* hdmaRx,

                                   IRQn_Type usartIrq,

                                   QueueType_t *queue,
                                   uint8_t *ringBuf,
                                   uint32_t ringBufSize,

                                   uint8_t *dmaRcvBuf,
                                   uint32_t dmaRcvBufSize)
{
    // 调用通用TX、RX DMA配置函数
    USART_TX_DMA_COMMON_Config(uartHandle, dmaTxInstance, dmaTxIrq, dmaTxRequest, hdmaTx);
    USART_RX_DMA_COMMON_Config(uartHandle, dmaRxInstance, dmaRxIrq, dmaRxRequest, hdmaRx);

#if USE_UART_DMA_RX||1
    // 接收用环形缓冲区初始化（入参传入，不再依赖全局RcvDmaQueData）
    QueueInit(queue, ringBuf, ringBufSize);

    // H7 Cache无效化，DMA接收内存必须做
    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)dmaRcvBuf, dmaRcvBufSize);

    // 开启DMA接收
    HAL_UART_Receive_DMA(uartHandle, dmaRcvBuf, dmaRcvBufSize);

    memset(dmaRcvBuf, 0, dmaRcvBufSize);

    // 清除IDLE标志位
    __HAL_UART_CLEAR_IDLEFLAG(uartHandle);

    // 强制读取ISR、RDR寄存器，清除硬件残留状态
    volatile uint32_t temp_isr = uartHandle->Instance->ISR;
    (void)temp_isr;

    volatile uint32_t temp_rdr = uartHandle->Instance->RDR;
    (void)temp_rdr;

    __HAL_UART_CLEAR_FLAG(uartHandle, UART_FLAG_RXNE);

    // 使能空闲中断 IDLE
    __HAL_UART_ENABLE_IT(uartHandle, UART_IT_IDLE);

    // 串口NVIC配置，使用传入的usartIrq
    HAL_NVIC_SetPriority(usartIrq, 3, 0);
    HAL_NVIC_EnableIRQ(usartIrq);

    HAL_Delay(1); // 关键延时，不可删除
#endif
}



///FIRST-----------------------

void USART_TX_RX_COMMON_DMA_ConfigALL(void){
	
	
	USARTx_COMMON_DMA_Init();
	
	
USART_TX_RX_DMA_COMMON_Config(&huart_DMA_COM01_Handle,
        DMA_COM01_STREAMx_TX,
        DMA_COM01_STREAM_IRQ_TX,
        DMA_COM01_REQUEST_USART_TX,
        &hdma_usartx_COM1_tx,

        DMA_COM01_STREAMx_RX,
        DMA_COM01_STREAM_IRQ_RX,
        DMA_COM01_REQUEST_USART_RX,
        &hdma_usartx_COM1_rx,

        USARTx_DMA_COM01_IRQ,

        &RcvDmaQue_COM1_Data.g_uartRingBuf,
        RcvDmaQue_COM1_Data.g_ringBufData,
        MAX_RING_BUFF_COM01_SIZE,

        RcvDmaQue_COM1_Data.g_rcvDataBuf,
        MAX_BUF_COM01_R_SIZE);
	
	
USART_TX_RX_DMA_COMMON_Config(&huart_DMA_COM02_Handle,
        DMA_COM02_STREAMx_TX,
        DMA_COM02_STREAM_IRQ_TX,
        DMA_COM02_REQUEST_USART_TX,
        &hdma_usartx_COM2_tx,

        DMA_COM02_STREAMx_RX,
        DMA_COM02_STREAM_IRQ_RX,
        DMA_COM02_REQUEST_USART_RX,
        &hdma_usartx_COM2_rx,

        USARTx_DMA_COM02_IRQ,

        &RcvDmaQue_COM2_Data.g_uartRingBuf,
        RcvDmaQue_COM2_Data.g_ringBufData,
        MAX_RING_BUFF_COM02_SIZE,

        RcvDmaQue_COM2_Data.g_rcvDataBuf,
        MAX_BUF_COM02_R_SIZE);
	
	
	
	
//p_tx_rx_groupedqueue_init_dma();
}


//void HAL_USARTx_DMA_ErrorCallback(void) {

//    uint32_t error_code = HAL_UART_GetError(&huart_DMA_Handle);  // 获取错误码
//    SYSTEM_DEBUG("UART Error: 0x%lx\n", error_code);  // 打印错误（需重定向printf）
//    // 常见错误：0x04（溢出错误，数据未及时读取导致覆盖）
//  
//}

/**
 * @brief UART DMA错误回调通用处理
 * @param huart 串口句柄指针
 */
void UART_COMMON_DmaErrorProcess(UART_HandleTypeDef *huart)
{
    if(huart == NULL)
    {
        return;
    }
    uint32_t error_code = HAL_UART_GetError(huart);
    SYSTEM_DEBUG("UART Error: 0x%lx\n", error_code);
    // 常见错误：0x04（溢出错误，数据未及时读取导致覆盖）
}


///**
// * @brief HAL库原生错误回调，原型固定不可修改
// */
//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
//{
//    if(huart->Instance == USART1)
//    {
//        UART_DmaErrorProcess(&huart1);
//    }
//    else if(huart->Instance == UART4)
//    {
//        UART_DmaErrorProcess(&huart4);
//    }
//}







#include "./DataConvert/data_random.h"

#if TEST_COMMON_UsartDMASendSaveAndSend
//(发送)测试发送前保存程序，之后才能从缓冲区取出数据通过dma发送出去
void Test_Usart_DMA_SendSaveFun(void){
uint8_t Senbuff[q_QUEUE_NODE_DATA_LEN_UsartDMAsend] ;
//	fill_data_False_random((char *)Senbuff,20);
//	Senbuff[10]='\0';
//	SYSTEM_DEBUG("ddd =  %d  %d\n",sizeof(Senbuff),strlen((char *)Senbuff));
//	Senbuff[10]='\0';
//	SYSTEM_DEBUG("ddd =  %d  %d\n",sizeof(Senbuff),strlen((char *)Senbuff));
for(uint16_t i=0;i<q_QUEUE_NODE_NUM_UsartDMAsend;i++){
	
fill_data_False_random((char *)Senbuff,QSENDMAXBUFFSIZE);
//SYSTEM_DEBUG("string = %s\n",Senbuff);
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,Senbuff,QSENDMAXBUFFSIZE,"send string = %s\n",Senbuff);
//	int ret = p_push_data_to_queue2(&q_tx_rx_queue_UsartDMAsend, (char *)Senbuff, sizeof(Senbuff)); // 不含'\0'
	Usart_SendDMA_SaveFun((char *)Senbuff,sizeof(Senbuff));

}
}
#endif



/**
 * @brief  将数据压入发送队列
 * @param pDmaSendCtrl: 发送控制结构体指针
 * @param pTxQueue: 发送队列指针 Q_QueueBuffer*
 * @param Sendbuff: 待发送数据
 * @param buff_len: 数据长度
 */
void Usart_SendDMA_COMMON_SaveFun(str_DMA_usart_send *pDmaSendCtrl,
                           Q_QueueBuffer *pTxQueue,
                           char *Sendbuff,
                           uint16_t buff_len)
{
    if(pDmaSendCtrl == NULL || pTxQueue == NULL)
    {
        return;
    }

    pDmaSendCtrl->dbret = p_push_data_to_queue2(pTxQueue, (char *)Sendbuff, buff_len);
    pDmaSendCtrl->uart_tx_justSaveOver = 1;
    pDmaSendCtrl->uart_tx_alldatas_sendfinish = 0;

#if USE_OS
    //释放信号量
    // if(NowUse_rtos==1){
    //     xSemaphoreGive(g_com_uart_send_sem);
    // }
#endif

#if DEBUG_COMMON_DmaUsartPrint
    if (pDmaSendCtrl->dbret == 1) {
        SYSTEM_DEBUG("p_push_data_to_queue2 full,error\n");
    }
#endif
}

/**
 * @brief DMA发送：从队列取出数据调用HAL_UART_Transmit_DMA发送
 * @param huart_x: 串口句柄
 * @param pDmaSendCtrl: 发送控制结构体指针
 * @param pTxQueue: 发送队列指针 Q_QueueBuffer*
 */
void Usart_SendDMA_COMMON_SendFun(str_DMA_usart_send *pDmaSendCtrl,
                           Q_QueueBuffer *pTxQueue,
														UART_HandleTypeDef *huart_x)
{
    if(pDmaSendCtrl == NULL || pTxQueue == NULL || huart_x == NULL)
    {
        return;
    }

    pDmaSendCtrl->current_time = HAL_GetTick();

    if (pDmaSendCtrl->uart_tx_justSaveOver==1
        || pDmaSendCtrl->uart_tx_thisdatas_sendover == 1
        || (pDmaSendCtrl->current_time - pDmaSendCtrl->last_tx_complete_time > pDmaSendCtrl->complete_timeout))
    {
        if(pDmaSendCtrl->current_time - pDmaSendCtrl->last_tx_complete_time > pDmaSendCtrl->complete_timeout)
        {
            pDmaSendCtrl->uart_tx_justSaveOver=0;
#if DEBUG_DmaUsartPrint
            SYSTEM_DEBUG("over time ==:%d\n", pDmaSendCtrl->current_time - pDmaSendCtrl->last_tx_complete_time);
#endif
        }

        // 原型：int p_pop_data_from_queue2(Q_QueueBuffer *qdf, char *dest_dat, uint16_t dest_max_len, uint16_t *out_len);
        pDmaSendCtrl->daret = p_pop_data_from_queue2(pTxQueue,
                                                     (char *)pDmaSendCtrl->send_data,
                                                     sizeof(pDmaSendCtrl->send_data),
                                                     &pDmaSendCtrl->read_out_len);

        if (pDmaSendCtrl->daret == 0)
        {
            pDmaSendCtrl->uart_tx_thisdatas_sendover=0;
            HAL_UART_Transmit_DMA(huart_x, pDmaSendCtrl->send_data, pDmaSendCtrl->read_out_len);

            if(pDmaSendCtrl->read_out_len==0)
            {
                pDmaSendCtrl->uart_tx_alldatas_sendfinish = 1;
            }
        }
        else if (pDmaSendCtrl->daret == 1)
        {
#if DEBUG_DmaUsartPrint
            SYSTEM_DEBUG("empty data %d %d %d\n",
                         pDmaSendCtrl->current_time,
                         pDmaSendCtrl->last_tx_complete_time,
                         pDmaSendCtrl->uart_tx_thisdatas_sendover);
#endif
            pDmaSendCtrl->complete_timeout = TX_WAITTING_TIMEOUT;
            pDmaSendCtrl->uart_tx_thisdatas_sendover=0;
        }
        else if (pDmaSendCtrl->daret == 2)
        {
#if DEBUG_DmaUsartPrint
            SYSTEM_DEBUG("butter too small,cant receive data\n");
#endif
        }

        pDmaSendCtrl->last_tx_complete_time = HAL_GetTick();
    }
}

/****
// USART1
str_DMA_usart_send g_usart1_dmaSendCtrl = {
    .send_data = {0},
    .uart_tx_justSaveOver = 0,
    .uart_tx_thisdatas_sendover = 0,
    .uart_tx_alldatas_sendfinish = 0,
    .read_out_len = 0,
    .daret = 0,
    .dbret = 0,
    .complete_timeout = TX_WAITTING_TIMEOUT,
    .current_time = 0,
    .last_tx_complete_time = 0
};
Q_QueueBuffer q_tx_buf_usart1;

// UART4
str_DMA_usart_send g_uart4_dmaSendCtrl = {
    .send_data = {0},
    .uart_tx_justSaveOver = 0,
    .uart_tx_thisdatas_sendover = 0,
    .uart_tx_alldatas_sendfinish = 0,
    .read_out_len = 0,
    .daret = 0,
    .dbret = 0,
    .complete_timeout = TX_WAITTING_TIMEOUT,
    .current_time = 0,
    .last_tx_complete_time = 0
};
Q_QueueBuffer q_tx_buf_uart4;



// USART1
Usart_SendDMA_COMMON_SaveFun(&g_usart1_dmaSendCtrl, &q_tx_buf_usart1, "test1", 5);
Usart_SendDMA_COMMON_SendFun(&huart1, &g_usart1_dmaSendCtrl, &q_tx_buf_usart1);

// UART4
Usart_SendDMA_COMMON_SaveFun(&g_uart4_dmaSendCtrl, &q_tx_buf_uart4, "test4", 5);
Usart_SendDMA_COMMON_SendFun(&huart4, &g_uart4_dmaSendCtrl, &q_tx_buf_uart4);

***/
	

//p_tx_rx_groupedqueue_init(&q_GROUPqueue_1_UsartDMAsend,&GV_ndb_GROUP_1,q_QUEUE_NODE_NUM_GROUP_1,
//	q_QUEUE_NODE_DATA_LEN_GROUP_1,global_elems_GROUP_1,node_data_GROUP_1,global_node_buff_GROUP_1);

//DMA发送数据(发送)
void Usart_SendFUN_COMMON_ALL(void){
	
Usart_SendDMA_COMMON_SaveFun(&GV_usartdmaCOMMON_COM1_Send, &q_GROUPqueue_1_UsartDMAsend, "test1", 5);
Usart_SendDMA_COMMON_SendFun(&GV_usartdmaCOMMON_COM1_Send, &q_GROUPqueue_1_UsartDMAsend,	&huart_DMA_Handle);
	
//Usart_SendDMA_SendFun(&huart_DMA_Handle);

}



#if TEST_COMMON_UsartDMASendSaveAndSend
void TEST_Usart_SendFUN_ALL(void){
	Test_Usart_DMA_SendSaveFun();
	 SYSTEM_DEBUG("send----------------");
	for(uint16_t i=0;i<q_QUEUE_NODE_NUM_UsartDMAsend;i++){
	Usart_SendFUN_ALL();
		HAL_Delay(1000);
	}
}
#endif	

///--------------------------


//一个一个字节读取BUFF数据
/**
 * @brief  逐个字节从环形队列读出并打印
 * @param  queue         环形队列指针 QueueType_t*
 */
//一个一个字节读取BUFF数据
/**
 * @brief  逐个字节从环形队列读出并打印
 * @param  queue 环形队列指针 QueueType_t*
 */
void USART_RX_DMA_COMMON_SEGGERprint_OneByte(QueueType_t *queue)
{
    if(queue == NULL)
    {
        return;
    }
    uint8_t recv_data; // 临时存储读取到的数据
//	SYSTEM_INFO("Read: %d, Write: %d\n", queue->read_idx, queue->write_idx);
    while (QueuePop(queue, &recv_data)==QUEUE_OK)
    {
        // 以十六进制和ASCII格式打印（根据需求选择）
        SYSTEM_INFO("Received: 0x%02X (%c)\n", recv_data, (recv_data >= 0x20 && recv_data <= 0x7E) ? recv_data : '.');
    }
}


//多个字节读取BUFF数据
/**
 * @brief  批量从环形队列读出并打印数组
 * @param  queue 环形队列指针 QueueType_t*
 */
void USART_RX_DMA_COMMON_SEGGERprint_MultByte(QueueType_t *queue)
{
    if(queue == NULL)
    {
        return;
    }
	uint8_t RDataBuf[MAX_RING_BUFF_SIZE];
	uint8_t R_data_len;
	R_data_len = QueuePopArray(queue, RDataBuf, MAX_RING_BUFF_SIZE);
	
	if(R_data_len>0){
		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,RDataBuf,R_data_len," R DMA D : %d",R_data_len);
	}
}

/**
// 原来全局：USART_RX_DMA_SEGGERprint_OneByte();
// 现在传入对应串口的环形队列指针
USART_RX_DMA_SEGGERprint_OneByte(&uart1_ringQueue);
USART_RX_DMA_SEGGERprint_MultByte(&uart1_ringQueue);

USART_RX_DMA_SEGGERprint_OneByte(&uart4_ringQueue);
USART_RX_DMA_SEGGERprint_MultByte(&uart4_ringQueue);
**/


#if TEST_COMMON_DMA_RB_READ
void TEST_USART_RX_DMA_ALL(void){
//	需要先初始化USART_TX_RX_DMA_ConfigALL
//Test_Usart_QueueSaveFun();//存入buff
	#if 1
USART_RX_DMA_SEGGERprint_OneByte();
	#else
USART_RX_DMA_SEGGERprint_MultByte();	
	#endif
}



void TEST_USART_RX_DMA_ALL_while(void){

while(1){

TEST_USART_RX_DMA_ALL();
	HAL_Delay(200);
}
}
#endif

///--------------------------
//weak void HAL_USARTx_DMA_RxCpltCallback(void){}

#if (USE_UART_COMMON_DMA)
#if (USE_UART_COMMON_DMA_RX)

/**
 * @brief DMA接收完成回调业务处理
 * @param huart        UART句柄
 * @param hdma_rx      RX DMA句柄
 * @param pRcvData     接收集合结构体指针 STR_RCV_DMA_que_data*
 */
void UART_COMMON_DmaRxCpltProcess(UART_HandleTypeDef *huart,
                           DMA_HandleTypeDef *hdma_rx,
                           STR_RCV_DMA_que_data *pRcvData)
{
    if(huart == NULL || hdma_rx == NULL || pRcvData == NULL)
    {
        return;
    }

    pRcvData->received_data_len = MAX_BUF_R_SIZE - __HAL_DMA_GET_COUNTER(hdma_rx);

    if (pRcvData->received_data_len > 0)
    {
        QueuePushArray(&pRcvData->g_uartRingBuf, pRcvData->g_rcvDataBuf, pRcvData->received_data_len);
    }

    //SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,pRcvData->g_rcvDataBuf,5,"finish %d",pRcvData->received_data_len);

    HAL_UART_Receive_DMA(huart, pRcvData->g_rcvDataBuf, MAX_BUF_R_SIZE);
}


/**
 * @brief IDLE空闲中断业务处理
 * @param huart        UART句柄
 * @param hdma_rx      RX DMA句柄
 * @param pRcvData     接收集合结构体指针 STR_RCV_DMA_que_data*
 */
void UART_COMMON_DmaIdleProcess(UART_HandleTypeDef *huart,
                         DMA_HandleTypeDef *hdma_rx,
                         STR_RCV_DMA_que_data *pRcvData)
{
    if(huart == NULL || hdma_rx == NULL || pRcvData == NULL)
    {
        return;
    }

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        HAL_UART_DMAStop(huart);

        SCB_InvalidateDCache_by_Addr((uint32_t *)pRcvData->g_rcvDataBuf, MAX_BUF_R_SIZE);

        pRcvData->received_data_len = MAX_BUF_R_SIZE - __HAL_DMA_GET_COUNTER(hdma_rx);

        SCB_InvalidateDCache_by_Addr((uint32_t *)pRcvData->g_rcvDataBuf, MAX_BUF_R_SIZE);

        if (pRcvData->received_data_len > 0)
        {
            QueuePushArray(&pRcvData->g_uartRingBuf, pRcvData->g_rcvDataBuf, pRcvData->received_data_len);
        }

        HAL_UART_Receive_DMA(huart, pRcvData->g_rcvDataBuf, MAX_BUF_R_SIZE);
    }
}

#endif
#endif


void USARTx_DMA_COM01_IRQHandler(void)
{
    UART_COMMON_DmaIdleProcess(&huart_DMA_COM01_Handle, &hdma_usartx_COM1_rx, &RcvDmaQue_COM1_Data);
    HAL_UART_IRQHandler(&huart_DMA_COM01_Handle);
}

void USARTx_DMA_COM02_IRQHandler(void)
{
    UART_COMMON_DmaIdleProcess(&huart_DMA_COM02_Handle, &hdma_usartx_COM2_rx, &RcvDmaQue_COM2_Data);
    HAL_UART_IRQHandler(&huart_DMA_COM02_Handle);
}
//void USART1_IRQHandler(void)
//{
//    UART_DmaIdleProcess(&huart1, &hdma_usart1_rx, &g_usart1_RcvQueData);
//    HAL_UART_IRQHandler(&huart1);
//}

//void UART4_IRQHandler(void)
//{
//    UART_DmaIdleProcess(&huart4, &hdma_uart4_rx, &g_uart4_RcvQueData);
//    HAL_UART_IRQHandler(&huart4);
//}





//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//#if (USE_UART_DMA)
//#if (USE_UART_DMA_RX)
//    if(huart->Instance == USART1)
//    {
//        UART_DmaRxCpltProcess(&huart1, &hdma_usart1_rx, &g_usart1_RcvQueData);
//    }
//    else if(huart->Instance == UART4)
//    {
//        UART_DmaRxCpltProcess(&huart4, &hdma_uart4_rx, &g_uart4_RcvQueData);
//    }
//#endif
//#endif
//}






/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usartx_COM1_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */

  /* USER CODE END DMA2_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usartx_COM1_tx);
  /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

  /* USER CODE END DMA2_Stream7_IRQn 1 */
}
#endif











/* USER CODE END 1 */




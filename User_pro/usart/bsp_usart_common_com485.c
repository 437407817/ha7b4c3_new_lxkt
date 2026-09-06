/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   使用串口1，重定向c库printf函数到usart端口，中断接收模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_usart_common_com485.h"
#include "./buffer/p_data_queue_outer.h"
#include "./buffer/que.h"
#include "./sys/sysio.h"
#include "./pro_com/usart485verify.h"
#include "./usart/bsp_usart_dma.h"

extern void Usart_SendDMA_SaveFun(char *buf, uint16_t num);
//extern void Usart_SendFUN_ALL(void);

UART_HandleTypeDef huart_COM01_COM485_Handle;


// .c文件
uint8_t com01_parseBuf[COM01_PARSE_BUF_SIZE];



// 全局实例，可以切换赋值
//U485UsartSend_Callback_t g_U485UsartSendCb;

U485ComUsartSend_Callback_t com01_485_cbCfg = {
    .U485ComSendDmaSaveDataFunc  = Usart_SendDMA_SaveFun,
    .U485ComSendAllFunc  = UART_COMMON_Instance_SendArray,

};

//UartComInstance com01_com485Inst = {
//    .huart_handle = &huart_COM01_COM485_Handle,
//};

//外部解析缓冲区、队列，在别的文件定义
//extern STR_RCV_DMA_que_data Rcv_Common_DmaQueData_Com1;
//extern uint8_t com01_parseBuf[];

UartComInstance com01_com485Inst = {
    .huart_handle = &huart_COM01_COM485_Handle,
    .sendCbStore = {0},

    .pRcvQueue      = &RcvDmaQue_COM1_Data.g_uartRingBuf,
    .parseBuf       = com01_parseBuf,
    .parseBufLen    = COM01_PARSE_BUF_SIZE,

    .pRxByteCb      = NULL,
    .pSlaveProcCb   = NULL,
};








// 声明全局执行指针
UsartComSendPtr this_com_com485_Usart_Send = NULL;

//UART_HandleTypeDef huart_COM485_Handle;




#if TEST_COM485_UART

#define RX_BUF_LEN 64
//static uint8_t uart_rx_buf[RX_BUF_LEN];  // 多字节接收缓冲区
static volatile uint8_t index = 0;
static volatile uint8_t indexsize = 0;

//uint16_t rx_len = 0;              // 接收长度计数
//uint8_t uart_rx_byte;  // 单次接收 1 字节
#endif




/**
 * @brief UART通用初始化函数，485串口初始化逻辑通用版
 * @param huart              UART句柄指针
 * @param Instance           外设实例，如 USART1
 * @param baudrate           波特率
 * @param irq_n              中断号，如 USART1_IRQn
 * @param preempt_prio       抢占优先级
 * @param sub_prio           子优先级
 * @param enableIdleIt       1:开启IDLE中断
 * @param enableRxneIt       1:开启RXNE中断
 * @param pfnClockEnable     回调：开启外设时钟，传入NULL则不执行时钟使能
 * @retval HAL status
 */
typedef void (*UartClockEnableCb)(void);

HAL_StatusTypeDef UART_Common_Init(UART_HandleTypeDef *huart,
                                   USART_TypeDef *Instance,
                                   uint32_t baudrate,
                                   IRQn_Type irq_n,
                                   uint32_t preempt_prio,
                                   uint32_t sub_prio,
                                   uint8_t enableIdleIt,
                                   uint8_t enableRxneIt,
                                   UartClockEnableCb pfnClockEnable)
{
    if(huart == NULL)
    {
        return HAL_ERROR;
    }

    //开启外设时钟
    if(pfnClockEnable != NULL)
    {
        pfnClockEnable();
    }

    huart->Instance = Instance;
    huart->Init.BaudRate = baudrate;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;
    huart->Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
    huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(huart) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_UARTEx_SetTxFifoThreshold(huart, UART_TXFIFO_THRESHOLD_1_2) != HAL_OK)
    {
        return HAL_ERROR;
    }
    if (HAL_UARTEx_SetRxFifoThreshold(huart, UART_RXFIFO_THRESHOLD_1_2) != HAL_OK)
    {
        return HAL_ERROR;
    }
    if (HAL_UARTEx_DisableFifoMode(huart) != HAL_OK)
    {
        return HAL_ERROR;
    }

#if !USE_UART_COMMON_DMA_RX
    if(enableIdleIt)
    {
        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    }
    if(enableRxneIt)
    {
        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
    }
//    if(enableReceiveIt)
//    {
//				HAL_UART_Receive_IT(&huart_COM485_Handle, &uart485_rx_byte, 1);
//    }		
		
		
#endif

    //配置NVIC中断
    HAL_NVIC_SetPriority(irq_n, preempt_prio, sub_prio);
    HAL_NVIC_EnableIRQ(irq_n);

    return HAL_OK;
}




static void USART_common_com1_ClockEnable(void)
{
    __HAL_RCC_USART2_CLK_ENABLE();
}



//#define USART_COM01_COM485                             USART2
//#define USART_COM01_COM485_CLK_ENABLE()                __USART2_CLK_ENABLE();
//			  
//#define USART_COM01_COM485_RX_GPIO_PORT                GPIOD
//#define USART_COM01_COM485_RX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()
//#define USART_COM01_COM485_RX_PIN                      GPIO_PIN_6
//#define USART_COM01_COM485_RX_AF                       GPIO_AF7_USART2
//			  
//#define USART_COM01_COM485_TX_GPIO_PORT                GPIOD
//#define USART_COM01_COM485_TX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()
//#define USART_COM01_COM485_TX_PIN                      GPIO_PIN_5
//#define USART_COM01_COM485_TX_AF                       GPIO_AF7_USART2
//			  
//#define USART_COM01_COM485_IRQHandler                  USART2_IRQHandler
//#define USART_COM01_COM485_IRQ                 		    USART2_IRQn

void USART_common_COM485_UartInit(void)
{
    if(UART_Common_Init(com01_com485Inst.huart_handle,
                        USART_COM01_COM485,
                        USART_COM01_COM485_BAUDRATE,
                        USART_COM01_COM485_IRQ,
                        10,        //抢占优先级
                        0,         //子优先级
                        ENABLE_IDLE_IT_STATUS,         //enableIdleIt
                        ENABLE_RXNE_IT_STATUS,          //enableRxneIt
                        USART_common_com1_ClockEnable) != HAL_OK)
    {
        Error_Handler();
    }

		
		
		
		

}







/**
 * @brief UART GPIO与外设时钟源通用初始化
 * @param tx_port         TX GPIO端口
 * @param tx_pin          TX引脚
 * @param tx_af           TX复用功能AF编号
 * @param rx_port         RX GPIO端口
 * @param rx_pin          RX引脚
 * @param rx_af           RX复用功能AF编号
 * @param pfnGpioClkEnable 回调函数：开启TX/RX GPIO时钟
 * @param periph_clk_sel  RCC_PeriphCLKInitTypeDef.PeriphClockSelection
 * @param uart_clk_sel1   Usart234578ClockSelection / Usart16ClockSelection 选项
 * @param is_usart16      true=USART1/6组(Usart16ClockSelection), false=USART2/3/4/5/7/8组(Usart234578ClockSelection)
 * @retval HAL status
 */
typedef void (*UartGpioClockEnableCb)(void);

HAL_StatusTypeDef UART_Common_GpioInit(GPIO_TypeDef *tx_port,
                                       uint16_t tx_pin,
                                       uint8_t tx_af,
                                       GPIO_TypeDef *rx_port,
                                       uint16_t rx_pin,
                                       uint8_t rx_af,
                                       UartGpioClockEnableCb pfnGpioClkEnable,
                                       uint32_t periph_clk_sel,
                                       uint32_t uart_clk_sel1,
                                       uint8_t is_usart16)
{
    if(tx_port == NULL || rx_port == NULL || pfnGpioClkEnable == NULL)
    {
        return HAL_ERROR;
    }

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};

    // 开启GPIO时钟
    pfnGpioClkEnable();

    // 配置UART外设时钟源
    RCC_PeriphClkInit.PeriphClockSelection = periph_clk_sel;
    if(is_usart16)
    {
        RCC_PeriphClkInit.Usart16ClockSelection = uart_clk_sel1;
    }
    else
    {
        RCC_PeriphClkInit.Usart234578ClockSelection = uart_clk_sel1;
    }
    if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
    {
        return HAL_ERROR;
    }

    // TX引脚配置 AF‑PP 上拉 高速
    GPIO_InitStruct.Pin = tx_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = tx_af;
    HAL_GPIO_Init(tx_port, &GPIO_InitStruct);

    // RX引脚配置，复用其余配置
    GPIO_InitStruct.Pin = rx_pin;
    GPIO_InitStruct.Alternate = rx_af;
    HAL_GPIO_Init(rx_port, &GPIO_InitStruct);

    return HAL_OK;
}




static void COM485_COM01_GpioClockEnable(void)
{
    USART_COM01_COM485_TX_GPIO_CLK_ENABLE();
    USART_COM01_COM485_RX_GPIO_CLK_ENABLE();
}


//#define USART_COM01_COM485                             USART2
//#define USART_COM01_COM485_CLK_ENABLE()                __USART2_CLK_ENABLE();
//			  
//#define USART_COM01_COM485_RX_GPIO_PORT                GPIOD
//#define USART_COM01_COM485_RX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()
//#define USART_COM01_COM485_RX_PIN                      GPIO_PIN_6
//#define USART_COM01_COM485_RX_AF                       GPIO_AF7_USART2
//			  
//#define USART_COM01_COM485_TX_GPIO_PORT                GPIOD
//#define USART_COM01_COM485_TX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()
//#define USART_COM01_COM485_TX_PIN                      GPIO_PIN_5
//#define USART_COM01_COM485_TX_AF                       GPIO_AF7_USART2
//			  
//#define USART_COM01_COM485_IRQHandler                  USART2_IRQHandler
//#define USART_COM01_COM485_IRQ                 		    USART2_IRQn



void USART_COMMON_COM485_GpioInit(void)
{
#if !EXCHINGE_UASRT_SHELL_485
    // USART2‑5‑7‑8组
    if(UART_Common_GpioInit(USART_COM01_COM485_TX_GPIO_PORT,
                            USART_COM01_COM485_TX_PIN,
                            USART_COM01_COM485_TX_AF,
		
                            USART_COM01_COM485_RX_GPIO_PORT,
                            USART_COM01_COM485_RX_PIN,
                            USART_COM01_COM485_RX_AF,
		
                            COM485_COM01_GpioClockEnable,
                            USART_COM01_COM485_RCC_PERIPHCLK,
                            USART_COM01_COM485_RCC_CLKSOURCE,
                            0) != HAL_OK)
#else
    // USART1/6组
    if(UART_Common_GpioInit(USART_COM485_TX_GPIO_PORT,
                            USART_COM485_TX_PIN,
                            USART_COM485_TX_AF,
                            USART_COM485_RX_GPIO_PORT,
                            USART_COM485_RX_PIN,
                            USART_COM485_RX_AF,
                            COM485_GpioClockEnable,
                            RCC_PERIPHCLK_USART1,
                            RCC_USART16CLKSOURCE_D2PCLK2,
                            1) != HAL_OK)
#endif
    {
        Error_Handler();
    }
}




/**
 * @brief 设置实例单字节接收回调
 */
void UART_INST_SetRxByteCb(UartComInstance *pInst, UartRxByteCallback  pFunc)
{
//    if(pInst == NULL) return;
    pInst->pRxByteCb = pFunc;
}






/**
***********************************************************
* @brief USB转串口硬件初始化
* @param
* @return 
***********************************************************
*/
//void USART_COMMON_COM485_232_ComDrvInit2222222(void)
//{
//	
////		Usart485ComAppInit();//如果无此，pProcUartDataFunc(recv_byte);会报错
//	
//	
//	USART_common_COM485_UartInit();

//USART_COMMON_COM485_GpioInit();

//	
////	Usart_COMMON_COM485_send_Config_Init();
//	
////U485Usart_SetSendCallback(&com01_485_cbCfg);
//	UART_COMMON_Instance_SetSendCallback(&com01_com485Inst, &com01_485_cbCfg);
//}





/**
 * @brief 通用：收到单字节，压入本实例的环形队列
 * @param data 收到字节
 * @param pCtx 上下文 = UartComInstance*
 */
static void UartRxPushToQueueCb(uint8_t data, void *pCtx)
{
    UartComInstance *pInst = (UartComInstance *)pCtx;
    if(pInst == NULL || pInst->pRcvQueue == NULL)
    {
        return;
    }
    QueuePush(pInst->pRcvQueue, data);
		SYSTEM_INFO("%c",data);
}




void USART_COMMON_COM485_232_ComDrvInit(void)
{
	USART_common_COM485_UartInit();
    USART_COMMON_COM485_GpioInit();

	UART_COMMON_Instance_SetSendCallback(&com01_com485Inst, &com01_485_cbCfg);

    //绑定接收回调：收到字节直接压入实例内部的pRcvQueue
    UART_INST_SetRxByteCb(&com01_com485Inst, UartRxPushToQueueCb);
}





/**
 * @brief UART RXNE+IDLE中断模式通用处理逻辑（非DMA，单字节中断接收）
 * @param huart          UART句柄
 * @param pByteCb        单字节接收回调；NULL则不回调
 *                       回调原型 void cb(uint8_t byte)
 */
//typedef void (*UartRxByteCallback )(uint8_t byte);

/**
 * @brief UART RXNE+IDLE中断模式通用处理逻辑（非DMA，单字节中断接收，带上下文）
 * @param huart          UART句柄
 * @param pByteCb        带上下文单字节接收回调；NULL则不回调
 * @param pUserCtx       用户上下文，这里传入UartComInstance*
 */
void UART_Common_IT_Process(UART_HandleTypeDef *huart, UartRxByteCallback  pByteCb, void *pUserCtx)
{
    if(huart == NULL)
    {
        return;
    }

    // RXNE 接收数据寄存器非空中断
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET)
    {
        __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_RXNE);
        uint8_t recv_byte = (uint8_t)(huart->Instance->RDR);

        if(pByteCb != NULL)
        {
            pByteCb(recv_byte, pUserCtx);
        }
    }

    // IDLE 空闲帧中断
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);
    }
}





//void HAL_UART_COM485_RxCpltCallback(UART_HandleTypeDef *huart){
//	
//uart_rx_buf[index++] = uart_rx_byte;
////SYSTEM_DEBUG("+++%c",uart_rx_byte);
//HAL_UART_Receive_IT(&huart_COM485_Handle, &uart_rx_byte, 1);
//		
//}

//#if USE_COM485_IT_1 && !USE_UART_DMA_RX
//extern void pProcUartDataFunc(uint8_t byte);

//void USART_COM485_IRQHandler(void)
//{
//    UART_Common_IT_Process(&huart_COM485_Handle, pProcUartDataFunc);
//    HAL_UART_IRQHandler(&huart_COM485_Handle);
//}
//#endif
//// 声明你自己的字节处理函数
//extern void Usart1_RxByteHandler(uint8_t byte);

//void USART1_IRQHandler(void)
//{
//    UART_Common_IT_Process(&huart1, Usart1_RxByteHandler);
//    HAL_UART_IRQHandler(&huart1);
//}
//typedef void (*UartRxByteCallback)(uint8_t byte);
//void UART_Common_IT_Process(UART_HandleTypeDef *huart, UartRxByteCallback pByteCb);

/**
***********************************************************
* @brief 串口1中断服务函数
* @param
* @return 
***********************************************************
*/
static void (*p_com01_UartDataFun)(uint8_t data);   //函数指针变量，保存应用层回调函数地址
/**
***********************************************************
* @brief 注册回调函数
* @param pFunc，函数指针变量，接收传入的回调函数地址
* @return 
***********************************************************
*/
void Register_com01_processUartDataFunc(void (*pFunc)(uint8_t data))
{
	p_com01_UartDataFun = pFunc;
}

//#if (USE_UART_RX_COMMON_DMA)
#if USE_COM01_COM485_IT_1 && !USE_UART_RX_COMMON_DMA 

//extern void pProcUartDataFunc(uint8_t byte);


extern UartComInstance com01_com485Inst;

void USART_COM01_COM485_IRQHandler(void)
{
    //把实例指针作为上下文传入
    UART_Common_IT_Process(com01_com485Inst.huart_handle,
                           com01_com485Inst.pRxByteCb,
                           &com01_com485Inst);

    HAL_UART_IRQHandler(com01_com485Inst.huart_handle);
}


#endif




//void UART_COMMON_Instance_SendArray(UART_HandleTypeDef *huart, uint8_t *array, uint16_t num);


void Usart_COMMON_COM485_send_Config_Init(void)
{
#if USE_COMMON_COM485_DMA_SEND
    this_com_com485_Usart_Send = UART_COMMON_Instance_SendArray_DMA;
#else
    this_com_com485_Usart_Send = UART_COMMON_Instance_SendArray;
#endif
}







//void Usart_COMMON_COM485_SendArray(UART_HandleTypeDef *huart, uint8_t *array, uint16_t num)
//{
//    // 直接发送整个数组（阻塞式）
//    HAL_UART_Transmit(huart, array, num, HAL_MAX_DELAY);

//    // 等待发送完成（可选，HAL_UART_Transmit 已等发送为空，再加更保险）
//    while(__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);
//}

/**
 * @brief UART 阻塞发送数组（实例版本）
 * @param pInst 串口实例指针
 * @param array 发送数据缓冲区
 * @param num   发送字节数
 * @return 无
 */
void UART_COMMON_Instance_SendArray(UartComInstance *pInst, uint8_t *array, uint16_t num)
{
    if (pInst == NULL || pInst->huart_handle == NULL || array == NULL || num == 0U)
    {
        return;
    }

    UART_HandleTypeDef *huart = pInst->huart_handle;

    // 阻塞式发送整个数组
    HAL_UART_Transmit(huart, array, num, HAL_MAX_DELAY);

    // 等待发送完成标志TC置位
    while (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);
}


#include "./usart/bsp_usart_dma.h"

/**
 * @brief DMA发送，直接传入实例指针，内部取实例里已经注册好的回调
 */
void UART_COMMON_Instance_SendArray_DMA(UartComInstance *pInst, uint8_t *array, uint16_t num)
{
    if(pInst == NULL || pInst->huart_handle == NULL || array == NULL || num == 0)
        return;

    U485ComUsartSend_Callback_t *pCb = &pInst->sendCbStore;
#if SAVEDMADATA		
    if(pCb->U485ComSendDmaSaveDataFunc != NULL)
    {
        pCb->U485ComSendDmaSaveDataFunc((char *)array, num);
    }
#endif		
    if(pCb->U485ComSendAllFunc != NULL)
    {
        pCb->U485ComSendAllFunc(pInst,array,num);
    }
}



/**
 * @brief 向该串口实例注册发送回调，只传源pSrcCb
 * @param pInst 串口实例
 * @param pSrcCb 源回调配置；NULL清空
 */
void UART_COMMON_Instance_SetSendCallback(UartComInstance *pInst, const U485ComUsartSend_Callback_t *pSrcCb)
{
    if(pInst == NULL)
        return;
    if(pSrcCb == NULL)
    {
        pInst->sendCbStore.U485ComSendDmaSaveDataFunc = NULL;
        pInst->sendCbStore.U485ComSendAllFunc = NULL;
        SYSTEM_ERROR("callback NOT register !");
        return;
    }
    pInst->sendCbStore = *pSrcCb;
}



////定义485实例
//UartComInstance com485Inst = {
//    .huart = &huart_COM_DW_Handle,
//};

//const U485UsartSend_Callback_t com01_485_cbCfg = {
//    .U485SendDmaSaveDataFunc  = Usart_SendDMA_SaveFun,
//    .U485SendAllFunc  = Usart_SendFUN_ALL,
//};

////注册：只传实例 + 源pSrcCb
//UART_Instance_SetSendCallback(&com485Inst, &com01_485_cbCfg);

////发送：只传实例
//UART_Instance_SendArray_DMA(&com485Inst, buf, len);

////清空注册
//UART_Instance_SetSendCallback(&com485Inst, NULL);




void UART_COMMON_SendAllFunc2(void){
//uint8_t tx_buf[] = "test1";
//UART_COMMON_Instance_SendArray(&com01_com485Inst, tx_buf, 5);
	
	
	
//com01_com485Inst.sendCbStore.U485ComSendAllFunc(&com01_com485Inst,tx_buf, 5);
}


void UART_COMMON_SendAllFunc(void)
{
    uint8_t tx_buf[] = "test1";

    if(com01_com485Inst.huart_handle == NULL)
    {
        SYSTEM_DEBUG("ERR:huart_handle is NULL\n");
        return;
    }

    UART_HandleTypeDef *huart = com01_com485Inst.huart_handle;

    //打印关键状态
    SYSTEM_DEBUG("huart->gState:%d\n", huart->gState);
    SYSTEM_DEBUG("huart->ErrorCode:0x%08lX\n", huart->ErrorCode);
    SYSTEM_DEBUG("Instance:%p\n", huart->Instance);

    if(huart->gState != HAL_UART_STATE_READY)
    {
        SYSTEM_DEBUG("ERR:uart not READY!\n");
        return;
    }

    HAL_StatusTypeDef hal_ret = HAL_UART_Transmit(huart, tx_buf, 5, 100); //不要HAL_MAX_DELAY，给100ms超时
    SYSTEM_DEBUG("HAL_UART_Transmit ret=%d\n", hal_ret);

    if(hal_ret == HAL_OK)
    {
        while(__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);
        SYSTEM_DEBUG("send finish TC ok\n");
    }
}




















/*********************************************END OF FILE**********************/

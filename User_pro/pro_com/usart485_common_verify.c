
#include "./pro_com/usart485_common_verify.h"

#include <stdio.h>
#include <stdbool.h>

//#include "./middle_business/usart_485_business.h"
#include "./DataConvert/data_verification.h"
#include <stdlib.h>
#include "./buffer/queue3.h"
 #include "./sys/sysio.h"

 #include "./usart/bsp_usart_COM485.h"
 #include "./middle_business/usart_485_dataGet.h"
 #include "./pro_com/usart_485_232_send.h"
 #include "./usart/bsp_usart_dma.h"
 
 #include "./usart/bsp_usart_common_com485.h"
/**
***********************************************************************
包格式：帧头0  帧头1  数据长度  功能字   LED编号  亮/灭  异或校验数据
        0x55   0xAA    0x05      0x82     0x00     0x01      0xFB
***********************************************************************
*/
#define FRAME_REC_HEAD_0           0x5A  
#define FRAME_REC_HEAD_1           0xA5




#define FUNC_DATA_IDX          3                  //功能字数组下标
#define SLAVE_CTRL_CODE          0x82               //功能字



//static uint8_t g_rcvDataBuf[MAX_BUF_SIZE];
//static QueueType_t g_rcvQueue;

extern STR_RCV_DMA_que_data RcvDmaQue_COM01_Data;

extern STR_RCV_DMA_que_data RcvDmaQue_COM02_Data;

extern STR_RCV_DMA_que_data RcvDmaQue_COM03_Data;

extern STR_RCV_DMA_que_data RcvDmaQue_COM04_Data;

extern STR_RCV_DMA_que_data RcvDmaQue_COM05_Data;

extern STR_RCV_DMA_que_data RcvDmaQue_COM06_Data;


//extern STR_RCV_DMA_que_data Rcv_Common_DmaQueData_Com1;
#define g_com01_rcvDataBuf RcvDmaQue_COM01_Data.g_ringBufData
#define g_com01_rcvQueue RcvDmaQue_COM01_Data.g_uartRingBuf

#define g_com02_rcvDataBuf RcvDmaQue_COM02_Data.g_ringBufData
#define g_com02_rcvQueue RcvDmaQue_COM02_Data.g_uartRingBuf

#define g_com03_rcvDataBuf RcvDmaQue_COM03_Data.g_ringBufData
#define g_com03_rcvQueue RcvDmaQue_COM03_Data.g_uartRingBuf

#define g_com04_rcvDataBuf RcvDmaQue_COM04_Data.g_ringBufData
#define g_com04_rcvQueue RcvDmaQue_COM04_Data.g_uartRingBuf

#define g_com05_rcvDataBuf RcvDmaQue_COM05_Data.g_ringBufData
#define g_com05_rcvQueue RcvDmaQue_COM05_Data.g_uartRingBuf

#define g_com06_rcvDataBuf RcvDmaQue_COM06_Data.g_ringBufData
#define g_com06_rcvQueue RcvDmaQue_COM06_Data.g_uartRingBuf



typedef struct
{
	uint8_t ledNo;
	uint8_t ledState;
} LedCtrlInfo_t;


// 回调原型：传入当前串口对应的环形队列指针
static void (*pProcCommonSlaveDataFunc)(uint8_t *readBuf, uint16_t frameLen);

/**
 * @brief 注册从机业务处理回调
 * @param pFunc 回调函数，参数为当前串口接收队列指针
 */
//void reg_SlaveComCb(void (*pFunc)(QueueType_t *pRcvQueue))
//{
//    pProcCommonSlaveDataFunc = pFunc;
//}

void reg_SlaveCommonComCb(void (*pFunc)(uint8_t *readBuf, uint16_t frameLen))
{
    pProcCommonSlaveDataFunc = pFunc;
}

/**
 * @brief 通用单字节处理函数，把字节压入传入的队列
 * @param data 接收字节
 * @param pCtx 上下文，存放队列指针
 */
//static void ProcUar---tData_Common(uint8_t data, UartRxCtx_t *pCtx)
//{
//    if(pCtx == NULL || pCtx->pRcvQueue == NULL)
//    {
//        return;
//    }
//    QueuePush(pCtx->pRcvQueue, data);
//}

//业务处理函数
//static void ProcUartData_Common(uint8_t data, void *pUserCtx)
//{
//    UartRxCtx_t *pCtx = (UartRxCtx_t *)pUserCtx;
//    if(pCtx == NULL || pCtx->pRcvQueue == NULL)
//    {
//        return;
//    }
//    QueuePush(pCtx->pRcvQueue, data);
//}




///**
//***********************************************************
//* @brief 对数据进行异或运算
//* @param data, 存储数组的首地址
//* @param len, 要计算的元素的个数
//* @return 异或运算结果
//***********************************************************
//*/
//static uint8_t CalXorSum(const uint8_t *data, uint32_t len)
//{
//	uint8_t xorSum = 0;
//	for (uint32_t i = 0; i < len; i++)
//	{
//		xorSum ^= data[i];
//	}
//	return xorSum;
//}


//static uint8_t CalHead( uint8_t *q_data)
//{

//			//SYSTEM_DEBUG_ARRAY((uint8_t*)*frame,rx_data->len);
//		if((q_data[0]!=FRAME_HEAD_0)||(q_data[1]!=FRAME_HEAD_1)){
//SYSTEM_ERROR("5AA5 data is not right \n");
//	return 1;
//		}

//return 0;
//}



static uint8_t CalCRC16( uint8_t *q_data, uint32_t q_len)
{
	uint16_t crc16_res;
	uint8_t	cl,ch;
//				crc16_res = crc_16(&q_data[3], q_len-5);
		crc16_res = crc_16(&q_data[3], q_len-5);
	cl = (uint8_t)(crc16_res & 0xff);
	ch = (uint8_t)((crc16_res & 0xff00) >> 8);
	
		
		if((q_data[q_len-2]!=cl)||(q_data[q_len-1]!=ch)){
		SYSTEM_ERROR("crc data is not right h=0x%X  l=0x%X \n",ch,cl);
		return 1;
}
return 0;
}



///**
//***********************************************************
//* @brief LED控制处理函数
//* @param ctrlData，结构体指针，传入LED的编号和状态
//* @return 
//***********************************************************
//*/
//static void CtrlLed(LedCtrlInfo_t *ctrlData)
//{
//	ctrlData->ledState != 0 ? TurnOnLed(ctrlData->ledNo) : TurnOffLed(ctrlData->ledNo);
//}

//static uint8_t readBuf[PACKET_DATA_LEN_MAX] = {0};
static uint8_t readBuf[PACKET_DATA_LEN_MAX + 4U] = {0};
//s485dataswitch(&f_data,s_num,s_command_num);
void s485_convert_queue222(uint8_t **outalldata,uint16_t* Command_address,uint8_t** OutData,uint8_t* Datanum){

*outalldata=readBuf;
*Command_address=((uint16_t)*(*outalldata+4)<<8)|(*(*outalldata+5));
//	SYSTEM_INFO("0x%4x ",(uint16_t)*(*outalldata+4)<<8);
//	SYSTEM_INFO("0x%4x ",(uint16_t)*(*outalldata+5));
//	SYSTEM_INFO("0x%4x ",((uint16_t)*(*outalldata+4)<<8)|(*(*outalldata+5)));
	*OutData=(*outalldata)+6;
	#if (c485_232_CRC16==1)	
	*Datanum=*(*outalldata+2)-5;
	#else
	*Datanum=*(*outalldata+2)-3;
	#endif
	if(*Datanum!=0){
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)*outalldata,*(*outalldata+2)+3,"485 GET-D-PACK -count: %d\n",*(*outalldata+2)+2);
	}
}


/**
 * @brief 设置实例从机业务回调
 */
void UART_INST_SetSlaveCb(UartComInstance *pInst, UartSlaveProcCallback pFunc)
{
    if(pInst == NULL) return;
    pInst->pSlaveProcCb = pFunc;
}




/**
 * @brief 485帧解析任务
 * @param pInst        串口实例
 * @param extReadBuf   外部解析缓冲区；传NULL，则使用实例内部parseBuf
 * @param extReadBufMaxLen 外部缓冲区大小；extReadBuf为NULL时该参数无效
 */
void Usart485CommonComTask(UartComInstance *pInst,
                           uint8_t *extReadBuf,
                           uint16_t extReadBufMaxLen)
{
    uint8_t *readBuf = NULL;
    uint16_t readBufMaxLen = 0U;

    if(pInst == NULL || pInst->pRcvQueue == NULL)
    {
        return;
    }

    //选择缓冲区：外部优先，外部为NULL则使用实例自带
    if(extReadBuf != NULL)
    {
        readBuf = extReadBuf;
        readBufMaxLen = extReadBufMaxLen;
    }
    else
    {
        readBuf = pInst->parseBuf;
        readBufMaxLen = pInst->parseBufLen;
    }

    //有效性校验
    if(readBuf == NULL || readBufMaxLen == 0U)
    {
        return;
    }

    QueueType_t *pRcvQueue = pInst->pRcvQueue;
    uint32_t qcp = 0U;

#if 0
    while ((QueueCount(pRcvQueue) >= PACKET_DATA_LEN_MIN) && (QueuePop(pRcvQueue, &readBuf[0]) == QUEUE_OK))
    {
        if (readBuf[0] != FRAME_REC_HEAD_0)
        {
            continue;
        }

        if ((QueuePop(pRcvQueue, &readBuf[1]) == QUEUE_EMPTY) || (readBuf[1] != FRAME_REC_HEAD_1))
        {
            SYSTEM_ERROR("XX A5 data is not right 0X%x\n",readBuf[1]);
            continue;
        }

        if ((QueuePop(pRcvQueue, &readBuf[2]) == QUEUE_EMPTY) || (readBuf[2] > PACKET_DATA_LEN_MAX))
        {
            SYSTEM_ERROR("num count data is not right1,%d \n",readBuf[2]);
            continue;
        }

        if((readBuf[2] + 3U) > readBufMaxLen)
        {
            SYSTEM_ERROR("parse frame buf overflow!\n");
            continue;
        }

        qcp = QueuePopArray(pRcvQueue, &readBuf[3], readBuf[2]);
        if (qcp!= readBuf[2])
        {
            SYSTEM_ERROR("num count data is not right2,%d ,%d \n",qcp,readBuf[2]);
            continue;
        }

#if c485_232_CRC16
        if (CalCRC16(readBuf, readBuf[2] + 3) != 0)
        {
            SYSTEM_ERROR("CalCRC16 ERROR\n");
            continue;
        }
#endif

        if (readBuf[FUNC_DATA_IDX] == SLAVE_CTRL_CODE)
        {
            if(pInst->pSlaveProcCb != NULL)
            {
                pInst->pSlaveProcCb(readBuf, readBuf[2] + 3U);
            }
        }
    }
#else
    while ((QueueCount(pRcvQueue) > 0U) && (QueuePop(pRcvQueue, &readBuf[0]) == QUEUE_OK))
    {
        SYSTEM_INFO("RAW_RX:0x%02X\n", readBuf[0]);
			pInst->pSlaveProcCb(readBuf, extReadBufMaxLen);
    }
#endif
}


//void Usart485CommonComTask(UartComInstance *pInst)
//{
//    if(pInst == NULL || pInst->pRcvQueue == NULL || pInst->parseBuf == NULL)
//    {
//        return;
//    }

//    QueueType_t *pRcvQueue = pInst->pRcvQueue;
//    uint8_t *readBuf = pInst->parseBuf;
//    uint16_t readBufMaxLen = pInst->parseBufLen;

//    static uint32_t qcp = 0;
//#if 1
//    while ((QueueCount(pRcvQueue) >= PACKET_DATA_LEN_MIN) && (QueuePop(pRcvQueue, &readBuf[0]) == QUEUE_OK))
//    {
//        if (readBuf[0] != FRAME_REC_HEAD_0)
//        {
//            continue;
//        }

//        if ((QueuePop(pRcvQueue, &readBuf[1]) == QUEUE_EMPTY) || (readBuf[1] != FRAME_REC_HEAD_1))
//        {
//            SYSTEM_ERROR("XX A5 data is not right 0X%x\n",readBuf[1]);
//            continue;
//        }

//        if ((QueuePop(pRcvQueue, &readBuf[2]) == QUEUE_EMPTY) || (readBuf[2] > PACKET_DATA_LEN_MAX))
//        {
//            SYSTEM_ERROR("num count data is not right1,%d \n",readBuf[2]);
//            continue;
//        }

//        if((readBuf[2] + 3U) > readBufMaxLen)
//        {
//            SYSTEM_ERROR("parse frame buf overflow!\n");
//            continue;
//        }

//        qcp = QueuePopArray(pRcvQueue, &readBuf[3], readBuf[2]);
//        if (qcp!= readBuf[2])
//        {
//            SYSTEM_ERROR("num count data is not right2,%d ,%d \n",qcp,readBuf[2]);
//            continue;
//        }

//#if c485_232_CRC16
//        if (CalCRC16(readBuf, readBuf[2] + 3) != 0)
//        {
//            SYSTEM_ERROR("CalCRC16 ERROR\n");
//            continue;
//        }
//#endif

//        if (readBuf[FUNC_DATA_IDX] == SLAVE_CTRL_CODE)
//        {
//            if(pInst->pSlaveProcCb != NULL)
//            {
//                pInst->pSlaveProcCb(pRcvQueue);
//            }
//        }
//    }
//		
//#else
//		    while ((QueueCount(pRcvQueue) > 0U) && (QueuePop(pRcvQueue, &readBuf[0]) == QUEUE_OK))
//    {
//        /* 每读出一个字节，立刻打印原始接收字节 */
//        SYSTEM_INFO("RAW_RX:0x%02X\n", readBuf[0]);
//		}
//#endif
//		
//		
//}
/**
 * @brief 帧解析完成回调
 * @param frameBuf  帧缓冲区（可能是实例自带parseBuf，也可能是外部传入的extReadBuf）
 * @param frameLen  整帧总长度(帧头+长度+数据+校验)
 */
 #if USE_COM01_COM485_FUN
static void com01_frame_process(uint8_t *frameBuf, uint16_t frameLen)
{
	#if 0
    if(frameBuf == NULL || frameLen < 3U)
    {
        return;
    }

    // frameBuf[0] = FRAME_REC_HEAD_0
    // frameBuf[1] = FRAME_REC_HEAD_1
    // frameBuf[2] = 数据域长度
    // frameBuf[3] = FUNC_DATA_IDX 功能码

    // ⚠️重要：frameBuf是临时工作缓冲区，下次Usart485CommonComTask执行会被覆盖
    // 如果需要保存这帧数据，必须memcpy拷贝出来，不要直接保存frameBuf指针

    /* 示例业务逻辑 */
    SYSTEM_INFO("recv frame len:%d func:%02X\n", frameLen, frameBuf[3]);
	#else
	SYSTEM_INFO("-%c\n",  frameBuf[0]);
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,frameBuf,frameLen,"com01_frame_process=\r\n");
	
	#endif
}
	#endif
	#if USE_COM02_COM485_FUN
static void com02_frame_process(uint8_t *frameBuf, uint16_t frameLen)
{
	#if 0
    if(frameBuf == NULL || frameLen < 3U)
    {
        return;
    }

    // frameBuf[0] = FRAME_REC_HEAD_0
    // frameBuf[1] = FRAME_REC_HEAD_1
    // frameBuf[2] = 数据域长度
    // frameBuf[3] = FUNC_DATA_IDX 功能码

    // ⚠️重要：frameBuf是临时工作缓冲区，下次Usart485CommonComTask执行会被覆盖
    // 如果需要保存这帧数据，必须memcpy拷贝出来，不要直接保存frameBuf指针

    /* 示例业务逻辑 */
    SYSTEM_INFO("recv frame len:%d func:%02X\n", frameLen, frameBuf[3]);
	#else
	SYSTEM_INFO("-%c\n",  frameBuf[0]);
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,frameBuf,frameLen,"com02_frame_process=\r\n");
	
	#endif
}
	#endif
	#if USE_COM03_COM485_FUN
static void com03_frame_process(uint8_t *frameBuf, uint16_t frameLen)
{
	#if 0
    if(frameBuf == NULL || frameLen < 3U)
    {
        return;
    }

    // frameBuf[0] = FRAME_REC_HEAD_0
    // frameBuf[1] = FRAME_REC_HEAD_1
    // frameBuf[2] = 数据域长度
    // frameBuf[3] = FUNC_DATA_IDX 功能码

    // ⚠️重要：frameBuf是临时工作缓冲区，下次Usart485CommonComTask执行会被覆盖
    // 如果需要保存这帧数据，必须memcpy拷贝出来，不要直接保存frameBuf指针

    /* 示例业务逻辑 */
    SYSTEM_INFO("recv frame len:%d func:%02X\n", frameLen, frameBuf[3]);
	#else
	SYSTEM_INFO("-%c\n",  frameBuf[0]);
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,frameBuf,frameLen,"com03_frame_process=\r\n");
	
	#endif
}
	#endif
	#if USE_COM04_COM485_FUN
static void com04_frame_process(uint8_t *frameBuf, uint16_t frameLen)
{
	#if 0
    if(frameBuf == NULL || frameLen < 3U)
    {
        return;
    }

    // frameBuf[0] = FRAME_REC_HEAD_0
    // frameBuf[1] = FRAME_REC_HEAD_1
    // frameBuf[2] = 数据域长度
    // frameBuf[3] = FUNC_DATA_IDX 功能码

    // ⚠️重要：frameBuf是临时工作缓冲区，下次Usart485CommonComTask执行会被覆盖
    // 如果需要保存这帧数据，必须memcpy拷贝出来，不要直接保存frameBuf指针

    /* 示例业务逻辑 */
    SYSTEM_INFO("recv frame len:%d func:%02X\n", frameLen, frameBuf[3]);
	#else
	SYSTEM_INFO("-%c\n",  frameBuf[0]);
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,frameBuf,frameLen,"com04_frame_process=\r\n");
	
	#endif
}
	#endif
	#if USE_COM05_COM485_FUN
static void com05_frame_process(uint8_t *frameBuf, uint16_t frameLen)
{
	#if 0
    if(frameBuf == NULL || frameLen < 3U)
    {
        return;
    }

    // frameBuf[0] = FRAME_REC_HEAD_0
    // frameBuf[1] = FRAME_REC_HEAD_1
    // frameBuf[2] = 数据域长度
    // frameBuf[3] = FUNC_DATA_IDX 功能码

    // ⚠️重要：frameBuf是临时工作缓冲区，下次Usart485CommonComTask执行会被覆盖
    // 如果需要保存这帧数据，必须memcpy拷贝出来，不要直接保存frameBuf指针

    /* 示例业务逻辑 */
    SYSTEM_INFO("recv frame len:%d func:%02X\n", frameLen, frameBuf[3]);
	#else
	SYSTEM_INFO("-%c\n",  frameBuf[0]);
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,frameBuf,frameLen,"com05_frame_process=\r\n");
	
	#endif
}
	#endif
	#if USE_COM06_COM485_FUN
static void com06_frame_process(uint8_t *frameBuf, uint16_t frameLen)
{
	#if 0
    if(frameBuf == NULL || frameLen < 3U)
    {
        return;
    }

    // frameBuf[0] = FRAME_REC_HEAD_0
    // frameBuf[1] = FRAME_REC_HEAD_1
    // frameBuf[2] = 数据域长度
    // frameBuf[3] = FUNC_DATA_IDX 功能码

    // ⚠️重要：frameBuf是临时工作缓冲区，下次Usart485CommonComTask执行会被覆盖
    // 如果需要保存这帧数据，必须memcpy拷贝出来，不要直接保存frameBuf指针

    /* 示例业务逻辑 */
    SYSTEM_INFO("recv frame len:%d func:%02X\n", frameLen, frameBuf[3]);
	#else
	SYSTEM_INFO("-%c\n",  frameBuf[0]);
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,frameBuf,frameLen,"com06_frame_process=\r\n");
	
	#endif
}
	#endif
	
/**
***********************************************************
* @brief USB转串口应用初始化函数
* @param
* @return 
***********************************************************
*/
void Usart485CommonComAppInit(void)
{
	
	    // 传入函数地址，不要加()
//    UART_INST_SetRxByteCb(&com01_com485Inst, ProcUartData_Common);
	#if USE_COM01_COM485_FUN
	UART_INST_SetSlaveCb(&com01_com485Inst, com01_frame_process);
	#endif
	#if USE_COM02_COM485_FUN
	UART_INST_SetSlaveCb(&com02_com485Inst, com02_frame_process);
	#endif
	#if USE_COM03_COM485_FUN
	UART_INST_SetSlaveCb(&com03_com485Inst, com03_frame_process);
	#endif
	#if USE_COM04_COM485_FUN
	UART_INST_SetSlaveCb(&com04_com485Inst, com04_frame_process);	
	#endif
	#if USE_COM05_COM485_FUN
	UART_INST_SetSlaveCb(&com05_com485Inst, com05_frame_process);
	#endif
	#if USE_COM06_COM485_FUN
	UART_INST_SetSlaveCb(&com06_com485Inst, com06_frame_process);	
	#endif
//	reg485ComCb(ProcUartData);//injectUARTIDLEcptCP
//	reg_SlaveComCb(pull_data_from_485);
//	#if !USE_UART_DMA_RX
//QueueInit(&g_rcvQueue, g_rcvDataBuf, MAX_BUF_SIZE);
//	#endif
}

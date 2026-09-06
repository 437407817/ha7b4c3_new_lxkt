
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

extern STR_RCV_DMA_que_data RcvDmaQue_COM1_Data;
//extern STR_RCV_DMA_que_data Rcv_Common_DmaQueData_Com1;
#define g_com01_rcvDataBuf RcvDmaQue_COM1_Data.g_ringBufData
#define g_com01_rcvQueue RcvDmaQue_COM1_Data.g_uartRingBuf





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
static void ProcUartData_Common(uint8_t data, void *pUserCtx)
{
    UartRxCtx_t *pCtx = (UartRxCtx_t *)pUserCtx;
    if(pCtx == NULL || pCtx->pRcvQueue == NULL)
    {
        return;
    }
    QueuePush(pCtx->pRcvQueue, data);
}




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




void Usart485CommonComTask(UartComInstance *pInst)
{
    if(pInst == NULL || pInst->pRcvQueue == NULL || pInst->parseBuf == NULL)
    {
        return;
    }

    QueueType_t *pRcvQueue = pInst->pRcvQueue;
    uint8_t *readBuf = pInst->parseBuf;
    uint16_t readBufMaxLen = pInst->parseBufLen;

    static uint32_t qcp = 0;

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
                pInst->pSlaveProcCb(pRcvQueue);
            }
        }
    }
}

/**
***********************************************************
* @brief USB转串口应用初始化函数
* @param
* @return 
***********************************************************
*/
void Usart485ComAppInit2(void)
{
	
	    // 传入函数地址，不要加()
    UART_INST_SetRxByteCb(&com01_com485Inst, ProcUartData_Common);
	
	
	
	
//	reg485ComCb(ProcUartData);//injectUARTIDLEcptCP
//	reg_SlaveComCb(pull_data_from_485);
//	#if !USE_UART_DMA_RX
//QueueInit(&g_rcvQueue, g_rcvDataBuf, MAX_BUF_SIZE);
//	#endif
}

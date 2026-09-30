#include "./screen/DW_com_send.h"

#include "./sys/bsp_systime.h"   




#include "./io/bsp_io_output.h" 

#include "./DataConvert/data_convert.h"
#include <stdlib.h>

 #include "./sys/sysio.h"
#include "string.h"

#include "./sys/systick.h"

#include "./usart/bsp_usart_common_com485.h"


















/**
***********************************************************************
包格式：帧头0  帧头1  数据长度  功能字   数据  数据  CRC校验高   CRC校验低
        0x5a   0xa5    0x03      0x06    0x00  0x01      0xFB     0xFB
***********************************************************************
5AA5 05 82 1000 0002 :解释：通过指令往 0000 地址里面赋值 2，屏上的显示，数据变量整数类型 2
5AA5 04 83 1000 01    解释：从 1000 地址开始读 1 个字长度，数据指令最大容许长度 0x7c

*/
//creat_ticktime(1)

//单片机是小端数据模式，该函数会自动转化成大端模式后发送
#define FRAME_SEND_HEAD_0           0x5a  
#define FRAME_SEND_HEAD_1           0xa5


#define DW_SCREEN_comInst           COM06_com485Inst
//#define DW_CRC16   c485_CRC16


//5AA5 04 83 1002 01是读单个1002
//5AA5 04 83 1002 02是读1002，1003，连续的
//如果想写多个，5AA5 07 82 1002 1234 5678
//5AA5 04 83 1002 02是读1002，1003，连续的,返回,5AA5 08 83 1002 02 0001 0002



/**
***********************************************************
* @brief 485输出任务处理函数（长度单字节）
* @param
* @return 
***********************************************************
*/
//单片机是小端数据模式，该函数会自动转化成大端模式后发送
void DW_SCREEN_write_cmd(uint16_t addr, uint16_t *buf, uint16_t size) {      //82

//	Cal_start_ticktime(1);
	uint8_t *frame = NULL;
	#if (DW_CRC16==1)	
	uint16_t crc16_res;
		#endif
	//frame = (uint8_t *)mymalloc(SRAMIN, size*2 + 8);
	
	#if (DW_CRC16==1)
	frame = (uint8_t *)malloc(size*2 + 8);
	#else
	frame = (uint8_t *)malloc(size*2 + 6);
	#endif
//	SYSTEM_INFO("SCREEN_write_cmd 1  %d \n",Calculate_diffRunTime(&starttime2,&overtime2));
	
	if(frame == NULL) {
		SYSTEM_ERROR("DW_CRC16 memory malloc wrong\n");
		return;
	}
	
	frame[0] = FRAME_SEND_HEAD_0;
	frame[1] = FRAME_SEND_HEAD_1;
	#if (DW_CRC16==1)	
	frame[2] = size*2 + 5;
	#else	
	frame[2] = size*2 + 3;
	#endif	
	frame[3] = 0x82;
	frame[4] = (uint8_t)((addr & 0xff00) >> 8);
	frame[5] = (uint8_t)(addr & 0xff);
	
	for(int i=0;i<size;i++) {
		frame[2*i+6] = (uint8_t)((buf[i] & 0xff00) >> 8);
		frame[2*i+7] = (uint8_t)(buf[i] & 0xff);
	}
		_485_A_TX_EN();
	#if (DW_CRC16==1)
	crc16_res = crc_16(&frame[3], frame[2]-2);
	
	frame[size*2 + 6] = (uint8_t)(crc16_res & 0xff);
	frame[size*2 + 7] = (uint8_t)((crc16_res & 0xff00) >> 8);
	
//SYSTEM_DEBUG("crc2=0x%x 0x%x 0x%x",crc16_res,frame[size*2 + 6],frame[size*2 + 7]);
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 8,"write TO SCREEN =");

	Usart_SendArray(&huart_485_Handle,frame,size*2 + 8);
	
		#else
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 6,"SCREEN write 82 write TO SCREEN =");
//	SYSTEM_INFO("SCREEN_write_cmd 2  %d \n",Calculate_diffRunTime(&starttime2,&overtime2));
//	Cal_diff_ticktime(1);
//	Usart_SendArray(&huart_485_Handle,frame,size*2 + 6);
	UART_COMMON_Instance_SendArray_DMA(&DW_SCREEN_comInst,frame, size*2 + 6);
  #endif
	//HAL_UART_Transmit(&huart1,frame ,size*2 + 8, 1000);


//	SYSTEM_INFO("SCREEN_write_cmd 3  %d \n",Calculate_diffRunTime(&starttime2,&overtime2));
	
	_485_A_RX_EN();
free(frame);
	frame = NULL;
	//myfree(SRAMIN, frame);
}



//单片机是小端数据模式，该函数会自动转化成大端模式后发送
void DW_SCREEN_write_cmd_print(uint16_t addr, uint16_t *buf, uint16_t size, uint16_t debugprint) {      //82

	uint8_t *frame = NULL;
	#if (DW_CRC16==1)	
	uint16_t crc16_res;
		#endif
	//frame = (uint8_t *)mymalloc(SRAMIN, size*2 + 8);
	
	#if (DW_CRC16==1)
	frame = (uint8_t *)malloc(size*2 + 8);
	#else
	frame = (uint8_t *)malloc(size*2 + 6);
	#endif
	
	
	if(frame == NULL) {
		SYSTEM_ERROR("DW_CRC16 memory malloc wrong\n");
		return;
	}
	
	frame[0] = FRAME_SEND_HEAD_0;
	frame[1] = FRAME_SEND_HEAD_1;
	#if (DW_CRC16==1)	
	frame[2] = size*2 + 5;
	#else	
	frame[2] = size*2 + 3;
	#endif	
	frame[3] = 0x82;
	frame[4] = (uint8_t)((addr & 0xff00) >> 8);
	frame[5] = (uint8_t)(addr & 0xff);
	
	for(int i=0;i<size;i++) {
		frame[2*i+6] = (uint8_t)((buf[i] & 0xff00) >> 8);
		frame[2*i+7] = (uint8_t)(buf[i] & 0xff);
	}
	_485_A_TX_EN();
	#if (DW_CRC16==1)
	crc16_res = crc_16(&frame[3], frame[2]-2);
	
	frame[size*2 + 6] = (uint8_t)(crc16_res & 0xff);
	frame[size*2 + 7] = (uint8_t)((crc16_res & 0xff00) >> 8);
	
//SYSTEM_DEBUG("crc2=0x%x 0x%x 0x%x",crc16_res,frame[size*2 + 6],frame[size*2 + 7]);
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 8,"write TO SCREEN =");
	
	Usart_SendArray(&huart_485_Handle,frame,size*2 + 8);
	
		#else
	if(debugprint==1){
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 6,"SCREEN write 82 data TO SCREEN ===");
	}
//	

	UART_COMMON_Instance_SendArray_DMA(&DW_SCREEN_comInst,frame, size*2 + 6);
  #endif
	_485_A_RX_EN();
	//HAL_UART_Transmit(&huart1,frame ,size*2 + 8, 1000);

	
	
	
	
free(frame);
	frame = NULL;
	//myfree(SRAMIN, frame);
}






void DW_SCREEN_read_cmd_print(uint16_t addr, uint8_t num, uint16_t debugprint) {      //83

	uint8_t *frame = NULL;
	#if (DW_CRC16==1)	
	uint16_t crc16_res;
	#endif
	if(num > 0x7c) {
				SYSTEM_ERROR("Data read is too much than 0x7c\n");
		return;
	}
	
	
	#if (DW_CRC16==1)	
	frame = (uint8_t *)malloc(9);
	#else
	frame = (uint8_t *)malloc(7);
	#endif	
	if(frame == NULL) {
				SYSTEM_ERROR("memory malloc wrong\n");
		return;
	}
	
	frame[0] = FRAME_SEND_HEAD_0;
	frame[1] = FRAME_SEND_HEAD_1;
#if (DW_CRC16==1)	
	frame[2] = 0x06;
#else
	frame[2] = 0x04;
#endif	
	frame[3] = 0x83;
	frame[4] = (uint8_t)((addr & 0xff00) >> 8);
	frame[5] = (uint8_t)(addr & 0xff);
	frame[6] = num;
	_485_A_TX_EN();
	#if (DW_CRC16==1)	
	crc16_res = crc_16(&frame[3], frame[2]-2);
	
	frame[7] = (uint8_t)(crc16_res & 0xff);	
	frame[8] = (uint8_t)((crc16_res & 0xff00) >> 8);
		
		Usart_SendArray(&huart_485_Handle,frame,9);
	
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,9,"Usart_SendArray 83 read TO SCREEN =");
	#else
	
		
		UART_COMMON_Instance_SendArray_DMA(&DW_SCREEN_comInst,frame, 7);
			if(debugprint==1){
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,7,"SCREEN write 0x83 read data TO SCREEN ===");
	}
	//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,7,"SCREEN read 83 read TO SCREEN =");
  #endif
	_485_A_RX_EN();

free(frame);
	frame = NULL;
}










//单片机是小端数据模式，该函数会自动转化成大端模式后发送
void DW_DGUSI_SCREEN_write_Reg_cmd_print(uint8_t REGaddr, uint16_t REGADATA,  uint16_t debugprint) {      //80

	uint8_t *frame = NULL;
	#if (DW_CRC16==1)	
	uint16_t crc16_res;
		#endif
	//frame = (uint8_t *)mymalloc(SRAMIN, size*2 + 8);
	
	#if (DW_CRC16==1)
	frame = (uint8_t *)malloc(2 + 7);
	#else
	frame = (uint8_t *)malloc(2 + 5);
	#endif
	
	
	if(frame == NULL) {
		SYSTEM_ERROR("DW_CRC16 memory malloc wrong\n");
		return;
	}
	
	frame[0] = FRAME_SEND_HEAD_0;
	frame[1] = FRAME_SEND_HEAD_1;
	#if (DW_CRC16==1)	
	frame[2] = 6;
	#else	
	frame[2] = 4;
	#endif	
	frame[3] = 0x80;
	frame[4] = REGaddr;
//	frame[5] = (uint8_t)(addr & 0xff);
	
	
		frame[5] = (uint8_t)((REGADATA & 0xff00) >> 8);
		frame[6] = (uint8_t)(REGADATA & 0xff);
	
	_485_A_TX_EN();
	#if (DW_CRC16==1)
	crc16_res = crc_16(&frame[3], frame[2]-2);
	
	frame[7] = (uint8_t)(crc16_res & 0xff);
	frame[8] = (uint8_t)((crc16_res & 0xff00) >> 8);
	
//SYSTEM_DEBUG("crc2=0x%x 0x%x 0x%x",crc16_res,frame[size*2 + 5],frame[size*2 + 6]);
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,size*2 + 7,"write TO SCREEN =");
	
	Usart_SendArray(&huart_485_Handle,frame,9);
	
		#else
	if(debugprint==1){
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,7,"SCREEN write 80 data TO SCREEN ===");
	}
//	
	
	UART_COMMON_Instance_SendArray_DMA(&DW_SCREEN_comInst,frame, 7);
  #endif
	_485_A_RX_EN();
	//HAL_UART_Transmit(&huart1,frame ,size*2 + 8, 1000);

	
	
	
	
free(frame);
	frame = NULL;
	//myfree(SRAMIN, frame);
}
















/*********************************************END OF FILE**********************/

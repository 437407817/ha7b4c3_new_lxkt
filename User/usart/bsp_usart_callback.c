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
  
#include "./usart/bsp_usart_callback.h"

#include "./usart/bsp_usart_dma.h"
#include "./usart/bsp_usart_shell.h"
#include "./usart/bsp_usart.h"
 #include "./shell_port.h"
#include "./user_config.h"
#include "./usart/bsp_usart_common_dma.h"

// DMA 接收完成回调（循环模式下，缓冲区满后触发）
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//  if (huart == &huart_a) {
//    HAL_UART_a_RxCpltCallback();
//  }else 

//	SYSTEM_INFO("-");
	#if USE_LETTER_SHELL||(TEST_SHELL_UART&&!USE_IT_1)
	if(huart == &huart_shell_Handle){
//		SYSTEM_INFO("*");
	HAL_UART_Shell_RxCpltCallback(huart);
	}
	#endif
	
#if USE_UART_DMA&&USE_UART_DMA_RX
if (huart == &huart_DMA_Handle) {
HAL_USARTx_DMA_RxCpltCallback();
}
#endif

}


//发送0个数据不会调用该函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
#if USE_UART_DMA&&USE_UART_DMA_TX
  if (huart == &huart_DMA_Handle) {
HAL_USARTx_DMA_TxCpltCallback();
  }
#endif
	
#if (USE_UART_COMMON_COM01_DMA&&USE_UART_COMMON_COM01_DMA_TX)||(USE_UART_COMMON_COM02_DMA&&USE_UART_COMMON_COM02_DMA_TX)||\
	(USE_UART_COMMON_COM03_DMA&&USE_UART_COMMON_COM03_DMA_TX)||(USE_UART_COMMON_COM04_DMA&&USE_UART_COMMON_COM04_DMA_TX)\
	||(USE_UART_COMMON_COM05_DMA&&USE_UART_COMMON_COM05_DMA_TX)||(USE_UART_COMMON_COM06_DMA&&USE_UART_COMMON_COM06_DMA_TX)
	HAL_UART_COMMON_TxCpltCallback(huart);
#endif	
//	#if USE_UART_COMMON_COM02_DMA&&USE_UART_COMMON_COM02_DMA_TX	
//	HAL_UART_COMMON_TxCpltCallback(huart);
//#endif	
	
}






//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
//#if USE_UART_DMA&&!USE_LETTER_SHELL&&!USE_OS
//if (huart == &huart_DMA_Handle) {
//	HAL_USARTx_DMA_ErrorCallback();
//  }
//#endif
//	uint8_t error_code=0;
//	  uint32_t err = huart->ErrorCode;
//  if(err & HAL_UART_ERROR_PE)  // 0x01 奇偶校验错误
//  {
//		error_code=0x01;
//  }
//  if(err & HAL_UART_ERROR_NE)  // 0x02 噪声错误，线上有干扰
//  {
//		error_code=0x02;
//  }
//  if(err & HAL_UART_ERROR_FE)  // 0x04 帧错误，停止位异常，最常见：波特率不匹配
//  {
//		error_code=0x04;
//  }
//  if(err & HAL_UART_ERROR_ORE) // 0x08 溢出错误！新数据来了，上一个字节还没读走【最高发】
//  {
//		error_code=0x08;
//  }
//  if(err & HAL_UART_ERROR_DMA) // 0x10 DMA传输错误（DMA接收时才会出现）
//  {
//		error_code=0x10;
//  }
//  if(err & HAL_UART_ERROR_RTO) // 0x20 接收超时
//  {
//		error_code=0x20;
//  }
////	    uint32_t error_code = HAL_UART_GetError(huart);  // 获取错误码
//    SYSTEM_DEBUG("--UART Error: 0x%02x\n", error_code);  // 打印错误（需重定向printf）
//	SYSTEM_INFO("\r\n----HAL_UART_ErrorCallback---\r\n");
//	#if USE_LETTER_SHELL&&USE_OS
//	if(huart == &huart_shell_Handle){
//	HAL_UART_Shell_ErrorCallback(huart);
//	}
//	#endif
//}





void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	uint8_t error_code=0;
  uint32_t err = huart->ErrorCode;
  if(err & HAL_UART_ERROR_PE)  error_code=0x01;
  if(err & HAL_UART_ERROR_NE)  error_code=0x02;
  if(err & HAL_UART_ERROR_FE)  error_code=0x04;
  if(err & HAL_UART_ERROR_ORE) error_code=0x08;
  if(err & HAL_UART_ERROR_DMA) error_code=0x10;
  if(err & HAL_UART_ERROR_RTO) error_code=0x20;

	SYSTEM_DEBUG("--UART Error: 0x%02x\n", error_code);
	SYSTEM_INFO("\r\n----HAL_UART_ErrorCallback---\r\n");

	//清除硬件错误标志位
	__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE | UART_FLAG_NE | UART_FLAG_FE);

#if USE_UART_COMMON_COM01_DMA_RX
	if(huart->Instance == DMA_COM01_USARTx)
	{
		HAL_UART_DMAStop(huart);
		HAL_UART_Receive_DMA(huart, RcvDmaQue_COM01_Data.g_rcvDataBuf, MAX_BUF_COM01_R_SIZE);
	}
#endif
#if USE_UART_COMMON_COM02_DMA_RX
	if(huart->Instance == DMA_COM02_USARTx)
	{
		HAL_UART_DMAStop(huart);
		HAL_UART_Receive_DMA(huart, RcvDmaQue_COM02_Data.g_rcvDataBuf, MAX_BUF_COM02_R_SIZE);
	}
#endif
#if USE_UART_COMMON_COM03_DMA_RX
	if(huart->Instance == DMA_COM03_USARTx)
	{
		HAL_UART_DMAStop(huart);
		HAL_UART_Receive_DMA(huart, RcvDmaQue_COM03_Data.g_rcvDataBuf, MAX_BUF_COM03_R_SIZE);
	}
#endif
#if USE_UART_COMMON_COM04_DMA_RX
	if(huart->Instance == DMA_COM04_USARTx)
	{
		HAL_UART_DMAStop(huart);
		HAL_UART_Receive_DMA(huart, RcvDmaQue_COM04_Data.g_rcvDataBuf, MAX_BUF_COM04_R_SIZE);
	}
#endif
#if USE_UART_COMMON_COM05_DMA_RX
	if(huart->Instance == DMA_COM05_USARTx)
	{
		HAL_UART_DMAStop(huart);
		HAL_UART_Receive_DMA(huart, RcvDmaQue_COM05_Data.g_rcvDataBuf, MAX_BUF_COM05_R_SIZE);
	}
#endif
#if USE_UART_COMMON_COM06_DMA_RX
	if(huart->Instance == DMA_COM06_USARTx)
	{
		HAL_UART_DMAStop(huart);
		HAL_UART_Receive_DMA(huart, RcvDmaQue_COM06_Data.g_rcvDataBuf, MAX_BUF_COM06_R_SIZE);
	}
#endif

	#if USE_LETTER_SHELL&&USE_OS
	if(huart == &huart_shell_Handle){
	HAL_UART_Shell_ErrorCallback(huart);
	}
	#endif
}












/*********************************************END OF FILE**********************/

#ifndef _USART485COMMONVERIFY_H_
#define _USART485COMMONVERIFY_H_

#include <stdint.h>



#include "./pro_com/usart_485_232_send.h"

 #include "./usart/bsp_usart_common_com485.h"





/**
 * @brief 485协议解析任务，传入实例指针
 */
void Usart485CommonComTask(UartComInstance *pInst);
















#endif

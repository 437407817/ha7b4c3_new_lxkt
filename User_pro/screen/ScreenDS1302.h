#ifndef __SCREENDS1302_H
#define	__SCREENDS1302_H


#include "stm32_FH_xxx.h"

//#include "./usart/p_data_queue.h"


void PageSysTimeSettingSave(uint8_t num,uint8_t** p_data);
void DS1302_getTime_Fun(void);
	
void ScreenSYS_SettingParaSend(void);

#endif /* __SCREENDS1302_H */


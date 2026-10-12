#ifndef __CVT_TIME_H
#define	__CVT_TIME_H

#include "stm32_FH_xxx.h"
#include <time.h>






















time_t CalculateTimestamp(uint16_t t_year,uint16_t t_month,uint16_t t_day,uint16_t t_hour,uint16_t t_minute,uint16_t t_second);
time_t CalculateTimestampDifference(time_t start_timestamp,uint16_t t_year,uint16_t t_month,uint16_t t_day,uint16_t t_hour,uint16_t t_minute,uint16_t t_second);
void CalculateTimestampToTime(time_t seconds,uint16_t* t_year,uint16_t* t_month,uint16_t* t_day,uint16_t* t_hour,uint16_t* t_minute,uint16_t* t_second);






#endif

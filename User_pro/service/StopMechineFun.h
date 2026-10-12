#ifndef __STOPMECHINEFUN_H
#define	__STOPMECHINEFUN_H

#include "stm32_FH_xxx.h"










//void StopBoardFunByBoardId(uint8_t launchboardnum);
void StopBoardFunByBoardId(uint8_t launchboardnum,uint8_t ifsend);
void ChangeNboardTo_stopORstart_IconState(uint8_t launchboardnum,uint16_t stopORstart);

void LedClose_ByBoardidFun(uint8_t boardid);
void ChangeNboardTo_stopORstart_ALL_IconState(uint16_t stopORstart);

void LedClose_All_ByBoardidFun(void);


void AutoChangeNboardTo_stopORstart_ALL_IconState(void);
#endif /* __DISPOSEDATAFUN_H */










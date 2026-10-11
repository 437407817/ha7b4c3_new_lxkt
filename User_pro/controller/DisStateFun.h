
#ifndef __DISSTATEFUN_H
#define	__DISSTATEFUN_H


#include "stm32_FH_xxx.h"



void RunWorkMode_ChangeIn(uint8_t changeboard_id);
//void MaintainState_Pause(uint8_t boardnum);

//void MaintainState_PauseResume(uint8_t boardnum);

void MaintainStateRun_Start(uint8_t launchboardnum);
void MaintainStateRun_SingleBoard_Stop(uint8_t launchboardnum,uint8_t ifsend);
void Mode_DisCharge_Stop(void);
void MaintainStateRun_AllBoard_Stop(uint8_t ifsend);
void Mode_MaintainState_Delay_Stop(void);

void clearAllErrorBoardNoFlag(void);
void CheckErrorStop(void);

#endif /* __DISWORKFUN_H */






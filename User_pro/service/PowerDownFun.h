


#ifndef __POWERDOWNFUN_H
#define __POWERDOWNFUN_H



#include "stm32_FH_xxx.h"





void initPowerDownData(void);
void disposeLaunchPageIfJumptoPowerDownPage(void);
void ResumeLaunchPowerDownFun(void);
void CancelLaunchPowerDownFun(void);
void launch_Machine_PowerDownUpdatetmpdata(void);




void TEST_PowerDownSAVE(void);









#endif /* __STM32Fxxx_H */

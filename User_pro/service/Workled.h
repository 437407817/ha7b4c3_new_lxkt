#ifndef __WORKLED_H
#define	__WORKLED_H


#include "stm32fxxx.h"
#include "./i2c/hard_i2c/bsp_i2c_ee.h"
#include "./i2c/soft_i2c/bsp_soft_i2c_ee.h"
#include "./GV_variable.h"


  #include "./system_config.h"
#include "./defines.h"

uint8_t turnOnLight(uint8_t lightNumber) ;

uint8_t turnOffLight(uint8_t lightNumber) ;

uint8_t turnOffAllLights(void);



uint8_t getLightState(uint8_t lightNumber) ;

uint8_t getAllLightsState(void) ;

void updateHardwareLightState(uint8_t lightNumber, uint8_t state) ;
void updateAllHardwareLightsState(uint8_t stateMask);

#define SYSTEM_getAllLightsStateCounters   SYSTEM_DEBUG("SYSTEM_getAllLightsStateCounters");SYSTEM_getAllLightsStateCountersfun
void SYSTEM_getAllLightsStateCountersfun(void) ;



#endif /* __WORKLED_H */


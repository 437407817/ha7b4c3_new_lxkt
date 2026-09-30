
#ifndef __HARDWARE_INTERFACE_API_H
#define	__HARDWARE_INTERFACE_API_H

#include "./stm32_FH_xxx_hal.h"

#include "./io/bsp_io_output.h"   
#include "./sys/sysio.h"


//前面板灯定义
#define Front_Panel_LED_ON(X)            	HAL_GPIO_WritePin(LED##X##_BOARD_GPIO_PORT, LED##X##_BOARD_PIN, GPIO_PIN_SET)
#define Front_Panel_LED_OFF(X)            HAL_GPIO_WritePin(LED##X##_BOARD_GPIO_PORT, LED##X##_BOARD_PIN, GPIO_PIN_RESET)

#define Turnled_ON(X) 	 Front_Panel_LED_ON(X)
#define Turnled_OFF(X)  Front_Panel_LED_OFF(X)


#define TurnParrelCharge_led_OFF  Turnled_OFF(1)
#define TurnParrelCharge_led_ON  Turnled_ON(1);TurnFinish_led_OFF

#define TurnParrelDisCharge_led_OFF  Turnled_OFF(2)
#define TurnParrelDisCharge_led_ON  Turnled_ON(2);TurnFinish_led_OFF

#define TurnMaintain_led_OFF  Turnled_OFF(3)
#define TurnMaintain_led_ON  Turnled_ON(3);TurnFinish_led_OFF

#define TurnFinish_led_OFF  Turnled_OFF(4)
#define TurnFinish_led_ON  turnOffAllLights();SYSTEM_DEBUG("----turnOffAllLights----");Turnled_ON(4)

#define TurnWrong_led_OFF  Turnled_OFF(5)
#define TurnWrong_led_ON  Turnled_ON(5);SYSTEM_DEBUG("----turnOffAllLights----");TurnFinish_led_OFF

//#define TurnWork_led_OFF  Turnled_OFF(1);Turnled_OFF(2);Turnled_OFF(3);Turnled_OFF(4)
#define TurnWork_led_OFF  turnOffAllLights();SYSTEM_DEBUG("----turnOffAllLights----");Turnled_OFF(4)





#define GetLed_STATE(X)         HAL_GPIO_ReadPin(LED##X##_BOARD_GPIO_PORT, LED##X##_BOARD_PIN)




#define FAN_ON(X)            	HAL_GPIO_WritePin(FAN##X##_GPIO_PORT, FAN##X##_PIN, GPIO_PIN_SET)
#define FAN_OFF(X)            HAL_GPIO_WritePin(FAN##X##_GPIO_PORT, FAN##X##_PIN, GPIO_PIN_RESET)


//-----------------------------


#define TurnFan_ON		FAN_ON(1);FAN_ON(2);FAN_ON(3);FAN_ON(4)
#define TurnFan_OFF		FAN_OFF(1);FAN_OFF(2);FAN_OFF(3);FAN_OFF(4)











#endif
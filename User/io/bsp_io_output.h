#ifndef __BSP_IO_OUTPUT_H
#define	__BSP_IO_OUTPUT_H

#include "./stm32_FH_xxx_hal.h"

//引脚定义
/*******************************************************/
//R 红色灯
#define LED1_PIN                  GPIO_PIN_0                 
#define LED1_GPIO_PORT            GPIOB                      
#define LED1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

//G 绿色灯
#define LED2_PIN                  GPIO_PIN_1                 
#define LED2_GPIO_PORT            GPIOB                      
#define LED2_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

//B 蓝色灯
#define LED3_PIN                  GPIO_PIN_3                 
#define LED3_GPIO_PORT            GPIOA                       
#define LED3_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

//小指示灯
#define LED4_PIN                  GPIO_PIN_11                 
#define LED4_GPIO_PORT            GPIOD                       
#define LED4_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()
/************************************************************/


//R 红色灯
#define LED01_BOARD_PIN                  GPIO_PIN_0                 
#define LED01_BOARD_GPIO_PORT            GPIOB                      
#define LED01_BOARD_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

//G 绿色灯
#define LED02_BOARD_PIN                  GPIO_PIN_1                 
#define LED02_BOARD_GPIO_PORT            GPIOB                      
#define LED02_BOARD_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

//B 蓝色灯
#define LED03_BOARD_PIN                  GPIO_PIN_3                 
#define LED03_BOARD_GPIO_PORT            GPIOA                       
#define LED03_BOARD_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

//小指示灯
#define LED04_BOARD_PIN                  GPIO_PIN_11                 
#define LED04_BOARD_GPIO_PORT            GPIOD                       
#define LED04_BOARD_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

//小指示灯
#define LED05_BOARD_PIN                  GPIO_PIN_11                 
#define LED05_BOARD_GPIO_PORT            GPIOD                       
#define LED05_BOARD_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()



//代码里宏定义示例
#define FAN01_PIN        						GPIO_PIN_4
#define FAN01_GPIO_PORT  						GPIOA
#define FAN01_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define FAN02_PIN        						GPIO_PIN_4
#define FAN02_GPIO_PORT  						GPIOA
#define FAN02_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define FAN03_PIN        						GPIO_PIN_4
#define FAN03_GPIO_PORT  						GPIOA
#define FAN03_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define FAN04_PIN        						GPIO_PIN_4
#define FAN04_GPIO_PORT  						GPIOA
#define FAN04_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()



#define RE01_PIN        						GPIO_PIN_4
#define RE01_GPIO_PORT  						GPIOA
#define RE01_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define RE02_PIN        						GPIO_PIN_4
#define RE02_GPIO_PORT  						GPIOA
#define RE02_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define RE03_PIN        						GPIO_PIN_4
#define RE03_GPIO_PORT  						GPIOA
#define RE03_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define RE04_PIN        						GPIO_PIN_4
#define RE04_GPIO_PORT  						GPIOA
#define RE04_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()


#define IO485_01_PIN        						GPIO_PIN_4
#define IO485_01_GPIO_PORT  						GPIOA
#define IO485_01_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define IO485_02_PIN        						GPIO_PIN_4
#define IO485_02_GPIO_PORT  						GPIOA
#define IO485_02_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define IO485_03_PIN        						GPIO_PIN_4
#define IO485_03_GPIO_PORT  						GPIOA
#define IO485_03_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define IO485_04_PIN        						GPIO_PIN_4
#define IO485_04_GPIO_PORT  						GPIOA
#define IO485_04_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()









/** 控制LED灯亮灭的宏，
	* LED低电平亮，设置ON=0，OFF=1
	* 若LED高电平亮，把宏设置成ON=1 ，OFF=0 即可
	*/
#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)	HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN,a)


#define LED2(a)	HAL_GPIO_WritePin(LED2_GPIO_PORT,LED2_PIN,a)


#define LED3(a)	HAL_GPIO_WritePin(LED2_GPIO_PORT,LED3_PIN,a)


#define LED4(a)	HAL_GPIO_WritePin(LED4_GPIO_PORT,LED4_PIN,a)


/* 直接操作寄存器的方法控制IO */
//#define	digitalHi(p,i)			{p->BSRR=i;}			  //设置为高电平		
//#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}				//输出低电平
//#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		HAL_GPIO_TogglePin(LED1_GPIO_PORT,LED1_PIN)
#define LED1_OFF		HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET)//digitalHi(LED1_GPIO_PORT,LED1_PIN)
#define LED1_ON			HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET) //digitalLo(LED1_GPIO_PORT,LED1_PIN)

#define LED2_TOGGLE		HAL_GPIO_TogglePin(LED2_GPIO_PORT,LED2_PIN)
#define LED2_OFF		HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET)
#define LED2_ON			HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET)

#define LED3_TOGGLE		HAL_GPIO_TogglePin(LED3_GPIO_PORT,LED3_PIN)
#define LED3_OFF		HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_SET)
#define LED3_ON			HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET)

#define LED4_TOGGLE		digitalToggle(LED4_GPIO_PORT,LED4_PIN)
#define LED4_OFF		HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_SET)
#define LED4_ON			HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_RESET)


/* 基本混色，后面高级用法使用PWM可混出全彩颜色,且效果更好 */





void IO_LED_GPIO_Config(void);
void OUT_Io_GPIO_Config(void);


void Test_Led_RunningWater(void);

#endif /* __LED_H */







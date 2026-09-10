#ifndef __BSP_IO_INPUT_H
#define	__BSP_IO_INPUT_H

#include "./stm32_FH_xxx_hal.h"







typedef struct
{
    GPIO_TypeDef        *port;              //GPIO端口
    uint16_t            pin;                //GPIO_PIN_x
    void                (*clkEnFunc)(void);  //端口时钟使能函数
    uint32_t            itMode;             //GPIO_MODE_IT_RISING / GPIO_MODE_IT_FALLING / GPIO_MODE_IT_RISING_FALLING
    uint32_t            pullMode;           //GPIO_NOPULL / GPIO_PULLUP / GPIO_PULLDOWN
    IRQn_Type           irqN;               //EXTI的中断号 EXTI0_IRQn ... EXTI15_10_IRQn
    uint32_t            prePriority;        //抢占优先级
    uint32_t            subPriority;         //子优先级
} IO_EXTI_Config_t;






#define IO1_INT_GPIO_PORT         GPIOA
#define IO1_INT_GPIO_PIN          GPIO_PIN_0
#define IO1_INT_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define IO1_INT_EXTI_IRQ          EXTI0_IRQn
#define IO1_IRQHandler                   EXTI0_IRQHandler



#define IO2_INT_GPIO_PORT         GPIOB
#define IO2_INT_GPIO_PIN          GPIO_PIN_1
#define IO2_INT_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define IO2_INT_EXTI_IRQ          EXTI1_IRQn
#define IO2_IRQHandler                   EXTI15_10_IRQHandler

#define IO3_INT_GPIO_PORT         GPIOA
#define IO3_INT_GPIO_PIN          GPIO_PIN_0
#define IO3_INT_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define IO3_INT_EXTI_IRQ          EXTI0_IRQn
#define IO3_IRQHandler                   EXTI1_IRQHandler



#define IO4_INT_GPIO_PORT         GPIOB
#define IO4_INT_GPIO_PIN          GPIO_PIN_1
#define IO4_INT_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define IO4_INT_EXTI_IRQ          EXTI1_IRQn
#define IO4_IRQHandler                   EXTI2_IRQHandler
//引脚定义
/*******************************************************/
//#define KEY1_INT_GPIO_PORT                GPIOA
//#define KEY1_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE();
//#define KEY1_INT_GPIO_PIN                 GPIO_PIN_0
//#define KEY1_INT_EXTI_IRQ                 EXTI0_IRQn
//#define KEY1_IRQHandler                   EXTI0_IRQHandler

//#define KEY2_INT_GPIO_PORT                GPIOC
//#define KEY2_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE();
//#define KEY2_INT_GPIO_PIN                 GPIO_PIN_13
//#define KEY2_INT_EXTI_IRQ                 EXTI15_10_IRQn
//#define KEY2_IRQHandler                   EXTI15_10_IRQHandler

/*******************************************************/














void IO_EXTI_Key_Config(void);





void IN_Io_ALL_GPIO_Config(void);


#endif /* __BSP_IO_INPUT_H */

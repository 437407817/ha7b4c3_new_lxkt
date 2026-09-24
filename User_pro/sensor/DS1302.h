
#ifndef __DS1302_H
#define	__DS1302_H


#include "./stm32_FH_xxx_hal.h"
#include "./sys/bsp_systime.h"   




// 假设DS1302连接到GPIOB的某些引脚
//#define DS1302_RST_GPIO_PORT   GPIOE
//#define DS1302_RST_GPIO_Pin    GPIO_Pin_3
//#define DS1302_RST_GPIO_CLK    RCC_AHB1Periph_GPIOE

//#define DS1302_CLK_GPIO_PORT   GPIOI
//#define DS1302_CLK_GPIO_Pin    GPIO_Pin_7
//#define DS1302_CLK_GPIO_CLK    RCC_AHB1Periph_GPIOI

//#define DS1302_DAT_GPIO_PORT   GPIOE
//#define DS1302_DAT_GPIO_Pin    GPIO_Pin_2
//#define DS1302_DAT_GPIO_CLK    RCC_AHB1Periph_GPIOE









//#include "stm32f4xx.h"

// DS1302引脚定义 H743?HAL
#define DS1302_SCLK_GPIO_PORT    GPIOE
#define DS1302_SCLK_GPIO_PIN     GPIO_PIN_2
#define DS1302_SCLK_GPIO_CLK     __HAL_RCC_GPIOE_CLK_ENABLE()

#define DS1302_IO_GPIO_PORT      GPIOE
#define DS1302_IO_GPIO_PIN       GPIO_PIN_3
#define DS1302_IO_GPIO_CLK       __HAL_RCC_GPIOE_CLK_ENABLE()

#define DS1302_RST_GPIO_PORT     GPIOE
#define DS1302_RST_GPIO_PIN      GPIO_PIN_4
#define DS1302_RST_GPIO_CLK      __HAL_RCC_GPIOE_CLK_ENABLE()

// HAL库引脚操作宏
#define DS1302_SCLK_H    HAL_GPIO_WritePin(DS1302_SCLK_GPIO_PORT,DS1302_SCLK_GPIO_PIN,GPIO_PIN_SET)
#define DS1302_SCLK_L    HAL_GPIO_WritePin(DS1302_SCLK_GPIO_PORT,DS1302_SCLK_GPIO_PIN,GPIO_PIN_RESET)

#define DS1302_IO_H      HAL_GPIO_WritePin(DS1302_IO_GPIO_PORT,DS1302_IO_GPIO_PIN,GPIO_PIN_SET)
#define DS1302_IO_L      HAL_GPIO_WritePin(DS1302_IO_GPIO_PORT,DS1302_IO_GPIO_PIN,GPIO_PIN_RESET)

#define DS1302_RST_H     HAL_GPIO_WritePin(DS1302_RST_GPIO_PORT,DS1302_RST_GPIO_PIN,GPIO_PIN_SET)
#define DS1302_RST_L     HAL_GPIO_WritePin(DS1302_RST_GPIO_PORT,DS1302_RST_GPIO_PIN,GPIO_PIN_RESET)

#define DS1302_io_read     HAL_GPIO_ReadPin(DS1302_IO_GPIO_PORT, DS1302_IO_GPIO_PIN)


#define DS1302_DELAY     delay_us(1)



// DS1302寄存器地址定义
#define DS1302_SEC_REG_ADDR      0x80
#define DS1302_MIN_REG_ADDR      0x82
#define DS1302_HOUR_REG_ADDR     0x84
#define DS1302_DATE_REG_ADDR     0x86
#define DS1302_MONTH_REG_ADDR    0x88
#define DS1302_YEAR_REG_ADDR     0x8C


#define DS1302_DAY_REG_ADDR      0x8A
#define DS1302_WP_REG_ADDR       0x8E

// 其他相关宏定义
#define DS1302_READ_MODE         0x01
#define DS1302_WRITE_MODE        0x00


#define Time_24_Hour    0x00    //24小时制
#define Time_Start     0x00    //开始走时
//DS1302寄存器操作指令定义时间地址
#define ds1302_sec_addr   0x80       //秒
#define ds1302_min_addr   0x82       //分
#define ds1302_hour_addr  0x84       //时
#define ds1302_day_addr   0x86       //日
#define ds1302_month_addr 0x88       //月
#define ds1302_year_addr  0x8c       //年



void DS1302_GPIO_Init(void);
void DS1302_WriteAddrOrData(uint8_t addrordata);
#if 0
void DS1302_WriteByte(uint8_t data);
uint8_t DS1302_ReadByte(void);
void DS1302_WriteReg(uint8_t regAddr, uint8_t data);
uint8_t DS1302_ReadReg(uint8_t regAddr);
void DS1302_SetTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, uint8_t year);
void DS1302_GetTime(uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *day, uint8_t *month, uint8_t *year);
#endif
//void DS1302_Write(uint8_t add,uint8_t dat);
uint8_t DS1302_Read(uint8_t add);
void DS1302_SetTime(uint8_t *ad);
void DS1302_OFF(void);
void DS1302_ON(void);


void DS1302_init(uint8_t *time);
void DS1302_Readtime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute,uint8_t *second );



void DS1302_TEST(void);
#endif /* __DS1302_H */


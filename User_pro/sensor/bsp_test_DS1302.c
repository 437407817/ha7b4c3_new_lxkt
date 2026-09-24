#include "./sensor/bsp_test_DS1302.h"

#include "./sys/bsp_systime.h"   
#include "./sys/sysio.h"

#include "./sensor/DS1302.h"

//#include "./sensor/DS1302.h"


 void settime(){

    // 假设这里有一个设置时间的函数调用
     uint8_t setTime[] = {0x23, 0x59, 0x59, 0x07, 0x01, 0x01, 0x23}; // 23:59:59, 07/01/2023
//     DS1302_SetTime(setTime);

}



void test_ds1302(void)
{
    static uint8_t i = 0;
    if(i == 0)
    {
        settime();
        i++;
    }

    uint8_t year, month, day, hour, minute, second;
    
    // 调用实际的读取函数获取当前时间
    DS1302_Readtime(&year, &month, &day, &hour, &minute, &second);

    // 打印解析后的时间
    SYSTEM_DEBUG("Time: %02d:%02d:%02d Date: %02d/%02d/%02d\n",
           hour, minute, second, 
           year + 2000, month, day); // 注意年份和年月日的对应顺序
}




static uint8_t second, minute, hour, day, month, year;


void DS1302_TEST_set(void)
{
    

    // 初始化DS1302相关GPIO引脚
    

    // 设置初始时间（示例时间，可按需调整）
//    DS1302_SetTime(0x00, 30, 12, 25, 12, 24);


        // 获取当前时间
//     DS1302_GetTime(&second, &minute, &hour, &day, &month, &year);

	
		uint8_t init_time[6]={35,59,23,31,12,23};//初始化时间参数
	DS1302_init(init_time);
	DS1302_SetTime(init_time);
		 SYSTEM_DEBUG("=====****WRITTE TIME");
}
#include "shell.h"

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), DS1302_TEST_SET, DS1302_TEST_set, test log);

void DS1302_TEST_get(void)
{



        // 获取当前时间
     DS1302_Readtime(&year, &month, &day, &hour, &minute, &second );
	        SYSTEM_DEBUG("Date: %02d/%02d/%02d Time: %02d:%02d:%02d \n",
               year, month, day,hour , minute ,second);
	

}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), DS1302_TEST_GET, DS1302_TEST_get, test log);






















/*********************************************END OF FILE**********************/

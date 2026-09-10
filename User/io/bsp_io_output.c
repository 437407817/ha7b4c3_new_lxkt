/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./io/bsp_io_output.h"   
#include "./sys/sysio.h"

typedef struct
{
    GPIO_TypeDef        *port;      // GPIO端口 GPIOA/GPIOB...
    uint16_t            pin;       // 引脚 GPIO_PIN_0
    void                (*clkEnFunc)(void); // 时钟使能宏包装函数
} CommonGPIOConfig_t;

/**
 * @brief 批量初始化LED推挽输出GPIO
 * @param pCfg LED配置数组
 * @param cfgCnt 数组元素个数
 */
void IO_CommonGpioOutputBatchConfig(const CommonGPIOConfig_t *pCfg, uint8_t cfgCnt)
{
    if(pCfg == NULL || cfgCnt == 0U)
    {
        return;
    }

    GPIO_InitTypeDef  GPIO_InitStruct;
    // 输出模式全局固定配置
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

    for(uint8_t i = 0; i < cfgCnt; i++)
    {
        const CommonGPIOConfig_t *pItem = &pCfg[i];
        if(pItem->port == NULL)
        {
            continue;
        }
        //开启时钟
        if(pItem->clkEnFunc != NULL)
        {
            pItem->clkEnFunc();
        }

        GPIO_InitStruct.Pin = pItem->pin;
        HAL_GPIO_Init(pItem->port, &GPIO_InitStruct);
    }
}



static void Led1_ClkEnable(void)
{
    LED1_GPIO_CLK_ENABLE();
}
static void Led2_ClkEnable(void)
{
    LED2_GPIO_CLK_ENABLE();
}
static void Led3_ClkEnable(void)
{
    LED3_GPIO_CLK_ENABLE();
}
static void Led4_ClkEnable(void)
{
    LED4_GPIO_CLK_ENABLE();
}





void IO_LED_GPIO_Config(void)
{
    const CommonGPIOConfig_t ledCfgTable[] =
    {
        {LED1_GPIO_PORT, LED1_PIN, Led1_ClkEnable},
        {LED2_GPIO_PORT, LED2_PIN, Led2_ClkEnable},
        {LED3_GPIO_PORT, LED3_PIN, Led3_ClkEnable},
        {LED4_GPIO_PORT, LED4_PIN, Led4_ClkEnable},
    };
    uint8_t cnt = sizeof(ledCfgTable)/sizeof(ledCfgTable[0]);

    IO_CommonGpioOutputBatchConfig(ledCfgTable, cnt);
		
		
		HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_SET);
}






static void Led01_BOARD_ClkEnable(void)
{
    LED01_BOARD_GPIO_CLK_ENABLE();
}
static void Led02_BOARD_ClkEnable(void)
{
    LED02_BOARD_GPIO_CLK_ENABLE();
}
static void Led03_BOARD_ClkEnable(void)
{
    LED03_BOARD_GPIO_CLK_ENABLE();
}
static void Led04_BOARD_ClkEnable(void)
{
    LED04_BOARD_GPIO_CLK_ENABLE();
}
static void Led05_BOARD_ClkEnable(void)
{
    LED05_BOARD_GPIO_CLK_ENABLE();
}

void IO_LED_BOARD_GPIO_Config(void)
{
    const CommonGPIOConfig_t ledBoardCfgTable[] =
    {
        {LED01_BOARD_GPIO_PORT, LED01_BOARD_PIN, Led01_BOARD_ClkEnable},
        {LED02_BOARD_GPIO_PORT, LED02_BOARD_PIN, Led02_BOARD_ClkEnable},
        {LED03_BOARD_GPIO_PORT, LED03_BOARD_PIN, Led03_BOARD_ClkEnable},
        {LED04_BOARD_GPIO_PORT, LED04_BOARD_PIN, Led04_BOARD_ClkEnable},
				{LED05_BOARD_GPIO_PORT, LED05_BOARD_PIN, Led05_BOARD_ClkEnable},
    };
    uint8_t cnt = sizeof(ledBoardCfgTable)/sizeof(ledBoardCfgTable[0]);

    IO_CommonGpioOutputBatchConfig(ledBoardCfgTable, cnt);
		
		
		HAL_GPIO_WritePin(LED01_BOARD_GPIO_PORT, LED01_BOARD_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED02_BOARD_GPIO_PORT, LED02_BOARD_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED03_BOARD_GPIO_PORT, LED03_BOARD_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED04_BOARD_GPIO_PORT, LED04_BOARD_PIN, GPIO_PIN_SET);	
		HAL_GPIO_WritePin(LED05_BOARD_GPIO_PORT, LED05_BOARD_PIN, GPIO_PIN_SET);			
}








static void FAN01_BOARD_ClkEnable(void)
{
    LED01_BOARD_GPIO_CLK_ENABLE();
}
static void FAN02_BOARD_ClkEnable(void)
{
    LED02_BOARD_GPIO_CLK_ENABLE();
}
static void FAN03_BOARD_ClkEnable(void)
{
    LED03_BOARD_GPIO_CLK_ENABLE();
}
static void FAN04_BOARD_ClkEnable(void)
{
    LED04_BOARD_GPIO_CLK_ENABLE();
}


void IO_FAN_GPIO_Config(void)
{
    const CommonGPIOConfig_t FANCfgTable[] =
    {
        {FAN01_GPIO_PORT, FAN01_PIN, FAN01_BOARD_ClkEnable},
        {FAN02_GPIO_PORT, FAN02_PIN, FAN02_BOARD_ClkEnable},
        {FAN03_GPIO_PORT, FAN03_PIN, FAN03_BOARD_ClkEnable},
        {FAN04_GPIO_PORT, FAN04_PIN, FAN04_BOARD_ClkEnable},
    };
    uint8_t cnt = sizeof(FANCfgTable)/sizeof(FANCfgTable[0]);

    IO_CommonGpioOutputBatchConfig(FANCfgTable, cnt);
		
		
		HAL_GPIO_WritePin(FAN01_GPIO_PORT, FAN01_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(FAN02_GPIO_PORT, FAN02_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(FAN03_GPIO_PORT, FAN03_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(FAN04_GPIO_PORT, FAN04_PIN, GPIO_PIN_SET);		
}




static void RE01_BOARD_ClkEnable(void)
{
    RE01_GPIO_CLK_ENABLE();
}
static void RE02_BOARD_ClkEnable(void)
{
    RE02_GPIO_CLK_ENABLE();
}
static void RE03_BOARD_ClkEnable(void)
{
    RE03_GPIO_CLK_ENABLE();
}
static void RE04_BOARD_ClkEnable(void)
{
    RE04_GPIO_CLK_ENABLE();
}


void IO_RE_GPIO_Config(void)
{
    const CommonGPIOConfig_t RECfgTable[] =
    {
        {RE01_GPIO_PORT, RE01_PIN, RE01_BOARD_ClkEnable},
        {RE02_GPIO_PORT, RE02_PIN, RE02_BOARD_ClkEnable},
        {RE03_GPIO_PORT, RE03_PIN, RE03_BOARD_ClkEnable},
        {RE04_GPIO_PORT, RE04_PIN, RE04_BOARD_ClkEnable},
    };
    uint8_t cnt = sizeof(RECfgTable)/sizeof(RECfgTable[0]);

    IO_CommonGpioOutputBatchConfig(RECfgTable, cnt);
		
		
		HAL_GPIO_WritePin(RE01_GPIO_PORT, RE01_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(RE02_GPIO_PORT, RE02_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(RE03_GPIO_PORT, RE03_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(RE04_GPIO_PORT, RE04_PIN, GPIO_PIN_SET);		
}



static void IO485_01_ClkEnable(void)
{
    IO485_01_GPIO_CLK_ENABLE();
}
static void IO485_02_ClkEnable(void)
{
    IO485_02_GPIO_CLK_ENABLE();
}
static void IO485_03_ClkEnable(void)
{
    IO485_03_GPIO_CLK_ENABLE();
}
static void IO485_04_ClkEnable(void)
{
    IO485_04_GPIO_CLK_ENABLE();
}


void IO485_GPIO_Config(void)
{
    const CommonGPIOConfig_t RECfgTable[] =
    {
        {IO485_01_GPIO_PORT, IO485_01_PIN, IO485_01_ClkEnable},
        {IO485_02_GPIO_PORT, IO485_02_PIN, IO485_02_ClkEnable},
        {IO485_03_GPIO_PORT, IO485_03_PIN, IO485_03_ClkEnable},
        {IO485_04_GPIO_PORT, IO485_04_PIN, IO485_04_ClkEnable},
    };
    uint8_t cnt = sizeof(RECfgTable)/sizeof(RECfgTable[0]);

    IO_CommonGpioOutputBatchConfig(RECfgTable, cnt);
		
		
		HAL_GPIO_WritePin(IO485_01_GPIO_PORT, RE01_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IO485_02_GPIO_PORT, RE02_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IO485_03_GPIO_PORT, RE03_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IO485_04_GPIO_PORT, RE04_PIN, GPIO_PIN_SET);		
}







uint16_t i=0;




//========测试函数：跑流水灯，放在while(1)调用========
void Test_Led_RunningWater(void)
{
	i++;
	if(i%3==0){
		HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
		SYSTEM_INFO("-A-- - %d",i);
	}else if(i%3==1){
		HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);	
		SYSTEM_INFO("-B-- - %d",i);
	}else if(i%3==2){
		HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);
		SYSTEM_INFO("-C-- - %d",i);
	}
	

		
	
	
    // LED2亮
//    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
//    HAL_Delay(700);
//    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);

//    // LED3亮
//    HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET);
//    HAL_Delay(700);
//    HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_SET);

//    // LED4亮
//    HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_RESET);
//    HAL_Delay(700);
//    HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_SET);
}




// /**
//  * @brief  初始化控制LED的IO
//  * @param  无
//  * @retval 无
//  */
//void IO_LED_GPIO_Config(void)
//{		
//		
//    /*定义一个GPIO_InitTypeDef类型的结构体*/
//    GPIO_InitTypeDef  GPIO_InitStruct;

//    /*开启LED相关的GPIO外设时钟*/
//    LED1_GPIO_CLK_ENABLE();
//    LED2_GPIO_CLK_ENABLE();
//    LED3_GPIO_CLK_ENABLE();
//    LED4_GPIO_CLK_ENABLE();

//    /*选择要控制的GPIO引脚*/															   
//    GPIO_InitStruct.Pin = LED1_PIN;	

//    /*设置引脚的输出类型为推挽输出*/
//    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

//    /*设置引脚为上拉模式*/
//    GPIO_InitStruct.Pull  = GPIO_PULLUP;

//    /*设置引脚速率为高速 */   
//    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; 

//    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
//    HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);	

//    /*选择要控制的GPIO引脚*/															   
//    GPIO_InitStruct.Pin = LED2_PIN;	
//    HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);	

//    /*选择要控制的GPIO引脚*/															   
//    GPIO_InitStruct.Pin = LED3_PIN;	
//    HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);	

//    /*选择要控制的GPIO引脚*/															   
//    GPIO_InitStruct.Pin = LED4_PIN;	
//    HAL_GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStruct);	

//    /*关闭RGB灯*/
//    LED_RGBOFF;

//    /*指示灯默认开启*/
//    LED4(ON);
//		
//}










void OUT_Io_GPIO_Config(void){

IO_LED_GPIO_Config();
	
	IO_LED_BOARD_GPIO_Config();
	
	IO_FAN_GPIO_Config();
		IO_RE_GPIO_Config();
	IO485_GPIO_Config();
//ChargerParallel_GPIO_Config();
//RelayXXA_GPIO_Config();
//RelayCHX_GPIO_Config();
//RelayGate_GPIO_Config();

}








/*********************************************END OF FILE**********************/

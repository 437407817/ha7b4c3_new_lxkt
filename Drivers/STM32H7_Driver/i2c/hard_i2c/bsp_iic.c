

#include "./i2c/hard_i2c/bsp_iic.h"
//#include "./usart/bsp_usart.h"






/* USER CODE BEGIN 0 */

/* USER CODE END 0 */







I2C_HandleTypeDef  I2C_A_Handle; 

/**
  * @brief  I2C1 I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
	  
	/*使能I2C时钟*/
	EEPROM_I2C_CLK_ENABLE();

	/*使能I2C的IO口时钟*/
	EEPROM_I2C_SCL_GPIO_CLK_ENABLE();
	EEPROM_I2C_SDA_GPIO_CLK_ENABLE(); 

	/*配置I2C的SCL口*/   
	GPIO_InitStructure.Pin = EEPROM_I2C_SCL_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = EEPROM_I2C_SCL_AF;
	HAL_GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	/*配置I2C的SDA口*/  
	GPIO_InitStructure.Pin = EEPROM_I2C_SDA_PIN;
	HAL_GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

	/* Force the I2C peripheral clock reset */  
	EEPROM_I2C_FORCE_RESET(); 

	/* Release the I2C peripheral clock reset */  
	EEPROM_I2C_RELEASE_RESET(); 

}

/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Config(void)
{
    /* I2C 配置 */
    I2C_A_Handle.Instance = EEPROM_I2C;
    I2C_A_Handle.Init.Timing           = 0x40604E73;//100KHz
    I2C_A_Handle.Init.OwnAddress1      = 0;
    I2C_A_Handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    I2C_A_Handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    I2C_A_Handle.Init.OwnAddress2      = 0;
    I2C_A_Handle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    I2C_A_Handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    I2C_A_Handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    /* Init the I2C */
    HAL_I2C_Init(&I2C_A_Handle);	

    HAL_I2CEx_AnalogFilter_Config(&I2C_A_Handle, I2C_ANALOGFILTER_ENABLE);    
}

/**
  * @brief  I2C 外设(EEPROM)初始化
  * @param  无
  * @retval 无
  */
void I2C_EE_Init(void)
{
   
	I2C_GPIO_Config();
	I2C_Mode_Config();
	
}





















 /**
  ******************************************************************************
  * @file    bsp_spi_flash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   spi flash 底层应用函数bsp 
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火STM32 F429 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  #if 1
#include "./spi/bsp_spi.h"

SPI_HandleTypeDef spi_a;




/* SPI1 init function */
void MX_SPI_A_Init(void)
{

spi_a.Instance = SPI_a;
  spi_a.Init.Mode = SPI_MODE_MASTER;
  spi_a.Init.Direction = SPI_DIRECTION_2LINES;
  spi_a.Init.DataSize = SPI_DATASIZE_8BIT;
  
  /* SPI 时序模式配置：Mode 0 (CPOL=0, CPHA=1Edge) */
  spi_a.Init.CLKPolarity = SPI_POLARITY_LOW;
  spi_a.Init.CLKPhase = SPI_PHASE_1EDGE;
  
  spi_a.Init.NSS = SPI_NSS_SOFT;
  spi_a.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; // 降频至约 2.8MHz 确保信号稳定
  spi_a.Init.FirstBit = SPI_FIRSTBIT_MSB;
  spi_a.Init.TIMode = SPI_TIMODE_DISABLE;
  spi_a.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  spi_a.Init.CRCPolynomial = 0x0;
  
  /* 关键修改：禁用硬件 NSSPulse，防止软件 CS 被干扰 */
  spi_a.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  spi_a.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  spi_a.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  spi_a.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  spi_a.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  spi_a.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  spi_a.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  spi_a.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  spi_a.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  spi_a.Init.IOSwap = SPI_IO_SWAP_DISABLE;

  if (HAL_SPI_Init(&spi_a) != HAL_OK)
  {
      /* 可以在此添加错误处理 */
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}




/**
  * @brief SPI MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(spiHandle->Instance == SPI_a)
  {
    /* 1. 使能外设与 GPIO 时钟 */
    SPI_a_SCK_GPIO_CLK_ENABLE();
    SPI_a_MISO_GPIO_CLK_ENABLE();
    SPI_a_MOSI_GPIO_CLK_ENABLE();
    SPI_a_CS_GPIO_CLK_ENABLE();
    SPI_a_CLK_ENABLE();

    /* 2. SCK 配置 */
    GPIO_InitStruct.Pin       = SPI_a_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = SPI_a_SCK_AF;
    HAL_GPIO_Init(SPI_a_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* 3. MISO 配置（补全 Mode 和 Pull，防止数据位移位） */
    GPIO_InitStruct.Pin       = SPI_a_MISO_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = SPI_a_MISO_AF;
    HAL_GPIO_Init(SPI_a_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* 4. MOSI 配置 */
    GPIO_InitStruct.Pin       = SPI_a_MOSI_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = SPI_a_MOSI_AF;
    HAL_GPIO_Init(SPI_a_MOSI_GPIO_PORT, &GPIO_InitStruct);

    /* 5. CS 引脚配置（软件推挽控制） */
    HAL_GPIO_WritePin(FLASH_CS_GPIO_PORT, FLASH_CS_PIN, GPIO_PIN_SET); // 默认高电平（不选通）
    GPIO_InitStruct.Pin       = FLASH_CS_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStruct);
  }
}



/**
 * @brief   SPI发送并接收一个字节（全双工标准实现）
 */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
    uint8_t rx_data = 0;
    HAL_SPI_TransmitReceive(&spi_a, &byte, &rx_data, 1, 100);
    return rx_data;
}

/**
 * @brief   读取一个字节 (发送 Dummy 0xFF 以产生 CLK 时钟)
 */
uint8_t SPI_FLASH_ReadByte(void)
{
    return SPI_FLASH_SendByte(0xFF);
}

///**
// * @brief   发送一个字节
// * @param   无
// * @retval  无
// */
//void SPI_FLASH_SendByte(uint8_t byte)
//{
//    HAL_SPI_Transmit(&spi_a, &byte, 1, 500);
//}
 
/**
 * @brief   发送n个字节
 * @param   pData：发送数据首地址
 * @param   data_number：发送数据个数（以字节为单位）
 * @retval  无
 */
void SPI_FLASH_SendnByte(uint8_t *pData, uint32_t data_number)
{
    HAL_SPI_Transmit(&spi_a, pData, data_number, 500);
}
 

/**
 * @brief   读取一个字节
 * @param   无
 * @retval  接收的数据
 */
//uint8_t SPI_FLASH_ReadByte(void)
//{
//    uint8_t rxData = 0;
//    HAL_SPI_Receive(&spi_a, &rxData, 1, 500);
//    return rxData;
//}

/**
 * @brief   接收n个字节
 * @param   pData：接收数据首地址
 * @param   data_number：接收数据个数（以字节为单位）
 * @retval  无
 */
void SPI_FLASH_ReadnByte(uint8_t *pData, uint32_t data_number)
{
    HAL_SPI_Receive(&spi_a, pData, data_number, 500);
}
 


 


#endif





   
/*********************************************END OF FILE**********************/

 /**
  ******************************************************************************
  * @file    bsp_qspi_flash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   qspi flash 底层应用函数bsp 
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火STM32 H743 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./flash/bsp_qspi_flash.h"


QSPI_HandleTypeDef QSPIHandle;

/**
  * @brief  底层的 GPIO 与 Clock 初始化回调函数
  */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 使能 QSPI 及 GPIO 时钟 */
    QSPI_FLASH_CLK_ENABLE();
    QSPI_CS_GPIO_CLK_ENABLE();     /* GPIOG */
    QSPI_CLK_GPIO_CLK_ENABLE();    /* GPIOF */

    /* 公共配置：复用推挽，最高速度 */
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

    /* CS Pin (PG6) -> AF10 */
    GPIO_InitStruct.Pin       = QSPI_CS_PIN;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = QSPI_CS_GPIO_AF; 
    HAL_GPIO_Init(QSPI_CS_GPIO_PORT, &GPIO_InitStruct);

    /* CLK Pin (PF10) -> AF9 */
    GPIO_InitStruct.Pin       = QSPI_CLK_PIN;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = QSPI_CLK_GPIO_AF; 
    HAL_GPIO_Init(QSPI_CLK_GPIO_PORT, &GPIO_InitStruct);

    /* D0 Pin (PF8) -> AF9 */
    GPIO_InitStruct.Pin       = QSPI_BK1_D0_PIN;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = QSPI_BK1_D0_GPIO_AF;
    HAL_GPIO_Init(QSPI_BK1_D0_GPIO_PORT, &GPIO_InitStruct);

    /* D1 Pin (PF9) -> AF9 */
    GPIO_InitStruct.Pin       = QSPI_BK1_D1_PIN;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = QSPI_BK1_D1_GPIO_AF;
    HAL_GPIO_Init(QSPI_BK1_D1_GPIO_PORT, &GPIO_InitStruct);

    /* D2 Pin (PF7) -> AF9 */
    GPIO_InitStruct.Pin       = QSPI_BK1_D2_PIN;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = QSPI_BK1_D2_GPIO_AF;
    HAL_GPIO_Init(QSPI_BK1_D2_GPIO_PORT, &GPIO_InitStruct);

    /* D3 Pin (PF6) -> AF9 */
    GPIO_InitStruct.Pin       = QSPI_BK1_D3_PIN;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = QSPI_BK1_D3_GPIO_AF;
    HAL_GPIO_Init(QSPI_BK1_D3_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  QSPI 初始化函数
  */
uint8_t BSP_QSPI_Init(void)
{
    QSPIHandle.Instance = QSPI_FLASH;

    /* 复位 QSPI 外设 */
    HAL_QSPI_DeInit(&QSPIHandle);

    /* 配置 QSPI 参数 */
    QSPIHandle.Init.ClockPrescaler     = 10;                              /* 降频测试，保证稳定性 */
    QSPIHandle.Init.FifoThreshold      = 1;
    QSPIHandle.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_HALFCYCLE; /* 关键：半周期延时采样 */
    QSPIHandle.Init.FlashSize          = QSPI_FLASH_SIZE;
    QSPIHandle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE;
    QSPIHandle.Init.ClockMode          = QSPI_CLOCK_MODE_3;               /* 关键：模式 3 提升兼容性 */
    QSPIHandle.Init.FlashID            = QSPI_FLASH_ID_1;
    QSPIHandle.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;

    if (HAL_QSPI_Init(&QSPIHandle) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    /* 上电复位 W25N01G 状态机 */
    QSPI_FLASH_Reset();

    /* 解除芯片上电写保护 (将 Protection Register 写入 0x00) */
    QSPI_FLASH_WriteStatusReg(STATUS_REG_PROTECTION, 0x00);

    return QSPI_OK;
}

/**
  * @brief  发送 0xFF 复位 Flash
  */
uint8_t QSPI_FLASH_Reset(void)
{
    QSPI_CommandTypeDef s_command = {0};

    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = RESET_CMD; // 0xFF
    s_command.AddressMode       = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_NONE;
    s_command.DummyCycles       = 0;

    if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_MAX_DELAY) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    HAL_Delay(5); // 等待 reset 结束 (tRST)
    return QSPI_OK;
}

/**
  * @brief  读取 W25N01GV 的 JEDEC ID (预期输出: 0xEF 0xAA 0x21)
  */
uint8_t QSPI_FLASH_ReadID(uint8_t *id_buf)
{
    QSPI_CommandTypeDef s_command = {0};

    /* 1. 先复位芯片状态机 */
    QSPI_FLASH_Reset();

    /* 2. 发送 Read ID 命令 (0x9F + 1 字节 dummy 0x00) */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = READ_ID_CMD;          /* 0x9F */
    
    s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize       = QSPI_ADDRESS_8_BITS;
    s_command.Address           = 0x00;                 /* 1 个哑字节地址 */
    
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_1_LINE;
    s_command.DummyCycles       = 0;                    
    s_command.NbData            = 3;

    if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_MAX_DELAY) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&QSPIHandle, id_buf, HAL_MAX_DELAY) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

/**
  * @brief  读取状态寄存器
  */
uint8_t QSPI_FLASH_ReadStatusReg(uint8_t reg_addr, uint8_t *regvalue)
{
    QSPI_CommandTypeDef s_command = {0};

    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = READ_STATUS_REG_CMD;  /* 0x0F */
    s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize       = QSPI_ADDRESS_8_BITS;
    s_command.Address           = reg_addr;             /* 0xA0 / 0xB0 / 0xC0 */
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_1_LINE;
    s_command.DummyCycles       = 0;
    s_command.NbData            = 1;

    if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_MAX_DELAY) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&QSPIHandle, regvalue, HAL_MAX_DELAY) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

/**
  * @brief  写入状态寄存器
  */
uint8_t QSPI_FLASH_WriteStatusReg(uint8_t reg_addr, uint8_t regvalue)
{
    QSPI_CommandTypeDef s_command = {0};

    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = WRITE_STATUS_REG_CMD; /* 0x1F */
    s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize       = QSPI_ADDRESS_8_BITS;
    s_command.Address           = reg_addr;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_1_LINE;
    s_command.DummyCycles       = 0;
    s_command.NbData            = 1;

    if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_MAX_DELAY) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Transmit(&QSPIHandle, &regvalue, HAL_MAX_DELAY) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}





/*********************************************END OF FILE**********************/

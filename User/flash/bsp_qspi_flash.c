#include "./flash/bsp_qspi_flash.h"
#include "./sys/sysio.h"
#include <string.h>

QSPI_HandleTypeDef QSPIHandle;
QspiFlashDev_t g_qspiFlashDev={0};

#define W25N_TMP_BUF_SZ     (2048U + 16U)
static uint8_t g_w25n_tx_tmp[W25N_TMP_BUF_SZ];
static uint8_t g_w25n_rx_tmp[W25N_TMP_BUF_SZ];

/**
 * @brief QSPI底层GPIO+外设初始化 H743 for W25N01G
 */
void QSPI_FLASH_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	QSPI_FLASH_CLK_ENABLE();
	QSPI_FLASH_CLK_GPIO_ENABLE();
	QSPI_FLASH_BK1_IO0_CLK_ENABLE();
	QSPI_FLASH_BK1_IO1_CLK_ENABLE();
	QSPI_FLASH_BK1_IO2_CLK_ENABLE();
	QSPI_FLASH_BK1_IO3_CLK_ENABLE();
	QSPI_FLASH_CS_GPIO_CLK_ENABLE();

	/* CLK */
	GPIO_InitStruct.Pin = QSPI_FLASH_CLK_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = QSPI_FLASH_CLK_GPIO_AF;
	HAL_GPIO_Init(QSPI_FLASH_CLK_GPIO_PORT, &GPIO_InitStruct);

	/* IO0 */
	GPIO_InitStruct.Pin = QSPI_FLASH_BK1_IO0_PIN;
	GPIO_InitStruct.Alternate = QSPI_FLASH_BK1_IO0_AF;
	HAL_GPIO_Init(QSPI_FLASH_BK1_IO0_PORT, &GPIO_InitStruct);

	/* IO1 */
	GPIO_InitStruct.Pin = QSPI_FLASH_BK1_IO1_PIN;
	GPIO_InitStruct.Alternate = QSPI_FLASH_BK1_IO1_AF;
	HAL_GPIO_Init(QSPI_FLASH_BK1_IO1_PORT, &GPIO_InitStruct);

	/* IO2 */
	GPIO_InitStruct.Pin = QSPI_FLASH_BK1_IO2_PIN;
	GPIO_InitStruct.Alternate = QSPI_FLASH_BK1_IO2_AF;
	HAL_GPIO_Init(QSPI_FLASH_BK1_IO2_PORT, &GPIO_InitStruct);

	/* IO3 */
	GPIO_InitStruct.Pin = QSPI_FLASH_BK1_IO3_PIN;
	GPIO_InitStruct.Alternate = QSPI_FLASH_BK1_IO3_AF;
	HAL_GPIO_Init(QSPI_FLASH_BK1_IO3_PORT, &GPIO_InitStruct);

	/* CS */
	GPIO_InitStruct.Pin = QSPI_FLASH_CS_PIN;
	GPIO_InitStruct.Alternate = QSPI_FLASH_CS_GPIO_AF;
	HAL_GPIO_Init(QSPI_FLASH_CS_GPIO_PORT, &GPIO_InitStruct);

	HAL_QSPI_DeInit(&QSPIHandle);

	QSPIHandle.Instance = QUADSPI;
	/* W25N01G NAND：Prescaler=16，240M/(16+1)=14.1M 或根据实际调整 */
	QSPIHandle.Init.ClockPrescaler = 16U;
	QSPIHandle.Init.FifoThreshold = 16U;
	QSPIHandle.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
	QSPIHandle.Init.FlashSize = 27U;  /* 128MByte 2^27 */
	QSPIHandle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_8_CYCLE;
	QSPIHandle.Init.ClockMode = QSPI_CLOCK_MODE_0;
	QSPIHandle.Init.FlashID = QSPI_FLASH_ID_1;
	QSPIHandle.Init.DualFlash = QSPI_DUALFLASH_DISABLE;

	if(HAL_QSPI_Init(&QSPIHandle) != HAL_OK)
	{
//		Error_Handler();
	}

	/* W25N01G 初始化序列：复位 →读ID →解锁写保护 */
	QSPI_W25N01G_Reset();
	uint32_t id = QSPI_W25N01G_ReadJedecID();
	if(id == W25N01G_JEDEC_ID)
	{
		g_qspiFlashDev.devType = FLASH_DEV_W25N01G;
		g_qspiFlashDev.jedecId = id;
		g_qspiFlashDev.pageSize = W25N01G_PAGE_SIZE;
		g_qspiFlashDev.blockSize = W25N01G_BLOCK_SIZE;
		g_qspiFlashDev.totalSize = W25N01G_TOTAL_SIZE;
		QSPI_W25N01G_UnlockProtect();
	}
}

/**
 * @brief W25N01G软件复位
 */
uint8_t QSPI_W25N01G_Reset(void)
{
	QSPI_CommandTypeDef cmd={0};
	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction = W25N01G_RESET_CMD;
	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.DataMode = QSPI_DATA_NONE;
	cmd.DummyCycles = 0;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	if(HAL_QSPI_Command(&QSPIHandle,&cmd,100)!=HAL_OK)
		return 1;
	HAL_Delay(5); /*tRST*/
	return 0;
}

/**
 * @brief W25N01G 读JEDEC ID 0x9F + 1Dummy字节，3字节ID
 */
uint32_t QSPI_W25N01G_ReadJedecID(void)
{
	uint8_t buf[3]={0};
	QSPI_CommandTypeDef cmd={0};
	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction = W25N01G_JEDEC_ID_CMD;
	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.DataMode = QSPI_DATA_1_LINE;
	cmd.DummyCycles = 8U;   // 8个SCLK(1字节dummy)
	cmd.NbData = 3U;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	HAL_StatusTypeDef ret_cmd = HAL_QSPI_Command(&QSPIHandle,&cmd,100);
	SYSTEM_DEBUG("HAL_QSPI_Command ret:%d\r\n",ret_cmd);
	if(ret_cmd != HAL_OK)
		return 0U;

	HAL_StatusTypeDef ret_rx = HAL_QSPI_Receive(&QSPIHandle,buf,100);
	SYSTEM_DEBUG("HAL_QSPI_Receive ret:%d\r\n",ret_rx);
	if(ret_rx != HAL_OK)
		return 0U;

	SYSTEM_DEBUG("RAW buf[0]=0x%02X buf[1]=0x%02X buf[2]=0x%02X\r\n",buf[0],buf[1],buf[2]);
	return ((uint32_t)buf[0]<<16)|((uint32_t)buf[1]<<8)|buf[2];
}

/**
 * @brief W25N01G写使能
 */
uint8_t QSPI_W25N01G_WriteEnable(void)
{
	QSPI_CommandTypeDef cmd={0};
	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction = W25N01G_WRITE_ENABLE;
	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.DataMode = QSPI_DATA_NONE;
	cmd.DummyCycles = 0;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	if(HAL_QSPI_Command(&QSPIHandle,&cmd,100)!=HAL_OK)
		return 1;
	return 0;
}

/**
 * @brief 查询状态寄存器等待BUSY位清除
 */
uint8_t QSPI_W25N01G_WaitBusy(uint32_t timeout)
{
	uint32_t tickstart = HAL_GetTick();
	QSPI_CommandTypeDef cmd={0};
	uint8_t sta;
	while(1)
	{
		if((HAL_GetTick()-tickstart)>=timeout)
			return 1;
		cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
		cmd.Instruction = W25N01G_GET_FEATURE;
		cmd.AddressMode = QSPI_ADDRESS_1_LINE;
		cmd.Address = W25N01G_STATUS_REG_ADDR;
		cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
		cmd.DataMode = QSPI_DATA_1_LINE;
		cmd.NbData = 1;
		cmd.DummyCycles = 0;
		cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
		if(HAL_QSPI_Command(&QSPIHandle,&cmd,100)!=HAL_OK)
			continue;
		if(HAL_QSPI_Receive(&QSPIHandle,&sta,100)!=HAL_OK)
			continue;
		if((sta & W25N01G_BUSY_MASK)==0)
			break;
	}
	return 0;
}

/**
 * @brief W25N01G 上电解锁保护寄存器
 */
/**
 * @brief W25N01G 上电解锁保护寄存器，并回读校验
 */
uint8_t QSPI_W25N01G_UnlockProtect(void)
{
	QSPI_CommandTypeDef cmd={0};
	uint8_t val = 0;

	// 1. 写使能
	if(QSPI_W25N01G_WriteEnable())
		return 1;

	// 2. 写入保护寄存器 0xA0 = 0x00 (解除所有区块保护)
	cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction       = W25N01G_SET_FEATURE;
	cmd.AddressMode       = QSPI_ADDRESS_1_LINE;
	cmd.AddressSize       = QSPI_ADDRESS_8_BITS;  // 寄存器地址通常为 8 位
	cmd.Address           = W25N01G_PROTECTION_REG_ADDR; // 0xA0
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.DataMode          = QSPI_DATA_1_LINE;
	cmd.NbData            = 1;
	cmd.DummyCycles       = 0;
	cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	
	if(HAL_QSPI_Command(&QSPIHandle, &cmd, 100) != HAL_OK)
		return 1;
		
	val = 0x00;
	if(HAL_QSPI_Transmit(&QSPIHandle, &val, 100) != HAL_OK)
		return 1;
		
	if(QSPI_W25N01G_WaitBusy(50))
		return 1;

	// 3. 【关键】回读 0xA0 寄存器，确保真的变成 0x00 了！
	cmd.Instruction       = W25N01G_GET_FEATURE;
	cmd.Address           = W25N01G_PROTECTION_REG_ADDR;
	cmd.NbData            = 1;
	
	if(HAL_QSPI_Command(&QSPIHandle, &cmd, 100) != HAL_OK)
		return 1;
		
	if(HAL_QSPI_Receive(&QSPIHandle, &val, 100) != HAL_OK)
		return 1;
		
	SYSTEM_DEBUG("W25N01G Protection Reg (0xA0) = 0x%02X\r\n", val);
	
	return (val == 0x00) ? 0 : 1; // 如果不为0说明解锁失败
}

/**
 * @brief 块擦除 W25N01G blockNum:块编号
 */
uint8_t QSPI_W25N01G_BlockErase(uint32_t blockNum)
{
	if(QSPI_W25N01G_WriteEnable())
		return 1;
	QSPI_CommandTypeDef cmd={0};
	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction = W25N01G_BLOCK_ERASE;
	cmd.AddressMode = QSPI_ADDRESS_1_LINE; // 修改为 1-line 模式
	cmd.Address = blockNum; 
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.DataMode = QSPI_DATA_NONE;
	cmd.DummyCycles = 0;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	if(HAL_QSPI_Command(&QSPIHandle,&cmd,100)!=HAL_OK)
		return 1;
	return QSPI_W25N01G_WaitBusy(120);
}

/**
 * @brief 页读：NAND页内容复制到内部SRAM缓存
 * @param pageNum NAND页号
 */
uint8_t QSPI_W25N01G_PageReadToCache(uint32_t pageNum)
{
	QSPI_CommandTypeDef cmd={0};
	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction = W25N01G_PAGE_DATA_READ;
	cmd.AddressMode = QSPI_ADDRESS_1_LINE; // 修改为 1-line 模式
	cmd.Address = pageNum;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.DataMode = QSPI_DATA_NONE;
	cmd.DummyCycles = 0;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	if(HAL_QSPI_Command(&QSPIHandle,&cmd,100)!=HAL_OK)
		return 1;
	return QSPI_W25N01G_WaitBusy(20);
}

/**
 * @brief 从NAND内部缓存读取数据(1-line SPI)
 * @param colAddr 列地址0-2047
 * @param len 读取长度
 */
/**
 * @brief 从NAND内部缓存读取数据(1-line SPI)
 * @param colAddr 列地址0-2047
 * @param len 读取长度
 */
uint8_t QSPI_W25N01G_ReadFromCache(uint8_t *pBuf, uint16_t colAddr, uint16_t len)
{
	QSPI_CommandTypeDef cmd={0};
	cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction       = W25N01G_READ_FROM_BUFFER; // 0x03
	cmd.AddressMode       = QSPI_ADDRESS_1_LINE;      
	cmd.AddressSize       = QSPI_ADDRESS_16_BITS;     
	cmd.Address           = colAddr;                  
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.DataMode          = QSPI_DATA_1_LINE;
	cmd.NbData            = len;                      
	cmd.DummyCycles       = 8;                        // 【关键修正】0x03指令需要 1 个 Dummy 字节 (8个时钟)
	cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if(HAL_QSPI_Command(&QSPIHandle, &cmd, 100U) != HAL_OK)
	{
		return 1;
	}
	
	if(HAL_QSPI_Receive(&QSPIHandle, pBuf, 100U) != HAL_OK)
	{
		return 1;
	}
	
	return 0;
}

/**
 * @brief 加载数据到NAND内部缓存，准备编程
 * @param colAddr 列地址0-2047
 */
/**
 * @brief 加载数据到NAND内部缓存，准备编程
 * @param colAddr 列地址0-2047
 */
uint8_t QSPI_W25N01G_LoadProgramData(uint8_t *pBuf, uint16_t colAddr, uint16_t len)
{
	if(QSPI_W25N01G_WriteEnable())
		return 1;

	QSPI_CommandTypeDef cmd={0};
	cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction       = W25N01G_LOAD_PROG_DATA; // 0x02
	cmd.AddressMode       = QSPI_ADDRESS_1_LINE;    
	cmd.AddressSize       = QSPI_ADDRESS_16_BITS;   
	cmd.Address           = colAddr;                // 硬件自动发列地址
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.DataMode          = QSPI_DATA_1_LINE;
	cmd.NbData            = len;                    // 纯数据长度
	cmd.DummyCycles       = 0;
	cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if(HAL_QSPI_Command(&QSPIHandle, &cmd, 100U) != HAL_OK)
		return 1;
		
	if(HAL_QSPI_Transmit(&QSPIHandle, pBuf, 100U) != HAL_OK)
		return 1;
		
	return 0;
}

/**
 * @brief 执行编程：把缓存数据烧写到NAND Flash页
 * @param pageNum 目标页号
 */
uint8_t QSPI_W25N01G_ProgramExecute(uint32_t pageNum)
{
	QSPI_CommandTypeDef cmd={0};
	cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction       = W25N01G_PROGRAM_EXECUTE; // 0x10
	cmd.AddressMode       = QSPI_ADDRESS_1_LINE;
	cmd.AddressSize       = QSPI_ADDRESS_16_BITS;    // 明确指定 16 位行地址
	cmd.Address           = pageNum;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.DataMode          = QSPI_DATA_NONE;
	cmd.DummyCycles       = 0;
	cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	if(HAL_QSPI_Command(&QSPIHandle,&cmd,100)!=HAL_OK)
		return 1;
	return QSPI_W25N01G_WaitBusy(30);
}

/**
 * @brief  W25N01G简单读，输入字节偏移
 * @param  offset:字节偏移
 * @param  buf:输出缓存
 * @param  len:读取字节数
 * @retval 0成功
 */
uint8_t W25N_Read(uint32_t offset, uint8_t *buf, uint32_t len)
{
    uint32_t page = offset / W25N01G_PAGE_SIZE;
    uint16_t col  = offset % W25N01G_PAGE_SIZE;

    while(len>0)
    {
        uint16_t readLen = len;
        if( (col + readLen) > W25N01G_PAGE_SIZE )
            readLen = W25N01G_PAGE_SIZE - col;

        if(QSPI_W25N01G_PageReadToCache(page))
            return 1;
        if(QSPI_W25N01G_ReadFromCache(buf, col, readLen))
            return 1;

        buf += readLen;
        offset += readLen;
        len -= readLen;
        page = offset / W25N01G_PAGE_SIZE;
        col  = offset % W25N01G_PAGE_SIZE;
    }
    return 0;
}
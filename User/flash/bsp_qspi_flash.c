#include "./flash/bsp_qspi_flash.h"
#include "./sys/sysio.h"
#include <string.h>

QSPI_HandleTypeDef QSPIHandle;
QspiFlashDev_t g_qspiFlashDev={0};

//#define W25N_TMP_BUF_SZ     (2048U + 16U)
//static uint8_t g_w25n_tx_tmp[W25N_TMP_BUF_SZ];
//static uint8_t g_w25n_rx_tmp[W25N_TMP_BUF_SZ];

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
//	SYSTEM_DEBUG("HAL_QSPI_Command ret:%d\r\n",ret_cmd);
	if(ret_cmd != HAL_OK)
		return 0U;

	HAL_StatusTypeDef ret_rx = HAL_QSPI_Receive(&QSPIHandle,buf,100);
//	SYSTEM_DEBUG("HAL_QSPI_Receive ret:%d\r\n",ret_rx);
	if(ret_rx != HAL_OK)
		return 0U;

//	SYSTEM_DEBUG("RAW buf[0]=0x%02X buf[1]=0x%02X buf[2]=0x%02X\r\n",buf[0],buf[1],buf[2]);
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
		
//	SYSTEM_DEBUG("W25N01G Protection Reg (0xA0) = 0x%02X\r\n", val);
	
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













//*************************

/**
 * @brief  根据字节偏移换算：页号、页内列地址、块号
 * @param  offset: 全局字节偏移
 * @param  out_page:输出物理页号
 * @param  out_col:输出页内列地址(0‑2047)
 * @param  out_block:输出块编号
 */
 void W25N_GetAddrInfo(uint32_t offset, uint32_t *out_page, uint16_t *out_col, uint32_t *out_block)
{
    *out_page = offset / W25N01G_PAGE_SIZE;
    *out_col  = offset % W25N01G_PAGE_SIZE;
    *out_block = *out_page / (W25N01G_BLOCK_SIZE / W25N01G_PAGE_SIZE);
}

/**
 * @brief  W25N01G 读数据，支持跨页
 * @param  offset: 全局字节偏移(0 ~ W25N01G_TOTAL_SIZE‑1)
 * @param  buf: 接收数据缓冲区
 * @param  len: 读取字节长度
 * @retval 0成功；非0错误码
 */
uint8_t W25N_Read(uint32_t offset, uint8_t *buf, uint32_t len)
{
    if(buf == NULL || len == 0U)
        return 1;
    if((offset + len) > W25N01G_TOTAL_SIZE)
        return 2; /*地址越界 */

    uint32_t remain = len;
    uint8_t *pDst = buf;

    while(remain > 0U)
    {
        uint32_t page;
        uint16_t col;
        uint32_t block;
        W25N_GetAddrInfo(offset, &page, &col, &block);

        uint16_t chunk = remain;
        if((col + chunk) > W25N01G_PAGE_SIZE)
        {
            chunk = W25N01G_PAGE_SIZE - col;
        }

        /* NAND物理页拷贝进入芯片SRAM缓存 */
        if(QSPI_W25N01G_PageReadToCache(page) != 0U)
        {
            return 3;
        }
        /* 从SRAM缓存读取数据 */
        if(QSPI_W25N01G_ReadFromCache(pDst, col, chunk) != 0U)
        {
            return 4;
        }

        pDst += chunk;
        offset += chunk;
        remain -= chunk;
    }
    return 0U;
}

/**
 * @brief 加载数据到NAND内部缓存【不内部做写使能！外部要自己调用WriteEnable】
 * @param colAddr 列地址0‑2047
 */
uint8_t QSPI_W25N01G_LoadProgramData_NoWE(uint8_t *pBuf,uint16_t colAddr,uint16_t len)
{
	QSPI_CommandTypeDef cmd={0};
	cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction       = W25N01G_LOAD_PROG_DATA; // 0x02
	cmd.AddressMode       = QSPI_ADDRESS_1_LINE;
	cmd.AddressSize       = QSPI_ADDRESS_16_BITS;
	cmd.Address           = colAddr;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.DataMode          = QSPI_DATA_1_LINE;
	cmd.NbData            = len;
	cmd.DummyCycles       = 0;
	cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	if(HAL_QSPI_Command(&QSPIHandle,&cmd,100U)!=HAL_OK)
		return 1;
	if(HAL_QSPI_Transmit(&QSPIHandle,pBuf,100U)!=HAL_OK)
		return 1;
	return 0;
}


static uint8_t W25N_PageWriteInner(uint32_t page, uint16_t col, uint8_t *pSrc, uint16_t len)
{
    if((col + len) > W25N01G_PAGE_SIZE)
        return 1;

    // 如果不是整页写入（即局部写入），我们需要保证 2048 字节缓存的其他部分不被污染
    if(len < W25N01G_PAGE_SIZE)
    {
        // 1. 尝试将当前物理页读入缓存（如果该页之前写过数据，能带出老数据）
        uint8_t read_res = QSPI_W25N01G_PageReadToCache(page);
        
        uint8_t temp_page_buf[W25N01G_PAGE_SIZE];
        
        if(read_res == 0U)
        {
            // 如果读取成功，把当前缓存完整读到 MCU 内存中
            if(QSPI_W25N01G_ReadFromCache(temp_page_buf, 0, W25N01G_PAGE_SIZE) != 0U)
            {
                memset(temp_page_buf, 0xFF, W25N01G_PAGE_SIZE);
            }
        }
        else
        {
            // 如果读取失败（例如刚擦除的新页），则默认全 0xFF
            memset(temp_page_buf, 0xFF, W25N01G_PAGE_SIZE);
        }

        // 2. 在 MCU 内存中将需要修改的片段更新进去（Read-Modify-Write）
        memcpy(&temp_page_buf[col], pSrc, len);

        // 3. 将整整 2048 字节一次性加载回芯片内部缓存
        if(QSPI_W25N01G_LoadProgramData(temp_page_buf, 0, W25N01G_PAGE_SIZE) != 0U)
        {
            return 3;
        }
    }
    else
    {
        // 如果刚好是完整的一页，直接加载即可
        if(QSPI_W25N01G_LoadProgramData(pSrc, col, len) != 0U)
        {
            return 3;
        }
    }

    // 4. 执行页编程前写使能
    if(QSPI_W25N01G_WriteEnable() != 0U)
        return 4;

    // 5. 触发烧录，将缓存写入 NAND 物理页
    if(QSPI_W25N01G_ProgramExecute(page) != 0U)
    {
        return 5;
    }
    
    return 0U;
}

/**
 * @brief W25N01G原始写接口
 * @note ⚠️重要！NAND不支持原地覆写！
 *       调用此函数前，**该数据所在块必须预先执行W25N_BlockErase擦除**；
 *       没有擦除直接写会数据错乱；本接口不会自动备份原有块内容。
 * @param offset 全局字节偏移
 * @param buf 待写入数据源
 * @param len 写入字节数
 * @retval 0成功
 */
uint8_t W25N_Write(uint32_t offset, uint8_t *buf, uint32_t len)
{
    if(buf == NULL || len == 0U)
        return 1;
    if((offset + len) > W25N01G_TOTAL_SIZE)
        return 2;

    uint32_t remain = len;
    uint8_t *pSrc = buf;

    while(remain > 0U)
    {
        uint32_t page;
        uint16_t col;
        uint32_t block;
        W25N_GetAddrInfo(offset, &page, &col, &block);

        uint16_t chunk = remain;
        if((col + chunk) > W25N01G_PAGE_SIZE)
        {
            chunk = W25N01G_PAGE_SIZE - col;
        }

        if(W25N_PageWriteInner(page, col, pSrc, chunk) != 0U)
        {
            return 3;
        }

        pSrc += chunk;
        offset += chunk;
        remain -= chunk;
    }
    return 0U;
}

/**
 * @brief 擦除完整128KB块
 * @param blockNo 块编号
 * @retval 0成功
 */
uint8_t W25N_BlockErase(uint32_t blockNo)
{
    uint32_t maxBlock = W25N01G_TOTAL_SIZE / W25N01G_BLOCK_SIZE;
    if(blockNo >= maxBlock)
        return 1;
    return QSPI_W25N01G_BlockErase(blockNo);
}

/**
 * @brief 【块修改辅助函数】修改块内部部分数据
 * @note 内部自动：读整个块到RAM缓冲区 → 修改 → 擦除块 → 写回整块
 * @warning 需要RAM缓冲区大小=W25N01G_BLOCK_SIZE(128KB)；MCU要有足够RAM！
 * @param blockNo:要修改的块号
 * @param blockOffsetIn:块内字节偏移 0~(128*1024‑1)
 * @param pData:待写入数据
 * @param len:写入长度
 * @param pBlockBuf:传入128KB RAM缓冲区
 * @retval 0成功
 */
uint8_t W25N_ModifyBlock(uint32_t blockNo, uint32_t blockOffsetIn, uint8_t *pData, uint32_t len, uint8_t *pBlockBuf)
{
    if(pBlockBuf == NULL)
        return 1;
    if((blockOffsetIn + len) > W25N01G_BLOCK_SIZE)
        return 2;

    uint32_t blockStartOffset = blockNo * W25N01G_BLOCK_SIZE;
    /*1.读取整个块内容到内存 */
    if(W25N_Read(blockStartOffset, pBlockBuf, W25N01G_BLOCK_SIZE) !=0 )
        return 3;
    /*2.内存中修改对应区域 */
    memcpy(&pBlockBuf[blockOffsetIn], pData, len);
    /*3.擦除Flash块 */
    if(W25N_BlockErase(blockNo) !=0 )
        return 4;
    /*4.把修改后的整块写回NAND */
    if(W25N_Write(blockStartOffset, pBlockBuf, W25N01G_BLOCK_SIZE) !=0 )
        return 5;
    return 0;
}





//***************************

/* 内存坏块表：上电扫描填充，不存flash */
uint8_t g_bad_block_table[W25N01G_TOTAL_BLOCKS / 8U] = {0};

/**
 * @brief 标记某块为坏块(仅内存表，不写flash)
 * @param blockNo 物理块号
 */
uint8_t W25N_MarkBadBlock(uint32_t blockNo)
{
    if(blockNo >= W25N01G_TOTAL_BLOCKS)
        return 1;
    uint32_t byte_idx = blockNo / 8U;
    uint32_t bit_idx  = blockNo % 8U;
    g_bad_block_table[byte_idx] |= (1U << bit_idx);
    return 0;
}

/**
 * @brief 判断块是否为坏块（查询内存坏块表）
 * @retval 0=好块，1=坏块
 */
uint8_t W25N_IsBadBlock(uint32_t blockNo)
{
    if(blockNo >= W25N01G_TOTAL_BLOCKS)
        return 1;
    uint32_t byte_idx = blockNo / 8U;
    uint32_t bit_idx  = blockNo % 8U;
    if( (g_bad_block_table[byte_idx] & (1U << bit_idx)) !=0 )
    {
        return W25N01G_BLOCK_BAD;
    }
    return W25N01G_BLOCK_GOOD;
}

/**
 * @brief 从startBlock开始，寻找下一个好块
 * @param startBlock 起始搜索块号
 * @retval 找到的好块号；超出范围返回0xFFFFFFFF代表无可用块
 */
uint32_t W25N_GetNextGoodBlock(uint32_t startBlock)
{
    uint32_t blk;
    for(blk = startBlock; blk < W25N01G_TOTAL_BLOCKS; blk++)
    {
        if(W25N_IsBadBlock(blk) == W25N01G_BLOCK_GOOD)
        {
            return blk;
        }
    }
    return 0xFFFFFFFFU;
}

/**
 * @brief 上电扫描坏块表：读取每块第0页OOB区(列地址2048)坏块标记
 * @note ⚠️！！！必须在任何擦除操作**之前调用**，擦除会清除出厂OOB坏块标记！
 * @retval 0扫描成功；非0失败
 */
uint8_t W25N_ScanBadBlockTable(void)
{
    uint32_t blkNo;
    uint32_t pageNo;
    uint8_t mark;

    /* 清空坏块表，初始全部标记为好块 */
    memset(g_bad_block_table, 0x00, sizeof(g_bad_block_table));

    SYSTEM_DEBUG("\r\n==== W25N01G Scan Factory Bad Block Start ====\r\n");

    for(blkNo = 0; blkNo < W25N01G_TOTAL_BLOCKS; blkNo++)
    {
        pageNo = blkNo * 64U; /*每块64页，取块内第0页 */

        /* 将物理页拷贝进芯片SRAM缓存 */
        if(QSPI_W25N01G_PageReadToCache(pageNo) !=0U )
        {
            SYSTEM_DEBUG("ScanBlk[%lu] PageReadToCache fail -> mark bad\r\n",blkNo);
            W25N_MarkBadBlock(blkNo);
            continue;
        }
        /* 读取OOB区第一个字节：col=2048，W25N01GV出厂坏块标记位置 */
        if(QSPI_W25N01G_ReadFromCache(&mark, 2048U, 1U) !=0U )
        {
            SYSTEM_DEBUG("ScanBlk[%lu] ReadOOB fail -> mark bad\r\n",blkNo);
            W25N_MarkBadBlock(blkNo);
            continue;
        }

        /* OOB[0] !=0xFF →出厂坏块 */
        if(mark != 0xFFU)
        {
            W25N_MarkBadBlock(blkNo);
            SYSTEM_DEBUG("Detect Factory Bad Block : Blk %lu , mark=0x%02X\r\n",blkNo,mark);
        }
    }

    /* 统计坏块总数 */
    uint32_t bad_cnt=0;
    for(blkNo=0;blkNo<W25N01G_TOTAL_BLOCKS;blkNo++)
    {
        if(W25N_IsBadBlock(blkNo)) bad_cnt++;
    }
    SYSTEM_DEBUG("Scan Complete! Total bad blocks: %lu \r\n",bad_cnt);
    SYSTEM_DEBUG("==== W25N01G Scan Factory Bad Block End ====\r\n");
    return 0U;
}






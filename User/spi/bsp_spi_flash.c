#include "./spi/bsp_spi_flash.h"
#include "./sys/sysio.h"

/* 识别到的全局 Flash 参数对象 */
FlashDevice_t g_FlashDev = {0, FLASH_TYPE_UNKNOWN, 0, 0, 0};

/**
  * @brief  发送写使能
  */
void SPI_FLASH_WriteEnable(void)
{
    SPI_FLASH_CS_LOW();
    SPI_FLASH_SendByte(W25X_WriteEnable);
    SPI_FLASH_CS_HIGH();
}

/**
  * @brief  等待 Flash 内部写/擦除结束 (根据 NOR/NAND 自动路由)
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t status = 0;

    if (g_FlashDev.Type == FLASH_TYPE_NAND)
    {
        do
        {
            SPI_FLASH_CS_LOW();
            SPI_FLASH_SendByte(W25N_GET_FEATURE);
            SPI_FLASH_SendByte(STAT_REG_STATUS_ADDR); // 0xC0
            status = SPI_FLASH_ReadByte();
            SPI_FLASH_CS_HIGH();
        } while ((status & WIP_Flag) == SET);
    }
    else // NOR Flash
    {
        SPI_FLASH_CS_LOW();
        SPI_FLASH_SendByte(W25X_ReadStatusReg);
        do
        {
            status = SPI_FLASH_ReadByte();
        } while ((status & WIP_Flag) == SET);
        SPI_FLASH_CS_HIGH();
    }
}

/**
  * @brief  读取 JEDEC ID (自动兼容 NOR 和 NAND 指令格式)
  */
uint32_t SPI_FLASH_ReadID2(void)
{
    uint8_t temp[4] = {0};

    /* 1. 发送 Reset 指令 (0xFF) 给 SPI NAND 进行软复位 */
    SPI_FLASH_CS_LOW();
    SPI_FLASH_SendByte(0xFF);
    SPI_FLASH_CS_HIGH();
    HAL_Delay(2); // 等待复位完成 (tRST)

    /* 2. 尝试 SPI NAND (W25N01GV) 方式读取: Command(0x9F) + Dummy(0x00/0xFF) + 3 Bytes ID */
    SPI_FLASH_CS_LOW();
    SPI_FLASH_SendByte(0x9F);       // Read JEDEC ID 指令
    SPI_FLASH_SendByte(Dummy_Byte); // 0xFF Dummy 字节
    temp[0] = SPI_FLASH_ReadByte(); // MID  (预期: 0xEF)
    temp[1] = SPI_FLASH_ReadByte(); // DID1 (预期: 0xAA)
    temp[2] = SPI_FLASH_ReadByte(); // DID2 (预期: 0x21)
    SPI_FLASH_CS_HIGH();

    uint32_t id = ((uint32_t)temp[0] << 16) | ((uint32_t)temp[1] << 8) | temp[2];

    /* 如果读出来不是 0xFFFFFF 且匹配到 W25N01GV，则返回 */
    if (id != 0xFFFFFF && id != 0x000000)
    {
        return id;
    }

    /* 3. 如果失败，尝试标准 SPI NOR 方式读取: Command(0x9F) + 3 Bytes ID */
    SPI_FLASH_CS_LOW();
    SPI_FLASH_SendByte(0x9F);
    temp[0] = SPI_FLASH_ReadByte();
    temp[1] = SPI_FLASH_ReadByte();
    temp[2] = SPI_FLASH_ReadByte();
    SPI_FLASH_CS_HIGH();

    return ((uint32_t)temp[0] << 16) | ((uint32_t)temp[1] << 8) | temp[2];
}

// 在读取 ID 函数的最开头，加入 Reset 逻辑
void SPI_NAND_Reset(void)
{
    SPI_FLASH_CS_LOW();
    SPI_FLASH_SendByte(0xFF); // NAND Reset 指令
    SPI_FLASH_CS_HIGH();
    
    HAL_Delay(2); // 等待 tRST (最大约 1ms)
}

uint32_t SPI_FLASH_ReadID3(void)
{
    uint8_t temp[3] = {0};

    // 1. 先进行软件复位，确保 Flash 退出异常状态
    SPI_NAND_Reset();

    // 2. 发送 Read ID 指令 (0x9F) + 1 Byte Dummy
    SPI_FLASH_CS_LOW();
    
    SPI_FLASH_SendByte(0x9F);       // Read JEDEC ID Cmd
    SPI_FLASH_SendByte(0x00);       // Dummy Byte (0x00 或 0xFF 均可)
    
    temp[0] = SPI_FLASH_ReadByte(); // MFR ID  -> 应读出 0xEF
    temp[1] = SPI_FLASH_ReadByte(); // Device ID High -> 应读出 0xAA
    temp[2] = SPI_FLASH_ReadByte(); // Device ID Low  -> 应读出 0x21
    
    SPI_FLASH_CS_HIGH();

    return ((uint32_t)temp[0] << 16) | ((uint32_t)temp[1] << 8) | temp[2];
}

//#define FLASH_ID_W25N01GV   0xEFAA21  // W25N01GV 官方正确 ID



uint32_t SPI_FLASH_ReadID(void)
{
uint8_t temp[3] = {0};

    // 1. 发送 Reset 指令 (0xFF) 给 SPI NAND 进行软复位
    SPI_FLASH_CS_LOW();
    SPI_FLASH_SendByte(0xFF);
    SPI_FLASH_CS_HIGH();
    HAL_Delay(5); // 等待复位完成 (tRST)

    // 2. 读取 JEDEC ID (格式: 0x9F + 1 Byte Dummy + 3 Bytes Data)
    SPI_FLASH_CS_LOW();

    SPI_FLASH_SendByte(0x9F); // Read ID 指令
    SPI_FLASH_SendByte(0x00); // W25N01G 要求的 Dummy 字节

    temp[0] = SPI_FLASH_ReadByte(); // MID  (预期: 0xEF)
    temp[1] = SPI_FLASH_ReadByte(); // DID1 (预期: 0xAA)
    temp[2] = SPI_FLASH_ReadByte(); // DID2 (预期: 0x21)

    SPI_FLASH_CS_HIGH();

    return ((uint32_t)temp[0] << 16) | ((uint32_t)temp[1] << 8) | temp[2];
}

/**
  * @brief  兼容接口：读取 Device ID
  */
uint32_t SPI_FLASH_ReadDeviceID(void)
{
    return SPI_FLASH_ReadID();
}

/**
  * @brief  解除 W25N01GV 默认上电写保护
  */
static void W25N_Unlock_Protection(void)
{
    SPI_FLASH_CS_LOW();
    SPI_FLASH_SendByte(W25N_SET_FEATURE);
    SPI_FLASH_SendByte(STAT_REG_PROTECTION_ADDR);
    SPI_FLASH_SendByte(0x00); // 0x00 代表无保护
    SPI_FLASH_CS_HIGH();
}

/**
  * @brief  自动识别芯片并配置参数
  */
void SPI_FLASH_Init(void)
{
    MX_SPI_A_Init();

    g_FlashDev.JedecID = SPI_FLASH_ReadID();

    switch (g_FlashDev.JedecID)
    {
//			case W25Q40_JEDEC_ID:                   // 新增 W25Q40 适配
//            g_FlashDev.Type = FLASH_TYPE_NOR;
//            g_FlashDev.SectorSize = 4096;        // 4KB Sector
//            g_FlashDev.PageSize = 256;           // 256B Page
//            g_FlashDev.TotalSize = 512 * 1024;   // 512KB (4Mbit)
//            break;
			
        case W25Q16_JEDEC_ID:
            g_FlashDev.Type = FLASH_TYPE_NOR;
            g_FlashDev.SectorSize = 4096;
            g_FlashDev.PageSize = 256;
            g_FlashDev.TotalSize = 2 * 1024 * 1024;
            break;

        case W25Q64_JEDEC_ID:
            g_FlashDev.Type = FLASH_TYPE_NOR;
            g_FlashDev.SectorSize = 4096;
            g_FlashDev.PageSize = 256;
            g_FlashDev.TotalSize = 8 * 1024 * 1024;
            break;

        case W25Q128_JEDEC_ID:
            g_FlashDev.Type = FLASH_TYPE_NOR;
            g_FlashDev.SectorSize = 4096;
            g_FlashDev.PageSize = 256;
            g_FlashDev.TotalSize = 16 * 1024 * 1024;
            break;

        case W25Q256_JEDEC_ID:
            g_FlashDev.Type = FLASH_TYPE_NOR;
            g_FlashDev.SectorSize = 4096;
            g_FlashDev.PageSize = 256;
            g_FlashDev.TotalSize = 32 * 1024 * 1024;

            /* 进入 4 字节地址模式 */
            SPI_FLASH_CS_LOW();
            SPI_FLASH_SendByte(W25X_Enter4ByteMode);
            SPI_FLASH_CS_HIGH();
            break;

        case W25N01GV_JEDEC_ID:
            g_FlashDev.Type = FLASH_TYPE_NAND;
            g_FlashDev.SectorSize = 128 * 1024; // 128KB Block 块擦除
            g_FlashDev.PageSize = 2048;        // 2KB Page
            g_FlashDev.TotalSize = 128 * 1024 * 1024;

            /* NAND 必做：解锁芯片上电写保护 */
            W25N_Unlock_Protection();
            break;

        default:
            g_FlashDev.Type = FLASH_TYPE_UNKNOWN;
            FLASH_SPI_ERROR("Unknown Flash ID: 0x%X\r\n", g_FlashDev.JedecID);
            break;
    }
}

/**
  * @brief  统一擦除接口（自动换算单位与指令）
  * @param  Addr: 擦除起始字节地址
  */
void SPI_FLASH_SectorErase(uint32_t Addr)
{
    if (g_FlashDev.Type == FLASH_TYPE_NAND)
    {
        uint32_t page_addr = Addr / g_FlashDev.PageSize;

        SPI_FLASH_WriteEnable();
        SPI_FLASH_CS_LOW();
        SPI_FLASH_SendByte(W25N_BLOCK_ERASE);
        SPI_FLASH_SendByte(Dummy_Byte);
        SPI_FLASH_SendByte((page_addr >> 8) & 0xFF);
        SPI_FLASH_SendByte(page_addr & 0xFF);
        SPI_FLASH_CS_HIGH();
        SPI_FLASH_WaitForWriteEnd();
    }
    else // NOR Flash
    {
        SPI_FLASH_WriteEnable();
        SPI_FLASH_CS_LOW();
        
        if (g_FlashDev.JedecID == W25Q256_JEDEC_ID)
        {
            SPI_FLASH_SendByte(W25X_SectorErase);
            SPI_FLASH_SendByte((Addr & 0xFF000000) >> 24);
            SPI_FLASH_SendByte((Addr & 0xFF0000) >> 16);
            SPI_FLASH_SendByte((Addr & 0xFF00) >> 8);
            SPI_FLASH_SendByte(Addr & 0xFF);
        }
        else
        {
            SPI_FLASH_SendByte(W25X_SectorErase);
            SPI_FLASH_SendByte((Addr & 0xFF0000) >> 16);
            SPI_FLASH_SendByte((Addr & 0xFF00) >> 8);
            SPI_FLASH_SendByte(Addr & 0xFF);
        }
        
        SPI_FLASH_CS_HIGH();
        SPI_FLASH_WaitForWriteEnd();
    }
}

/**
  * @brief  统一读数据接口
  */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
    if (g_FlashDev.Type == FLASH_TYPE_NAND)
    {
        while (NumByteToRead > 0)
        {
            uint32_t page_addr = ReadAddr / g_FlashDev.PageSize;
            uint16_t col_addr = ReadAddr % g_FlashDev.PageSize;
            uint16_t single_read_len = (col_addr + NumByteToRead > g_FlashDev.PageSize) ? 
                                       (g_FlashDev.PageSize - col_addr) : NumByteToRead;

            /* 1. Page Read to Buffer */
            SPI_FLASH_CS_LOW();
            SPI_FLASH_SendByte(W25N_PAGE_DATA_READ);
            SPI_FLASH_SendByte(Dummy_Byte);
            SPI_FLASH_SendByte((page_addr >> 8) & 0xFF);
            SPI_FLASH_SendByte(page_addr & 0xFF);
            SPI_FLASH_CS_HIGH();

            SPI_FLASH_WaitForWriteEnd();

            /* 2. Read from Buffer */
            SPI_FLASH_CS_LOW();
            SPI_FLASH_SendByte(W25N_READ_FROM_BUFFER);
            SPI_FLASH_SendByte((col_addr >> 8) & 0xFF);
            SPI_FLASH_SendByte(col_addr & 0xFF);
            SPI_FLASH_SendByte(Dummy_Byte);

            for (uint16_t i = 0; i < single_read_len; i++)
            {
                *pBuffer++ = SPI_FLASH_ReadByte();
            }
            SPI_FLASH_CS_HIGH();

            ReadAddr += single_read_len;
            NumByteToRead -= single_read_len;
        }
    }
    else // NOR Flash
    {
        SPI_FLASH_CS_LOW();
        SPI_FLASH_SendByte(W25X_ReadData);

        if (g_FlashDev.JedecID == W25Q256_JEDEC_ID)
        {
            SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
        }
        SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
        SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
        SPI_FLASH_SendByte(ReadAddr & 0xFF);

        while (NumByteToRead--)
        {
            *pBuffer++ = SPI_FLASH_ReadByte();
        }
        SPI_FLASH_CS_HIGH();
    }
}

/**
  * @brief  单页写入底层实现
  */
static void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t PageAddr, uint16_t ColAddr, uint16_t NumByteToWrite)
{
    SPI_FLASH_WriteEnable();

    if (g_FlashDev.Type == FLASH_TYPE_NAND)
    {
        /* 1. Load Program Data */
        SPI_FLASH_CS_LOW();
        SPI_FLASH_SendByte(W25N_LOAD_PROGRAM_DATA);
        SPI_FLASH_SendByte((ColAddr >> 8) & 0xFF);
        SPI_FLASH_SendByte(ColAddr & 0xFF);
        for (uint16_t i = 0; i < NumByteToWrite; i++)
        {
            SPI_FLASH_SendByte(pBuffer[i]);
        }
        SPI_FLASH_CS_HIGH();

        /* 2. Program Execute */
        SPI_FLASH_CS_LOW();
        SPI_FLASH_SendByte(W25N_PROGRAM_EXECUTE);
        SPI_FLASH_SendByte(Dummy_Byte);
        SPI_FLASH_SendByte((PageAddr >> 8) & 0xFF);
        SPI_FLASH_SendByte(PageAddr & 0xFF);
        SPI_FLASH_CS_HIGH();

        SPI_FLASH_WaitForWriteEnd();
    }
    else // NOR Flash
    {
        uint32_t byte_addr = PageAddr * g_FlashDev.PageSize + ColAddr;

        SPI_FLASH_CS_LOW();
        SPI_FLASH_SendByte(W25X_PageProgram);

        if (g_FlashDev.JedecID == W25Q256_JEDEC_ID)
        {
            SPI_FLASH_SendByte((byte_addr & 0xFF000000) >> 24);
        }
        SPI_FLASH_SendByte((byte_addr & 0xFF0000) >> 16);
        SPI_FLASH_SendByte((byte_addr & 0xFF00) >> 8);
        SPI_FLASH_SendByte(byte_addr & 0xFF);

        while (NumByteToWrite--)
        {
            SPI_FLASH_SendByte(*pBuffer++);
        }
        SPI_FLASH_CS_HIGH();

        SPI_FLASH_WaitForWriteEnd();
    }
}

/**
  * @brief  统一写数据接口（自动处理跨页计算）
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    while (NumByteToWrite > 0)
    {
        uint32_t page_addr = WriteAddr / g_FlashDev.PageSize;
        uint16_t col_addr = WriteAddr % g_FlashDev.PageSize;
        uint16_t single_write_len = (col_addr + NumByteToWrite > g_FlashDev.PageSize) ? 
                                    (g_FlashDev.PageSize - col_addr) : NumByteToWrite;

        SPI_FLASH_PageWrite(pBuffer, page_addr, col_addr, single_write_len);

        pBuffer += single_write_len;
        WriteAddr += single_write_len;
        NumByteToWrite -= single_write_len;
    }
}

uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
    FLASH_SPI_ERROR("SPI overtime!errorCode = %d", errorCode);
    return 0;
}
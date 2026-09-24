
#include "./test/bsp_qspi_flash_test.h"

#include "./DataConvert/data_compare.h"

#include "./user_test_config.h"
 #include "./sys/sysio.h"
#if TEST_QSPI_FLASH
//#define TEST_ADDR        0x00000000     /* 测试起始地址 (第 0 块) */
//#define TEST_SIZE        2048           /* 测试数据大小 (1 个 Page) */

//uint8_t Tx2_Buffer[TEST_SIZE];
//uint8_t Rx2_Buffer[TEST_SIZE];

void QSPI_Flash_Test(void)
{
    uint32_t jedec_id = 0;
    uint8_t w_buf[2048];   // 一页最大2048字节
    uint8_t r_buf[2048];
    uint32_t i;
    const uint32_t test_block = 0;   //测试块0
    const uint32_t test_page  = 0;   //测试块内page0
    uint16_t test_len = 256;         //只测试256字节，不要写满2048

    SYSTEM_DEBUG("\r\n===== W25N01G QSPI Test Start =====\r\n");

    /* step1 初始化QSPI + 芯片复位 + 读ID + 解锁保护 */
    

    jedec_id = QSPI_W25N01G_ReadJedecID();
    SYSTEM_DEBUG("JEDEC ID = 0x%06X \r\n", jedec_id);
    if(jedec_id != W25N01G_JEDEC_ID)
    {
        SYSTEM_DEBUG("[FAIL] ID mismatch! expect 0xEFAA21\r\n");
        return ;
    }
    SYSTEM_DEBUG("[OK] ID match W25N01G\r\n");

    /* step2 块擦除，擦除测试块（块0，128KB）*/
    SYSTEM_DEBUG("Erase block %lu ...\r\n", test_block);
    if(QSPI_W25N01G_BlockErase(test_block) != 0)
    {
        SYSTEM_DEBUG("[FAIL] Block Erase fail\r\n");
        return ;
    }
    SYSTEM_DEBUG("[OK] Block erase done\r\n");

    /* step3 填充测试数据 0~255循环 */
    for(i=0;i<test_len;i++)
    {
        w_buf[i] = i & 0xFFU;
    }
    /* 剩余部分填充0，本次不使用 */
    for(i=test_len;i<2048;i++)
    {
        w_buf[i] = 0U;
    }

    /* step4 NAND写流程：LoadProgramData加载到缓存 → ProgramExecute烧录到NAND页 */
    SYSTEM_DEBUG("Write page %lu len=%d\r\n",test_page, test_len);
    //①把数据加载到芯片内部SRAM缓存，列地址从0开始
    if(QSPI_W25N01G_LoadProgramData(w_buf, 0, test_len) != 0)
    {
        SYSTEM_DEBUG("[FAIL] LoadProgramData fail\r\n");
        return ;
    }
    //②执行烧录：缓存数据写入NAND物理页
    if(QSPI_W25N01G_ProgramExecute(test_page) !=0 )
    {
        SYSTEM_DEBUG("[FAIL] ProgramExecute fail\r\n");
        return ;
    }
    SYSTEM_DEBUG("[OK] Page write done\r\n");

    /* step5 NAND读流程：PageReadToCache(把NAND物理页拷贝到内部缓存) → ReadFromCache读缓存 */
    SYSTEM_DEBUG("Read page %lu\r\n",test_page);
    if(QSPI_W25N01G_PageReadToCache(test_page) !=0 )
    {
        SYSTEM_DEBUG("[FAIL] PageReadToCache fail\r\n");
        return ;
    }
    if(QSPI_W25N01G_ReadFromCache(r_buf,0, test_len) !=0 )
    {
        SYSTEM_DEBUG("[FAIL] ReadFromCache fail\r\n");
        return ;
    }
    SYSTEM_DEBUG("[OK] Page read done\r\n");

    /* step6 校验写入和读出数据 */
    for(i=0;i<test_len;i++)
    {
        if(w_buf[i] != r_buf[i])
        {
            SYSTEM_DEBUG("[FAIL] data mismatch idx:%lu wr:0x%02X rd:0x%02X\r\n",i,w_buf[i],r_buf[i]);
            return ;
        }
    }
    SYSTEM_DEBUG("[OK] Data verify PASS!\r\n");
    SYSTEM_DEBUG("===== W25N01G QSPI Test ALL PASS =====\r\n");
    return ;
	}
//｝
		
		
		
		
#endif
















/*********************************************END OF FILE**********************/


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
    uint8_t id_buf[3] = {0};

    SYSTEM_DEBUG("\r\n================ QSPI W25N01G Test Start ================\r\n");

    /* 1. 初始化 QSPI 接口 */
    if (BSP_QSPI_Init() == QSPI_OK)
    {
        SYSTEM_DEBUG("<-DEBUG-> [OK] QSPI Init Success\r\n");
    }
    else
    {
        SYSTEM_DEBUG("<-DEBUG-> [FAIL] QSPI Init Failed!\r\n");
        return;
    }

    /* 2. 读取 JEDEC ID */
    if (QSPI_FLASH_ReadID(id_buf) == QSPI_OK)
    {
        SYSTEM_DEBUG("<-DEBUG-> [INFO] Flash JEDEC ID: 0x%02X 0x%02X 0x%02X\r\n", id_buf[0], id_buf[1], id_buf[2]);

        /* 验证 ID 是否为 W25N01GV (0xEF 0xAA 0x21) */
        if (id_buf[0] == 0xEF && id_buf[1] == 0xAA && id_buf[2] == 0x21)
        {
            SYSTEM_DEBUG("<-DEBUG-> [OK] Flash Chip ID Matched: W25N01GVZEIG!\r\n");
        }
        else
        {
            SYSTEM_DEBUG("<-DEBUG-> [WARN] Flash Chip ID Mismatch! Check GPIO or Hardware!\r\n");
        }
    }
    else
    {
        SYSTEM_DEBUG("<-DEBUG-> [FAIL] QSPI Read ID Command Execution Failed!\r\n");
    }
	}
//｝
		
		
		
		
#endif
















/*********************************************END OF FILE**********************/

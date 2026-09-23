


#include "./test/bsp_spi_test.h"
//#include "./usart/bsp_usart.h"

#include "./DataConvert/data_compare.h"


#include "./spi/bsp_spi_flash.h"

//#include "stm32f4xx_hal.h"
//#include "./spi/bsp_spi_flash.h"

#define TEST_BUFFER_SIZE    4096       // Test payload size (4KB)
#define FLASH_TEST_ADDR     0x00000000 // Test start address

uint8_t Tx_Buffer[TEST_BUFFER_SIZE];
uint8_t Rx_Buffer[TEST_BUFFER_SIZE];

/**
  * @brief  SPI Flash Read/Write Test Program
  * @retval 1: PASS, 0: FAIL
  */
uint8_t Flash_Test(void)
{
    uint32_t i = 0;

    SYSTEM_DEBUG("\r\n================ SPI Flash Test Routine ================\r\n");

    /* 1. Print Flash Device Information */
    SYSTEM_DEBUG("1. Detected Flash Device Information:\r\n");
    SYSTEM_DEBUG("   - JEDEC ID    : 0x%06X\r\n", g_FlashDev.JedecID);
    
    if (g_FlashDev.Type == FLASH_TYPE_NOR)
    {
        SYSTEM_DEBUG("   - Flash Type  : SPI NOR Flash\r\n");
    }
    else if (g_FlashDev.Type == FLASH_TYPE_NAND)
    {
        SYSTEM_DEBUG("   - Flash Type  : SPI NAND Flash (W25N01GV)\r\n");
    }
    else
    {
        SYSTEM_DEBUG("   - Flash Type  : Unknown / Chip Not Found\r\n");
        return 0;
    }

    SYSTEM_DEBUG("   - Capacity    : %lu MB (%lu Bytes)\r\n", g_FlashDev.TotalSize / (1024 * 1024), g_FlashDev.TotalSize);
    SYSTEM_DEBUG("   - Erase Size  : %lu KB\r\n", g_FlashDev.SectorSize / 1024);
    SYSTEM_DEBUG("   - Page Size   : %lu Bytes\r\n", g_FlashDev.PageSize);

    /* 2. Prepare Tx Buffer Pattern */
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    {
        Tx_Buffer[i] = (uint8_t)(i & 0xFF); // Fill cyclic pattern 0x00 ~ 0xFF
        Rx_Buffer[i] = 0;                   // Clear Rx buffer
    }

    /* 3. Erase Target Area */
    SYSTEM_DEBUG("\r\n2. Erasing target area (Addr: 0x%08X)...\r\n", FLASH_TEST_ADDR);
    SPI_FLASH_SectorErase(FLASH_TEST_ADDR);
    SYSTEM_DEBUG("   -> Erase Completed!\r\n");

    /* 4. Write Test Data */
    SYSTEM_DEBUG("\r\n3. Writing test pattern (%d Bytes)...\r\n", TEST_BUFFER_SIZE);
    SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_TEST_ADDR, TEST_BUFFER_SIZE);
    SYSTEM_DEBUG("   -> Write Completed!\r\n");

    /* 5. Read Test Data */
    SYSTEM_DEBUG("\r\n4. Reading data back from Flash...\r\n");
    SPI_FLASH_BufferRead(Rx_Buffer, FLASH_TEST_ADDR, TEST_BUFFER_SIZE);
    SYSTEM_DEBUG("   -> Read Completed!\r\n");

    /* 6. Verify Data Integrity */
    SYSTEM_DEBUG("\r\n5. Verifying written data against read data...\r\n");
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    {
        if (Tx_Buffer[i] != Rx_Buffer[i])
        {
            SYSTEM_DEBUG("ERROR: Mismatch at Addr 0x%08X! (Tx: 0x%02X, Rx: 0x%02X)\r\n",
                         (FLASH_TEST_ADDR + i), Tx_Buffer[i], Rx_Buffer[i]);
            return 0; // Test Failed
        }
    }

    SYSTEM_DEBUG("SUCCESS: All %d Bytes verified successfully! Flash driver is fully operational.\r\n", TEST_BUFFER_SIZE);
    SYSTEM_DEBUG("========================================================\r\n");
    
    return 1;
}

/**
  * @brief  Main Application Entry Point
  */
void SPI_FLASH_Test(void)
{
//    HAL_Init();

//    SYSTEM_DEBUG("\r\n***** Universal SPI Flash (NOR / NAND) Demo *****\r\n");

//    /* Initialize SPI Flash with Auto-Detection & Protection Unlock */
    SPI_FLASH_Init();

    /* Execute Flash Read/Write Test */
    if (Flash_Test() == 1)
    {
        SYSTEM_DEBUG("\r\n>>> Flash Test PASSED! Entering main program loop... <<<\r\n");
    }
    else
    {
        SYSTEM_DEBUG("\r\n!!! Flash Test FAILED! Please verify hardware connections or chip status. !!!\r\n");
    }

    while (1)
    {
    }
}












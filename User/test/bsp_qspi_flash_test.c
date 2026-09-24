
#include "./test/bsp_qspi_flash_test.h"

#include "./DataConvert/data_compare.h"

#include "./user_test_config.h"
 #include "./sys/sysio.h"
#if TEST_QSPI_FLASH
//#define TEST_ADDR        0x00000000     /* 测试起始地址 (第 0 块) */
//#define TEST_SIZE        2048           /* 测试数据大小 (1 个 Page) */

//uint8_t Tx2_Buffer[TEST_SIZE];
//uint8_t Rx2_Buffer[TEST_SIZE];

void QSPI_W25N01G_Test(void);
void Test_W25N_UpperApi(void);
uint8_t Test_W25N_Extend(void);



void QSPI_Flash_Test(void){



//QSPI_W25N01G_Test();
/* 必须先扫描坏块！！在所有擦除之前 */
//W25N_ScanBadBlockTable();
	
	
    W25N_ScanBadBlockTable(); //必须最先执行，任何擦除之前！

    Test_W25N_UpperApi();     //基础单页读写
    Test_W25N_Extend();       //扩展：跨页 + 块边界	
//Test_W25N_UpperApi();







}


void QSPI_W25N01G_Test(void)
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
		
		
		
	
	
	
	
//	### 示例1：标准读写（**写前必须擦除块**）
void Test_W25N_UpperApi(void)
{
    uint8_t wbuf[256];
    uint8_t rbuf[256];
    uint32_t i;
    uint32_t test_offset = 0U;
    uint32_t block_no = test_offset / (W25N01G_BLOCK_SIZE);

    for(i=0;i<sizeof(wbuf);i++)
    {
        wbuf[i] = i & 0xFFU;
    }

    //NAND写前：必须擦除所属块！！
    if(W25N_BlockErase(block_no) != 0U)
    {
        SYSTEM_DEBUG("Block erase fail!\r\n");
        return;
    }

    //写入
    if(W25N_Write(test_offset, wbuf, sizeof(wbuf)) !=0U)
    {
        SYSTEM_DEBUG("W25N_Write fail\r\n");
        return;
    }
    //读出
    if(W25N_Read(test_offset, rbuf, sizeof(rbuf)) !=0U)
    {
        SYSTEM_DEBUG("W25N_Read fail\r\n");
        return;
    }
    //校验
    uint8_t pass = 1U;
    for(i=0;i<sizeof(wbuf);i++)
    {
        if(wbuf[i] != rbuf[i])
        {
            SYSTEM_DEBUG("mismatch idx:%lu wr:0x%02X rd:0x%02X\r\n",i,wbuf[i],rbuf[i]);
            pass = 0U;
            break;
        }
    }
    if(pass)
    {
        SYSTEM_DEBUG("Upper api test PASS\r\n");
    }
}	
	
	
//	### 示例2：修改块中一小部分（ModifyBlock）
//> ⚠️需要128KB RAM缓冲区；H743内存充足可以用；注意不要在栈上定义大数组，要用全局/静态缓冲区。
//```c
static uint8_t g_block_buf[128*1024]; //全局128KB缓冲区

void Test_Modify(void)
{
    uint8_t data[16] = {11,22,33,44,55,66,77,88,99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0x10};
    //修改块0，块内偏移100字节，写入16字节
    uint8_t ret = W25N_ModifyBlock(0U,100U, data, sizeof(data), g_block_buf);
    if(ret == 0)
    {
        SYSTEM_DEBUG("Modify block ok\r\n");
    }
}
	





/**
 * @brief 扩展测试：跨页读写 + 块边界读写
 * @retval 0全部通过，非0失败
 */
uint8_t Test_W25N_Extend(void)
{
    SYSTEM_DEBUG("\r\n==== W25N01G Extend Test Start ====\r\n");

    /*----------测试1：跨页读写，offset=2000，长度128，跨过页边界2048----------*/
    {
        const uint32_t test_offset1 = 2000U;
        const uint32_t test_len1    = 128U;
        uint8_t wbuf1[128];
        uint8_t rbuf1[128];
        uint32_t i;

        //获取所属块，擦除
        uint32_t page,col,block;
        W25N_GetAddrInfo(test_offset1, &page, &col, &block);
        if(W25N_IsBadBlock(block))
        {
            SYSTEM_DEBUG("Test1: block %lu is bad, skip test1\r\n",block);
        }
        else
        {
            SYSTEM_DEBUG("\r\n[Test1] Cross‑page test offset=%lu len=%lu\r\n",test_offset1,test_len1);
            for(i=0;i<test_len1;i++)
            {
                wbuf1[i] = (0x10 + i) & 0xFFU;
            }
            if(W25N_BlockErase(block) !=0)
            {
                SYSTEM_DEBUG("Test1 BlockErase fail\r\n");
                return 1;
            }
            if(W25N_Write(test_offset1, wbuf1, test_len1)!=0)
            {
                SYSTEM_DEBUG("Test1 W25N_Write fail\r\n");
                return 2;
            }
            if(W25N_Read(test_offset1, rbuf1, test_len1)!=0)
            {
                SYSTEM_DEBUG("Test1 W25N_Read fail\r\n");
                return 3;
            }
            uint8_t ok = 1;
            for(i=0;i<test_len1;i++)
            {
                if(wbuf1[i] != rbuf1[i])
                {
                    SYSTEM_DEBUG("Test1 mis idx:%lu wr:0x%02X rd:0x%02X\r\n",i,wbuf1[i],rbuf1[i]);
                    ok =0;
                    break;
                }
            }
            if(ok)
            {
                SYSTEM_DEBUG("[Test1] Cross‑page test PASS\r\n");
            }
            else
            {
                SYSTEM_DEBUG("[Test1] Cross‑page test FAIL\r\n");
                return 4;
            }
        }
    }

    /*----------测试2：块边界读写；offset=131072‑16，len=32；跨块0 / 块1边界----------*/
    {
        const uint32_t block0_size = W25N01G_BLOCK_SIZE; //131072
        const uint32_t test_offset2 = block0_size - 16U;  //块0最后16字节
        const uint32_t test_len2    = 32U;                //16字节块0末尾 +16字节块1开头
        uint8_t wbuf2[32];
        uint8_t rbuf2[32];
        uint32_t i;

        uint32_t page0,col0,block0;
        W25N_GetAddrInfo(test_offset2, &page0, &col0, &block0);

        uint32_t page1,col1,block1;
        W25N_GetAddrInfo(test_offset2 + test_len2 -1U, &page1, &col1, &block1);

        SYSTEM_DEBUG("\r\n[Test2] Block boundary cross test offset=%lu len=%lu\r\n",test_offset2,test_len2);
        SYSTEM_DEBUG("  start belong block %lu , end belong block %lu\r\n",block0,block1);

        //⚠️跨两个块，两个块都必须擦除！！
        if(W25N_IsBadBlock(block0) || W25N_IsBadBlock(block1))
        {
            SYSTEM_DEBUG("Test2 block0 or block1 bad, skip test2\r\n");
        }
        else
        {
            if(W25N_BlockErase(block0)!=0)
            {
                SYSTEM_DEBUG("Test2 erase block0 fail\r\n");
                return 5;
            }
            if(W25N_BlockErase(block1)!=0)
            {
                SYSTEM_DEBUG("Test2 erase block1 fail\r\n");
                return 6;
            }
            for(i=0;i<test_len2;i++)
            {
                wbuf2[i] = (0x20 + i) &0xFFU;
            }
            if(W25N_Write(test_offset2, wbuf2, test_len2)!=0)
            {
                SYSTEM_DEBUG("Test2 W25N_Write fail\r\n");
                return 7;
            }
            if(W25N_Read(test_offset2, rbuf2, test_len2)!=0)
            {
                SYSTEM_DEBUG("Test2 W25N_Read fail\r\n");
                return 8;
            }
            uint8_t ok=1;
            for(i=0;i<test_len2;i++)
            {
                if(wbuf2[i] != rbuf2[i])
                {
                    SYSTEM_DEBUG("Test2 mis idx:%lu wr:0x%02X rd:0x%02X\r\n",i,wbuf2[i],rbuf2[i]);
                    ok=0;
                    break;
                }
            }
            if(ok)
            {
                SYSTEM_DEBUG("[Test2] Block‑boundary cross test PASS\r\n");
            }
            else
            {
                SYSTEM_DEBUG("[Test2] Block‑boundary cross test FAIL\r\n");
                return 9;
            }
        }
    }

    SYSTEM_DEBUG("\r\n==== W25N01G All Extend Test PASS ====\r\n");
    return 0;
}







	
/*
	
 业务层使用示例
 示例1：操作前检查块是否坏块
```c
uint32_t blk = 0;
if(W25N_IsBadBlock(blk))
{
     当前块是坏块，自动跳到下一个好块 
    blk = W25N_GetNextGoodBlock(blk+1);
    if(blk == 0xFFFFFFFFU)
    {
        SYSTEM_DEBUG("No available good block!\r\n");
    }
}
```

## 本套坏块方案局限性（调试/简易产品）
1. ✅优点：简单，上电自动识别出厂坏块；不需要占用Flash存储坏块表；
2. ❗**局限1：只能识别【出厂坏块】，不能识别运行过程中产生的运行时坏块（擦除/编程失败产生的新坏块）。**
> 运行时，当`BlockErase` / `PageProgram`返回失败，业务层需要手动调用`W25N_MarkBadBlock(blockNo)`标记进内存坏块表。
3. ❗**掉电丢失坏块表**；上电必须重新扫描；
4. ❗扫描全部1024块，每块读一页OOB，上电扫描耗时几百ms；
> 如果不想上电扫描全部1024块，可以改为**按需扫描**：只有业务要使用某个块的时候才去读OOB判断，缩短上电时间。

## 运行时新增坏块标记示例（编程/擦除出错时调用）
```c
uint32_t blk_no = 2;
uint8_t ret = W25N_BlockErase(blk_no);
if(ret !=0 )
{
    /*擦除失败，标记该块为内存坏块表 
    W25N_MarkBadBlock(blk_no);
    /*寻找下一个可用好块 
    blk_no = W25N_GetNextGoodBlock(blk_no+1);
}
```
******/	
	
	
	
	
	
	
	
		
#endif
















/*********************************************END OF FILE**********************/

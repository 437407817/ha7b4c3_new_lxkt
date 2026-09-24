#include "./sensor/DS1302.h"


#include "./sys/sysio.h"




static GPIO_InitTypeDef  GPIO_InitStructure;

//IO引脚配置为输出
static void DS1302_OUT_GPIO_INIT(void)
{
    GPIO_InitStructure.Pin = DS1302_IO_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DS1302_IO_GPIO_PORT, &GPIO_InitStructure);
}

//IO引脚配置为输入
static void DS1302_IN_GPIO_INIT(void)
{
    GPIO_InitStructure.Pin = DS1302_IO_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DS1302_IO_GPIO_PORT, &GPIO_InitStructure);
}

void DS1302_GPIO_Init(void)
{
    //开启GPIO时钟 HAL库
    DS1302_SCLK_GPIO_CLK;
    DS1302_IO_GPIO_CLK;
    DS1302_RST_GPIO_CLK;

    //SCLK 推挽输出
    GPIO_InitStructure.Pin = DS1302_SCLK_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DS1302_SCLK_GPIO_PORT, &GPIO_InitStructure);

    //RST 推挽输出
    GPIO_InitStructure.Pin = DS1302_RST_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DS1302_RST_GPIO_PORT, &GPIO_InitStructure);

    DS1302_RST_L;
    DS1302_SCLK_L;
}

//写一个字节（地址或者数据）
void DS1302_WriteAddrOrData(uint8_t addrordata)
{
    uint8_t temp1;
    temp1=addrordata;
    DS1302_OUT_GPIO_INIT();

    for(uint8_t i=0;i<8;i++)
    {
        if(temp1&0x01)
        {
            DS1302_IO_H;
        }
        else
        {
            DS1302_IO_L;
        }
        temp1=temp1>>1;
        DS1302_SCLK_H;
        DS1302_DELAY;
        DS1302_SCLK_L;
        DS1302_DELAY;
    }
}

//写寄存器
void DS1302_WriteByte(uint8_t add,uint8_t dat)
{
    DS1302_RST_L;
    DS1302_DELAY;
    DS1302_SCLK_L;
    DS1302_DELAY;
    DS1302_RST_H;
    DS1302_DELAY;

    DS1302_WriteAddrOrData(add);
    DS1302_WriteAddrOrData(dat);

    DS1302_RST_L;
}

//读寄存器
uint8_t DS1302_Read(uint8_t add)
{
    uint8_t suf = 0U;
    DS1302_RST_H;
    DS1302_DELAY;
    DS1302_WriteAddrOrData(add);
    DS1302_IN_GPIO_INIT();

    for(uint8_t i=0;i<8;i++)
    {
        suf=suf>>1;
        if(DS1302_io_read)
        {
            suf=suf|0x80;
        }
        else
        {
            suf=suf&0x7f;
        }
        DS1302_SCLK_H;
        DS1302_DELAY;
        DS1302_SCLK_L;
        DS1302_DELAY;
    }
    DS1302_RST_L;
    DS1302_DELAY;
    return suf;
}

// 优化后的读寄存器函数
uint8_t DS1302_Read2(uint8_t add)
{
    uint8_t suf = 0U;
    uint8_t i = 0;
    
    DS1302_RST_L;
    DS1302_DELAY;
    DS1302_RST_H;          // RST拉高，开始通信
    DS1302_DELAY;
    
    DS1302_WriteAddrOrData(add); // 写入读地址（含读命令 0x01）
    
    // 关键点 1：写完地址后，IO口变更为输入模式
    DS1302_IN_GPIO_INIT();
    DS1302_DELAY;          // 增加微小延时，等待输入电平稳定

    for(i = 0; i < 8; i++)
    {
        suf >>= 1;         // DS1302是 LSB（低位先行），数据从右向左移
        
        // 关键点 2：在 SCLK 上升沿采样数据
        DS1302_SCLK_H;
        DS1302_DELAY;
        
        if(DS1302_io_read)
        {
            suf |= 0x80;   // 读到高电平，置 1
        }
        else
        {
            suf &= 0x7f;   // 读到低电平，清 0
        }
        
        DS1302_SCLK_L;     // SCLK拉低，为下一个下降沿输出做准备
        DS1302_DELAY;
    }
    
    DS1302_RST_L;          // 结束通信
    DS1302_DELAY;
    return suf;
}
uint8_t DS1302_Read3(uint8_t add)
{
    uint8_t suf = 0U;
    DS1302_RST_L;
    DS1302_DELAY;
    DS1302_SCLK_L;
    DS1302_DELAY;
    DS1302_RST_H;
    DS1302_DELAY;

    DS1302_WriteAddrOrData(add);   //发送读指令
    DS1302_IN_GPIO_INIT();         //IO切换输入

    for(uint8_t i=0;i<8;i++)
    {
        suf >>= 1;
        if(DS1302_io_read)
        {
            suf |= 0x80;
        }
        DS1302_SCLK_H;
        DS1302_DELAY;
        DS1302_SCLK_L;
        DS1302_DELAY;
    }
    DS1302_RST_L;
    DS1302_DELAY;
    return suf;
}

//设置时间，输入BCD数组 {sec,min,hour,day,month,year}
void DS1302_SetTime(uint8_t *ad)
{
    DS1302_WriteByte(0x8e,0x00);   //WP=0 解除写保护
    DS1302_WriteByte(DS1302_SEC_REG_ADDR,ad[0]/10*16+ad[0]%10);
    DS1302_WriteByte(DS1302_MIN_REG_ADDR,ad[1]/10*16+ad[1]%10);
    DS1302_WriteByte(DS1302_HOUR_REG_ADDR,ad[2]/10*16+ad[2]%10);
    DS1302_WriteByte(DS1302_DATE_REG_ADDR,ad[3]/10*16+ad[3]%10);
    DS1302_WriteByte(DS1302_MONTH_REG_ADDR,ad[4]/10*16+ad[4]%10);
    DS1302_WriteByte(DS1302_YEAR_REG_ADDR,ad[5]/10*16+ad[5]%10);
    DS1302_WriteByte(0x8e,0x80);   //WP=1 开启写保护
}

//DS1302初始化，检测时钟是否停止，停止则设置初始时间
void DS1302_init(uint8_t *time)
{
    DS1302_GPIO_Init();
    DS1302_DELAY;
    DS1302_RST_L;
    DS1302_SCLK_L;

    DS1302_WriteByte(0x8e,0x00);
    if(DS1302_Read(0X81)&0X80)
    {
        DS1302_SetTime(time);
    }
}

//读取时间，BCD转十进制
void DS1302_Readtime(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute,uint8_t *second )
{
	uint8_t mh,nl,value,suf;

    suf = DS1302_Read(DS1302_YEAR_REG_ADDR|0x01);
    mh=(suf&0xf0)>>4;
    nl=suf&0x0f;
    value=mh*10+nl;
    *year = value;

    suf = DS1302_Read(DS1302_MONTH_REG_ADDR|0x01);
    mh=(suf&0xf0)>>4;
    nl=suf&0x0f;
    value=mh*10+nl;
    *month = value;

    suf = DS1302_Read(DS1302_DATE_REG_ADDR|0x01);
    mh=(suf&0xf0)>>4;
    nl=suf&0x0f;
    value=mh*10+nl;
    *day = value;

    suf=DS1302_Read(DS1302_HOUR_REG_ADDR|0x01);
    mh=(suf&0x30)>>4;
    nl=suf&0x0f;
    value=mh*10+nl;
    *hour = value;

    suf=DS1302_Read(DS1302_MIN_REG_ADDR|0x01);
    mh=(suf&0xf0)>>4;
    nl=suf&0x0f;
    value=mh*10+nl;
    *minute = value;

suf = DS1302_Read(DS1302_SEC_REG_ADDR | 0x01);
mh = (suf & 0xf0) >> 4;
nl = suf & 0x0f;
value = mh * 10 + nl;
*second = value;
}






































#if 0



//4. 单字节读取函数
//依据 DS1302 的通信规则，在时钟线 SCLK 的下降沿从 DS1302 中逐位读取一个字节的数据。
uint8_t DS1302_ReadByte(void)
{
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++)
    {
        data >>= 1;
        //GPIO_SetBits(DS1302_SCLK_GPIO_PORT, DS1302_SCLK_GPIO_PIN);
			DS1302_SCLK_H;
			DS1302_DELAY;
        if (DS1302_io_read)
        {
            data |= 0x80;
        }
				DS1302_DELAY;
        //GPIO_ResetBits(DS1302_SCLK_GPIO_PORT, DS1302_SCLK_GPIO_PIN);
				DS1302_SCLK_L;
				DS1302_DELAY;
    }
    return data;
}





//6. 从指定寄存器读取数据函数
//同样先操作复位线 RST 进入读取准备状态，向指定寄存器地址写入读取指令（地址加上读模式标志），接着读取数据，最后拉低复位线完成读取操作
uint8_t DS1302_Read(uint8_t regAddr)
{
    uint8_t data;
    //GPIO_ResetBits(DS1302_RST_GPIO_PORT, DS1302_RST_GPIO_PIN);
		DS1302_RST_L;
		DS1302_DELAY;
	
    //GPIO_SetBits(DS1302_RST_GPIO_PORT, DS1302_RST_GPIO_PIN);
	  DS1302_RST_H;
		DS1302_DELAY;
    DS1302_WriteByte(regAddr | DS1302_READ_MODE);
    data = DS1302_ReadByte();

    //GPIO_ResetBits(DS1302_RST_GPIO_PORT, DS1302_RST_GPIO_PIN);

		DS1302_RST_L;
		DS1302_DELAY;
    return data;
}



//7. 设置 DS1302 时间函数
//先解除写保护，然后依次将秒、分、时、日、月、年等时间数据写入对应的寄存器，最后再恢复写保护，防止误写入。
void DS1302_SetTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t month, uint8_t year)
{
    // 解除写保护
    DS1302_WriteReg(DS1302_WP_REG_ADDR, 0x00);

    // 写入时间数据到各寄存器
    DS1302_WriteReg(DS1302_SEC_REG_ADDR, second);
    DS1302_WriteReg(DS1302_MIN_REG_ADDR, minute);
    DS1302_WriteReg(DS1302_HOUR_REG_ADDR, hour);
    DS1302_WriteReg(DS1302_DATE_REG_ADDR, day);
    DS1302_WriteReg(DS1302_MONTH_REG_ADDR, month);
    DS1302_WriteReg(DS1302_YEAR_REG_ADDR, year);

    // 恢复写保护
    DS1302_WriteReg(DS1302_WP_REG_ADDR, 0x80);
}



//8. 获取 DS1302 时间函数
//从 DS1302 的秒、分、时、日、月、年等寄存器依次读取数据，存储到相应的变量中，以获取当前时间信息。
void DS1302_GetTime(uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *day, uint8_t *month, uint8_t *year)
{
    *second = DS1302_Read(DS1302_SEC_REG_ADDR);
    *minute = DS1302_Read(DS1302_MIN_REG_ADDR);
    *hour = DS1302_Read(DS1302_HOUR_REG_ADDR);
    *day = DS1302_Read(DS1302_DATE_REG_ADDR);
    *month = DS1302_Read(DS1302_MONTH_REG_ADDR);
    *year = DS1302_Read(DS1302_YEAR_REG_ADDR);
}


#endif





















/*********************************************END OF FILE**********************/

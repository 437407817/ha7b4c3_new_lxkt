#ifndef __EEPROMFUN_H
#define	__EEPROMFUN_H


#include "stm32_FH_xxx.h"
#include "./i2c/hard_i2c/bsp_i2c_ee.h"
#include "./i2c/soft_i2c/bsp_soft_i2c_ee.h"
#include "./global/GV_variable.h" 
#include "./global/GV_enum.h" 


//  #include "./system_config.h"
//#include "./defines.h"
//	#if(USE_EEPROM_soft0_or_Hard1==1)
//	I2C_HardEE_Init();
//	
//	#else
//	i2c_Soft_CfgGpio();
//	
//	#endif
//#define USE_EEPROM_soft0_or_Hard1  0  //切换软件eeprom还是硬件eeprom

#define  EEP_PageAddress_01      0x0010
#define  EEP_PageAddress_02      0x0200
#define  EEP_PageAddress_03      0x0220
#define  EEP_PageAddress_04      0x0240



//#if (USE_EEPROM_soft0_or_Hard1==1)
//#define EEPROM_WriteBuffer( pBuffer,  WriteAddr,  NumByteToWrite) sEE_WriteBuffer( pBuffer,  WriteAddr,  NumByteToWrite)
//#define EEPROM_ReadBuffer( pBuffer,  ReadAddr,  NumByteToRead) sEE_ReadBuffer( pBuffer,  ReadAddr,  (uint16_t *)(&NumByteToRead))
//#else
//#define EEPROM_WriteBuffer( pBuffer,  WriteAddr,  NumByteToWrite) EE_soft_WriteBytes(pBuffer,  WriteAddr,  NumByteToWrite)
//#define EEPROM_ReadBuffer( pBuffer,  ReadAddr,  NumByteToRead) EE_soft_ReadBytes(pBuffer, ReadAddr, NumByteToRead)

//#endif

uint8_t SaveBoardSettingEEpromData(void);

void GetBoardSettingEEpromData(void);

uint8_t SaveSYS_SettingEEpromData(void);

void GetSYS_SettingEEpromData(void);

uint8_t SavePowerDownData(void);
void GetPowerDownData(void);





uint8_t GetChipEncodeEEpromData(void);
uint8_t SaveChipEncodeEEpromData(void);

#endif /* __EEPROMFUN_H */


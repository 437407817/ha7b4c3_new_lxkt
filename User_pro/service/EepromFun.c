#include "./service/EepromFun.h"

#include "./sys/bsp_systime.h"   


//#include "./GV_variable.h"

//#include "./io/OUT_io/bsp_gpio_out.h" 

//#include "./i2c/hard_i2c/bsp_iic.h"
//#include "./i2c/hard_i2c/bsp_i2c_ee.h"
//#include "./test/bsp_test_iic.h" 
#include "./i2c/factory_iic.h"


extern str_EEpromSettingState TMP_EEpromSettingState;
extern str_EEprom_SYS_SettingDataState TMP_EEprom_SYS_SettingState;
extern str_ChipEncode GV_EEpromChipEncode;
extern str_PowerDownState GV_PowerDownState;

//#define  EEP_PageAddress_01      0x0000
//#define  EEP_PageAddress_02      0x0200
//#define  EEP_PageAddress_03      0x0220
//#define  EEP_PageAddress_04      0x0240

uint8_t* tmpPtr;
/**
  * @brief  保存eeprom数据
  * @param  SYSState_Item_t获取版号 BatterySetTargetStateItem_t获取保存信息
  * @retval 无
  */
uint8_t SaveBoardSettingEEpromData(void){

		 tmpPtr= (uint8_t*)&TMP_EEpromSettingState.EEP_Bat_SingleBattRun;
	return	SaveEEpromDataFun(&tmpPtr ,sizeof(TMP_EEpromSettingState) ,  EEP_PageAddress_01);
	
	

}



/**
  * @brief  读取eeprom数据
  * @param  SYSState_Item_t获取版号 BatterySetTargetStateItem_t获取保存信息
  * @retval 无
  */
void GetBoardSettingEEpromData(void){

	

				tmpPtr = (uint8_t*)&TMP_EEpromSettingState.EEP_Bat_SingleBattRun;
		ReadEEpromDataFun(&tmpPtr ,sizeof(TMP_EEpromSettingState) ,  EEP_PageAddress_01);
	



	
	

}





/**
  * @brief  保存eeprom数据,timedate
  * @param  SYSState_Item_t获取版号 BatterySetTargetStateItem_t获取保存信息
  * @retval 无
  */
uint8_t SaveSYS_SettingEEpromData(void){

			 tmpPtr= (uint8_t*)&TMP_EEprom_SYS_SettingState.EEP_DS_save_hour;
	return	SaveEEpromDataFun(&tmpPtr ,EEPROM_SYS_SettingSaveDataCount ,  EEP_PageAddress_02);
	
	

}



void GetSYS_SettingEEpromData(void){

	
				tmpPtr = (uint8_t*)&TMP_EEprom_SYS_SettingState.EEP_DS_save_hour;
		ReadEEpromDataFun(&tmpPtr ,EEPROM_SYS_SettingSaveDataCount ,  EEP_PageAddress_02);
	


	
	

}



uint8_t SavePowerDownData(void){

				 tmpPtr= (uint8_t*)&GV_PowerDownState.PowerDown_State;
	return	SaveEEpromDataFun(&tmpPtr ,10 ,  EEP_PageAddress_03);
	
	

}

void GetPowerDownData(void){

	
					tmpPtr = (uint8_t*)&GV_PowerDownState.PowerDown_State;
		ReadEEpromDataFun(&tmpPtr ,10 ,  EEP_PageAddress_03);
	
	



}













#include <string.h>




extern str_ChipEncode GV_EEpromChipEncode;
extern str_ChipEncode GV_SaveEEpChipEncode;



uint8_t SaveChipEncodeEEpromData(void){

uint16_t size_ee = 16;
	uint16_t size_ee02 = size_ee;
uint8_t  I2c_Buf_Write[size_ee];
uint8_t 	I2c_Buf_Read[size_ee];
	tmpPtr = (uint8_t*)&GV_SaveEEpChipEncode.code;
//memcpy(&I2c_Buf_Write,&GV_SaveEEpChipEncode.code,size_ee);	
	memcpy(&I2c_Buf_Write,tmpPtr,size_ee);	
SaveEEpromDataFun( &tmpPtr, EEP_PageAddress_03, size_ee);

Delay(100);
	
	size_ee02 = size_ee;
//EEPROM_ReadBuffer(I2c_Buf_Read, EEP_PageAddress_03, size_ee02);	
	uint8_t *pBuf = I2c_Buf_Read;
ReadEEpromDataFun(&pBuf ,EEP_PageAddress_03 , size_ee02);
//					tmpPtr = (uint8_t*)&TMP_EEprom_SYS_SettingState.EEP_DS_save_hour;
//		ReadEEpromDataFun(I2c_Buf_Read ,EEP_PageAddress_03 ,  size_ee02);
//sEE_ReadBuffer(I2c_Buf_Read, EEP_PageAddress_01, (uint16_t *)(&size_ee02));
	
	SYSTEM_DEBUG_ARRAY_MESSAGE(I2c_Buf_Read,size_ee,"  I2c_Buf_Read  =????? ");
	for (uint8_t i=0; i<size_ee; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			
			SYSTEM_ERROR("\n SaveChipEncodeEEpromData error\n");
		return 1;
		}
   
	}

return 0;

}



uint8_t GetChipEncodeEEpromData(void){

uint16_t size_ee = 16;

uint8_t 	I2c_Buf_Read[size_ee];
	
	
uint8_t *pBuf = I2c_Buf_Read;
	
ReadEEpromDataFun(&pBuf ,EEP_PageAddress_03 , size_ee);
	
//EEPROM_ReadBuffer(I2c_Buf_Read, EEP_PageAddress_03, size_ee);	

memcpy(&GV_EEpromChipEncode.code,&I2c_Buf_Read,size_ee);	

	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)GV_EEpromChipEncode.code,size_ee,"  GetChipEncodeEEpromData=== ");

return 0;


}


















/*********************************************END OF FILE**********************/

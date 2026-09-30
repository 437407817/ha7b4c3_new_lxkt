#include "./screen/ScreenDS1302.h"

#include "./sys/bsp_systime.h"   

#include "./global/GV_variable.h" 
#include "./screen/DW_com_send.h"
#include "./screen/ScreenDataDis.h"
#include "./io/bsp_io_output.h" 

#include "./DataConvert/data_convert.h"
#include <stdlib.h>
//#include "./usart/rx_data_queue.h"

 #include "./sys/sysio.h"
#include "./screen/Screen.h"
#include "./controller/StateFun.h"
#include "./service/EepromFun.h"

#include "./screen/ScreenSendFun.h"
#include "./controller/DisStateFun.h"
#include "./service/EepromFun.h"
#include "./usart/p_data_queue.h"

#include "./service/DisposeDataFun.h"
#include "./controller/TaskFun.h"
#include "./controller/TaskSingleWork.h"
#include "./usart/bsp_usart_sd.h"

#include "./sensor/DS1302.h"

extern str_Screen_SYS_SettingDataState Screen_SYS_SettingDataState;
extern str_Screen_SYS_SettingDataState TEMP_Screen_SYS_SettingDataState;
extern str_Screen_SYS_SHOW_SettingDataState Screen_SYS_SHOW_SettingDataState;

//extern str_Screen_SYS_SHOW_SettingDataState Screen_SYS_SHOW_SettingDataState;

str_DS1302_data DS1302_data;


str_Screen_DS_SettingDataState  Screen_DS_SettingDataState;

DS_time_ITEM_t  DS_time_ITEM;

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern ScreenCommu_Item_t  GV_ScreenCommu;
//MaintainState_Serial(void)

extern str_EEprom_SYS_SettingDataState TMP_EEprom_SYS_SettingState;

extern  uint16_t Array_Page_SaveSucceed[2];
extern  uint16_t Array_Page_SaveFailure[2];
extern   uint16_t Array_Page_Main[2];
extern   uint16_t Array_Page_Main_English[2];

void PageSysTimeSettingSave(uint8_t num,uint8_t** p_data){



VpChange16HL((uint16_t*)*p_data,num);
//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)*p_data,num,"PageSysTimeSettingSave ***************");

//Delay(10);

memcpy(&Screen_DS_SettingDataState.src_DS_year,(*p_data)+0,num);
	

//_485_B_RX_EN();	sd_usart_cmd_ReadData();Delay(80);sd_usart_cmd_ReadData();Delay(80);sd_usart_cmd_ReadData();Delay(80);sd_usart_cmd_ReadData();Delay(70);	
//	sd_usart_cmd_ReadData();delay_ms(80);
	

	DS1302_data.sys_DS_year=Screen_DS_SettingDataState.src_DS_year;
	DS1302_data.sys_DS_month=Screen_DS_SettingDataState.src_DS_month;
	DS1302_data.sys_DS_day=Screen_DS_SettingDataState.src_DS_day;
	DS1302_data.sys_DS_hour=Screen_DS_SettingDataState.src_DS_hour;
	DS1302_data.sys_DS_minute=Screen_DS_SettingDataState.src_DS_minute;
	DS1302_data.sys_DS_second=Screen_DS_SettingDataState.src_DS_second;	
	
  DS1302_SetTime((uint8_t *)&DS1302_data);
	
	
	memcpy(&TMP_EEprom_SYS_SettingState.EEP_DS_save_hour,(*p_data)+12,num-12);//保存sd保存间隔
	//SuspendTaskHandler(&Handle_SendSDTimeDataToScreen);//----------------
	
	
	
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)&TMP_EEprom_SYS_SettingState,num-12,"eeprom_save_intervaltime  ***************");
	
	//pull_data_from_SD_485();pull_data_from_SD_485();//----------
	
	//memcpy(&I2c_Buf_Write,&TMP_EEprom_SYS_SettingState.EEP_sd_save_hour,EEPROM_SYS_SettingSaveDataCount);
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)&TMP_EEprom_SYS_SettingState.EEP_DS_save_hour,EEPROM_SYS_SettingSaveDataCount,"KKKKKK ==***************");
	
//	_485_B_RX_EN();
//sd_usart_cmd_ReadData();delay_ms(70);	
_485_B_RX_EN();delay_ms(80);
sd_usart_cmd_ReadData();delay_ms(100);	sd_usart_cmd_ReadData();delay_ms(100);	sd_usart_cmd_ReadData();delay_ms(100);	sd_usart_cmd_ReadData();delay_ms(100);	

	sd_usart_cmd_SetData(formatDateTimeToString(0,0,0),"",0,NULL,0XFF);
//	_485_B_RX_EN();
	delay_ms(100);			
	
		sd_usart_cmd_SetTime(DS1302_data.sys_DS_year,DS1302_data.sys_DS_month,DS1302_data.sys_DS_day,
	DS1302_data.sys_DS_hour,DS1302_data.sys_DS_minute,DS1302_data.sys_DS_second);//+++++++++++++++++++++xuyaoxie
	delay_ms(100);
			sd_usart_cmd_SetTime(DS1302_data.sys_DS_year,DS1302_data.sys_DS_month,DS1302_data.sys_DS_day,
	DS1302_data.sys_DS_hour,DS1302_data.sys_DS_minute,DS1302_data.sys_DS_second);//+++++++++++++++++++++xuyaoxie
	delay_ms(100);
			sd_usart_cmd_SetTime(DS1302_data.sys_DS_year,DS1302_data.sys_DS_month,DS1302_data.sys_DS_day,
	DS1302_data.sys_DS_hour,DS1302_data.sys_DS_minute,DS1302_data.sys_DS_second);//+++++++++++++++++++++xuyaoxie
//	
	delay_ms(100);
	
	
	  if(SaveSYS_SettingEEpromData()==0){
	 
		
		
		
		
		 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_SaveSucceed;

		GV_ScreenCommu.ScreenDataSize=2;
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面12
				 delay_ms(1000);
		
					if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){
			
			GV_ScreenCommu.ScreenData =Array_Page_Main;
			}else{
			
			GV_ScreenCommu.ScreenData =Array_Page_Main_English;
			}
		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
		 return;
	 }else{
	 
	 		 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_SaveFailure;

		GV_ScreenCommu.ScreenDataSize=2;
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面13
				 delay_ms(1000);
		
					if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){
			
			GV_ScreenCommu.ScreenData =Array_Page_Main;
			}else{
			
			GV_ScreenCommu.ScreenData =Array_Page_Main_English;
			}
		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
	  return;
	 }
	

	
	return ;

	 
	 
}













/**
* @brief  发送系统设置数据（点开设置按钮只发一次）
  * @param  
  * @retval 无
  */

void ScreenSYS_SettingParaSend(void){
//sd_usart_cmd_ReadTime();
	
	DS1302_getTime_Fun();
	
	
	GetSYS_SettingEEpromData();
	//GV_ScreenSetRunBoardState.Set_AllUpperLimitParallelVol
	
	
	Screen_DS_SettingDataState.sys_DS_tmp_flag=1;
	
	memcpy(&Screen_DS_SettingDataState.src_sd_save_refresh_hour,&TMP_EEprom_SYS_SettingState.EEP_DS_save_hour,EEPROM_SYS_SettingSaveDataCount);//eep的保存时间取出来显示
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)&Screen_DS_SettingDataState.src_sd_save_refresh_hour,EEPROM_SYS_SettingSaveDataCount,"KKKAAA ==***************");
	

			
		memcpy(&Screen_DS_SettingDataState.src_DS_year,&DS_time_ITEM.DS_year,12);
		SCREEN_write_cmd(EM_SYS_SET_YEAR,&Screen_DS_SettingDataState.src_DS_year,18/2);
		
	void* frame=&Screen_SYS_SettingDataState.sys_sd_year;
	
	
		SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,19," setting time =");
	
   frame=NULL;

	
	
	//ResumeTaskHandler(&Handle_SendSDTimeDataToScreen);

}






void DS1302_getTime_Fun(void){

	DS1302_Readtime((uint8_t *)&DS_time_ITEM.DS_year,(uint8_t *)&DS_time_ITEM.DS_month,(uint8_t *)&DS_time_ITEM.DS_day,
	(uint8_t *)&DS_time_ITEM.DS_hour,(uint8_t *)&DS_time_ITEM.DS_minute,(uint8_t *)&DS_time_ITEM.DS_second);
	
	
	
		memcpy(&Screen_SYS_SHOW_SettingDataState.sys_NOW_year,&DS_time_ITEM.DS_year,12);
		SCREEN_write_cmd(EM_SYS_SHOW_YEAR,&Screen_SYS_SHOW_SettingDataState.sys_NOW_year,12/2);

}



















/*********************************************END OF FILE**********************/

#include "./service/DisposeDataFun.h"



#include "./screen/ScreenDataDis.h"

#include "./global/GV_variable.h" 
 #include "./sys/sysio.h"
#include "./screen/DW_com_send.h"
#include <string.h>

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern str_HardwareState GV_HardwareState; 
extern str_EEpromSettingState TMP_EEpromSettingState;

extern ScreenCommu_Item_t  GV_ScreenCommu;
extern str_BatterySetCalibration GV_BatterySetCalibration;

extern str_BatterySetCalibration BigD_GV_BatterySetCalibration;

extern str_ScreenSettingDataState GV_ScreenSettingDataState;



extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;

extern str_ScreenAllCalibration GV_ScreenAllCalibration;


void CopyEEpromDatatoSettingData(void){

memcpy(&GV_ScreenSettingDataState.Set_Bat_SingleBattRun,&TMP_EEpromSettingState.EEP_Bat_SingleBattRun,EEPROM_SettingSaveDataCount);
	
memcpy(&GV_ScreenSetRunBoardState.Set_ChargeStateSelect,&GV_ScreenSettingDataState.Set_Bat_Runworkingmode,16);
	DW_SCREEN_write_cmd_print(EM_Icon_Mode_Select_1,GV_ScreenSetRunBoardState.Set_ChargeStateSelect,8,0);//修改屏幕的模式图标
}







//void DisposeSaveSettingData(void){

////memcpy(&GV_ScreenSetRunBoardState.Set_AllUpperLimitParallelVol,*p_data,num);

//	
//	for(uint16_t i=0;i<=12;i++){
//	GV_ScreenSettingDataState.Set_Bat_UpperLimitParallelVol[i]=GV_ScreenSettingDataState.Set_AllUpperLimitParallelVol;
//	GV_ScreenSettingDataState.Set_Bat_UpperAlarmParallelVol[i]=GV_ScreenSettingDataState.Set_AllUpperAlarmParallelVol;		
//	GV_ScreenSettingDataState.Set_Bat_LowerLimitParallelVol[i]=GV_ScreenSettingDataState.Set_AllLowerLimitParallelVol;		
//	GV_ScreenSettingDataState.Set_Bat_LowerAlarmParallelVol[i]=GV_ScreenSettingDataState.Set_AllLowerAlarmParallelVol;		
//		
//	GV_ScreenSettingDataState.Set_Bat_BalanceVol_xmV[i]=GV_ScreenSettingDataState.Set_ALLBalanceVol_xmV;
//	GV_ScreenSettingDataState.Set_Bat_BalanceCurrent_xmA[i]=GV_ScreenSettingDataState.Set_ALLBalanceCurrent_xmA;		
//	GV_ScreenSettingDataState.Set_Bat_ChargeCurrent_xmA[i]=GV_ScreenSettingDataState.Set_ALLChargeCurrent_xmA;		
//	GV_ScreenSettingDataState.Set_Bat_DischargeCurrent_xmA[i]=GV_ScreenSettingDataState.Set_ALLDischargeCurrent_xmA;			
//	}
//	

//};



//uint8_t Addr;//0XFE=广播地址
//uint8_t BoardNum;//从控板数量
//uint8_t IsSavePara;//0=不保存；1=保存
//uint8_t Run;//0=停止；1=并充；2=并放；3=维护；4=串充；5=串放；
//uint8_t BattNum[12];//共12组(每个板子电池个数)
//uint16_t StopParalleltChargeVol[12];//共12组并充截止电压 mV
//uint16_t AlarmParalleltChargeVol[12];//共12组并充报警上限电压 mV
//uint16_t StopParalleltDischargeVol[12];//共12组并放截止电压 mV
//uint16_t AlarmParalleltDischargeVol[12];//共12组并放报警上限电压 mV
//uint16_t BalanceVol[12];//共12组均衡电压 mv
//uint16_t BalanceCur[12];//共12组均衡电流 mA
//uint16_t ChargeCurrent[12];//并充电流 mA
//uint16_t DischargeCurrent[12];//并放电流 mA
//#define _Set_BoardBat_0n(i)  Set_BoardBat##i
//#define Set_BoardBat_0n(i)  _Set_BoardBat_0n(i)

void CopySettingDataToSlaveBuff(uint8_t itv){


memcpy(GV_CommSendToSlaveBoardState.SingleBattRun,GV_ScreenSettingDataState.Set_Bat_SingleBattRun,8*2);	
	for(uint16_t i=0;i<BOARD_COUNT;i++)
{
	//GV_HardwareState.HW_NeedWorkingState[i]=GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i];
	
	
//GV_CommSendToSlaveBoardState.SingleBattRun[i]=GV_ScreenSettingDataState.Set_Bat_SingleBattRun[i]; 		
GV_CommSendToSlaveBoardState.Vol_difference[i]=(uint8_t)GV_ScreenSettingDataState.Set_Bat_Run_Vol_difference[i]; 	
GV_CommSendToSlaveBoardState.RunCount_polar[i]=(uint8_t)GV_ScreenSettingDataState.Set_Bat_RunCount_polar[i]; 	
//GV_CommSendToSlaveBoardState.Run[i]=(uint8_t)GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i]; 
GV_CommSendToSlaveBoardState.S_Run[i]=itv; 	
GV_CommSendToSlaveBoardState.BattNum[i]=(uint8_t)GV_ScreenSettingDataState.Set_Bat_BattNum[i]; 

}	


memcpy(GV_CommSendToSlaveBoardState.StopParalleltChargeVol,GV_ScreenSettingDataState.Set_Bat_UpperLimitParallelVol,BOARD_COUNT*10);

//memcpy(GV_CommSendToSlaveBoardState.BalanceCur,GV_ScreenSettingDataState.Set_Bat_BalanceCurrent_xmA,BOARD_COUNT*6);
for(uint16_t ii=0;ii<BOARD_COUNT;ii++){
	

GV_CommSendToSlaveBoardState.ChargeCurrent[ii]=GV_ScreenSettingDataState.Set_Bat_ChargeCurrent_xmA[ii]*100;

GV_CommSendToSlaveBoardState.StopCapacity[ii]=GV_ScreenSettingDataState.Set_Bat_StopCapacity[ii];
GV_CommSendToSlaveBoardState.StopTime[ii]=GV_ScreenSettingDataState.Set_Bat_StopTime[ii];
	
}

SYSTEM_DEBUG_ARRAY_MESSAGE(&GV_CommSendToSlaveBoardState.CommS_Addr,sizeof(GV_CommSendToSlaveBoardState),"  CopySettingDataToSlaveBuff==  ");

}


void calculateVolDataFun(void){

//SumParallel_vol_Value
uint16_t Max_vol_Value_tmp=0,Min_vol_Value_tmp=0xffff;
	uint32_t SumPvoltemp=0;
	
	for(uint8_t ics=0;ics<BOARD_COUNT_CHANGE;ics++){
		Max_vol_Value_tmp=0;
		Min_vol_Value_tmp=0xffff;
		SumPvoltemp=0;
		for(uint8_t ocs=0;ocs<GV_CommSendToSlaveBoardState.BattNum[ics];ocs++){
			
		if(GV_HardwareState.batterystate[ics].bat_vol[ocs]>Max_vol_Value_tmp){
		Max_vol_Value_tmp=GV_HardwareState.batterystate[ics].bat_vol[ocs];
		}
		
		if(GV_HardwareState.batterystate[ics].bat_vol[ocs]<Min_vol_Value_tmp){
		Min_vol_Value_tmp=GV_HardwareState.batterystate[ics].bat_vol[ocs];
		}
		

	SumPvoltemp+=GV_HardwareState.batterystate[ics].bat_vol[ocs];
			
		}
		
		

		
		
		
		GV_HardwareState.batterystate[ics].SumParallel_vol_Value=SumPvoltemp/10;
		
#if 1
//		if()
		GV_HardwareState.batterystate[ics].Max_vol_Value=Max_vol_Value_tmp;
		GV_HardwareState.batterystate[ics].Min_vol_Value=Min_vol_Value_tmp;
		if(GV_CommSendToSlaveBoardState.BattNum[ics]==0){
		GV_HardwareState.batterystate[ics].Min_vol_Value=0;
		}
		
		GV_HardwareState.batterystate[ics].MaxDiff_vol_Value=GV_HardwareState.batterystate[ics].Max_vol_Value-GV_HardwareState.batterystate[ics].Min_vol_Value;
		

		
		
		//GV_HardwareState.batterystate[ics].Average_vol_Value=GV_HardwareState.batterystate[ics].MaxDiff_vol_Value/GV_CommSendToSlaveBoardState.BattNum[ics];		
#endif		
			
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
}





void SlaveCalibrationcalculate(void){

//GV_ScreenAllCalibration
GV_BatterySetCalibration.cal_Addr=GV_ScreenAllCalibration.Rectify_pagenum+1;
//*************************************************SetCCurrent
memcpy(&GV_BatterySetCalibration.BX_DisplaySampleChargeCurrent[0],&GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCur[0],24);
	
	
//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_BatterySetCalibration.BX_SetCCurrent,24,"*****cde **");	
	
//	for(uint8_t i=0;i<12;i++){

//if(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCurEnable>>i)&1)==1){

//	//SYSTEM_DEBUG("*****xx*==,%04X*",GV_BatterySetCalibration.BX_SetCCurrent[0]);
//	
//GV_BatterySetCalibration.BX_SetCCurrent[i]=GV_BatterySetCalibration.BX_SetCCurrent[i]|0x8000;
//	//SYSTEM_DEBUG("*****xxx*==,%04X*",GV_BatterySetCalibration.BX_SetCCurrent[0]);
//}else{

//GV_BatterySetCalibration.BX_SetCCurrent[i]=GV_BatterySetCalibration.BX_SetCCurrent[i]&0x7fff;

//}	





//if(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCurReset>>i)&1)==1){

//GV_BatterySetCalibration.BX_SetCCurrent[i]=GV_BatterySetCalibration.BX_SetCCurrent[i]|0x4000;

//}else{

//GV_BatterySetCalibration.BX_SetCCurrent[i]=GV_BatterySetCalibration.BX_SetCCurrent[i]&0xbfff;

//}	


//}
	



//SYSTEM_DEBUG("*****c*==%d,,,%02X,%02X*=,%02X*,%02X*,%d",
//GV_ScreenAllCalibration.Rectify_pagenum,
//GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCurEnable,
//GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCurReset,

//(GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCurEnable>>0),
//((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCurEnable>>0)&1),
//(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCurEnable>>0)&1)==1));	
//	

//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_BatterySetCalibration.BX_SetCCurrent,24,"*****cde1 **");

//*************************************************SetDCurrent


memcpy(&GV_BatterySetCalibration.BX_TrueSampleChargeCurrent[0],&GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_TrueSampleChargeCurrent[0],24);
	
	for(uint8_t i=0;i<12;i++){

if(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_TrueSampleChargeCurrentEnable>>i)&1)==1){

GV_BatterySetCalibration.BX_TrueSampleChargeCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleChargeCurrent[i]|0x8000;

}else{

GV_BatterySetCalibration.BX_TrueSampleChargeCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleChargeCurrent[i]&0x7fff;

}	


if(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_TrueSampleChargeCurrentReset>>i)&1)==1){

GV_BatterySetCalibration.BX_TrueSampleChargeCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleChargeCurrent[i]|0x4000;

}else{

GV_BatterySetCalibration.BX_TrueSampleChargeCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleChargeCurrent[i]&0xbfff;

}	


}
	


//*************************************************TrueSampleCCurrent 


memcpy(&GV_BatterySetCalibration.BX_DisplaySampleDchargeCurrent[0],&GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_DisplaySampleDchargeCurrent[0],24);
	
//	for(uint8_t i=0;i<12;i++){

//if(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_MeasureChargeCurEnable>>i)&1)==1){

//GV_BatterySetCalibration.BX_TrueSampleCCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleCCurrent[i]|0x8000;

//}else{

//GV_BatterySetCalibration.BX_TrueSampleCCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleCCurrent[i]&0x7fff;

//}	


//if(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_MeasureChargeCurReset>>i)&1)==1){

//GV_BatterySetCalibration.BX_TrueSampleCCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleCCurrent[i]|0x4000;

//}else{

//GV_BatterySetCalibration.BX_TrueSampleCCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleCCurrent[i]&0xbfff;

//}	


//}
	


//*************************************************TrueSampleDCurrent  


memcpy(&GV_BatterySetCalibration.BX_TrueSampleDchargeCurrent[0],&GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_TrueSampleDchargeCurrent[0],24);
	
	for(uint8_t i=0;i<12;i++){

if(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_TrueSampleDchargeCurrentEnable>>i)&1)==1){

GV_BatterySetCalibration.BX_TrueSampleDchargeCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleDchargeCurrent[i]|0x8000;

}else{

GV_BatterySetCalibration.BX_TrueSampleDchargeCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleDchargeCurrent[i]&0x7fff;

}	


if(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_TrueSampleDchargeCurrentReset>>i)&1)==1){

GV_BatterySetCalibration.BX_TrueSampleDchargeCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleDchargeCurrent[i]|0x4000;

}else{

GV_BatterySetCalibration.BX_TrueSampleDchargeCurrent[i]=GV_BatterySetCalibration.BX_TrueSampleDchargeCurrent[i]&0xbfff;

}	


}
	


//*************************************************TrueSampleDCurrent  


memcpy(&GV_BatterySetCalibration.BX_TrueVoltage[0],&GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_TrueVoltage[0],24);
	
	for(uint8_t i=0;i<12;i++){

if(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_TrueVoltageEnable>>i)&1)==1){

GV_BatterySetCalibration.BX_TrueVoltage[i]=GV_BatterySetCalibration.BX_TrueVoltage[i]|0x8000;

}else{

GV_BatterySetCalibration.BX_TrueVoltage[i]=GV_BatterySetCalibration.BX_TrueVoltage[i]&0x7fff;

}	


if(((GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_TrueVoltageReset>>i)&1)==1){

GV_BatterySetCalibration.BX_TrueVoltage[i]=GV_BatterySetCalibration.BX_TrueVoltage[i]|0x4000;

}else{

GV_BatterySetCalibration.BX_TrueVoltage[i]=GV_BatterySetCalibration.BX_TrueVoltage[i]&0xbfff;

}	


}
	

SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)&GV_BatterySetCalibration,sizeof(GV_BatterySetCalibration),"GV_BatterySetCalibration ***************");





//memcpy(&BigD_GV_BatterySetCalibration.cal_Addr,&GV_BatterySetCalibration.cal_Addr,sizeof(GV_BatterySetCalibration));













}














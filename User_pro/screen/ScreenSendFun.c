#include "./screen/ScreenSendFun.h"

#include "./sys/bsp_systime.h"   

#include "./global/GV_variable.h" 


#include "./screen/DW_com_send.h"
#include "./io/bsp_io_output.h" 

#include "./DataConvert/data_convert.h"
#include <stdlib.h>

 #include "./sys/sysio.h"
#include "string.h"
#include "./screen/ScreenDataDis.h"
//#include "./controller/DisStateFun.h"
//#include "./defines.h"

extern ScreenCommu_Item_t  GV_ScreenCommu;
extern str_HardwareState GV_HardwareState; 
extern str_BatterySetCalibration GV_BatterySetCalibration;
extern str_ComuBoardState GV_ComuBoardState;

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;

uint16_t Array_ErrorType_Nomal[4]={0xCEDE,0xB9CA,0xD5CF,0x2020};  //无故障
uint16_t Array_ErrorType_Nomal_English[ETE_Size];  //ACR_BYTE0_ADDRESS

char* Array_ErrorType_Nomal_English_string ="nofault";





uint16_t Array_ErrorType_OverCharge[4]={0xB9FD,0xD1B9,0x2020,0x2020};//并充过压//过压
uint16_t Array_ErrorType_OverCharge_English[ETE_Size];
char*  Array_ErrorType_OverCharge_English_string="P-Charge over vol";




uint16_t Array_ErrorType_OverDisCharge[4]={0xC7B7,0xD1B9,0x2020,0x2020};//并放欠压//欠压
uint16_t Array_ErrorType_OverDisCharge_English[ETE_Size];//并放欠压//欠压
char*  Array_ErrorType_OverDisCharge_English_string="P-DisCharge low vol";




uint16_t Array_ErrorType_BreakLine[4]={0xB5E7,0xB3D8,0xB6CF,0xCFDF};
uint16_t Array_ErrorType_BreakLine_English[4];
char*  Array_ErrorType_BreakLine_English_string="BreakLine error";




uint16_t Array_ErrorType_OverCurrent[4]={0xB2A2,0xB3E4,0xB9FD,0xC1F7};     //并充过流
uint16_t Array_ErrorType_OverCurrent_English[ETE_Size];
char*  Array_ErrorType_OverCurrent_English_string="P-Charge ov cur";



uint16_t Array_ErrorType_OverDisCurrent[4]={0xB7C5,0xB5E7,0xB9FD,0xC1F7};//并放过流//放电过流
uint16_t Array_ErrorType_OverDisCurrent_English[ETE_Size];
char*  Array_ErrorType_OverDisCurrent_English_string="P-DisCharge ov cur";




uint16_t Array_ErrorType_OverSerialChargeCurrent[4]={0xB4AE,0xB3E4,0xB9FD,0xC1F7};
uint16_t Array_ErrorType_OverSerialChargeCurrent_English[ETE_Size];
char*  Array_ErrorType_OverSerialChargeCurrent_English_string="S-Charge ov cur";





uint16_t Array_ErrorType_OverSerialChargeVol[4]={0xB4AE,0xB3E4,0xB9FD,0xD1B9};
uint16_t Array_ErrorType_OverSerialChargeVol_English[ETE_Size];
char*  Array_ErrorType_OverSerialChargeVol_English_string="S-Charge ov vol";






uint16_t Array_ErrorType_OverSerialDisCurrent[4]={0xB4AE,0xB7C5,0xB9FD,0xC1F7};
uint16_t Array_ErrorType_OverSerialDisCurrent_English[ETE_Size];
char*  Array_ErrorType_OverSerialDisCurrent_English_string="S-DisCharge ov cur";




uint16_t Array_ErrorType_OverSerialDisVol[4]={0xB4AE,0xB7C5,0xB9FD,0xD1B9};
uint16_t Array_ErrorType_OverSerialDisVol_English[ETE_Size];
char*  Array_ErrorType_OverSerialDisVol_English_string="S-DisCharge ov vol";





uint16_t Array_ErrorType_ReceiveInterval[4]={0xBDD3,0xCAD5,0xD6D0,0xB6CF};//接收中断BDD3,CAD5,D6D0,B6CF,
uint16_t Array_ErrorType_ReceiveInterval_English[ETE_Size];
char*  Array_ErrorType_ReceiveInterval_English_string="Receive break";


uint16_t Array_ErrorType_CloseError[4]={0xCDA8,0xD0C5,0xD6D0,0xB6CF};
uint16_t Array_ErrorType_CloseError_English[ETE_Size];
char*  Array_ErrorType_CloseError_English_string="Com break";


uint16_t Array_ErrorType_CurrentBreakdown[4]={0xB5E7,0xC1F7,0xB9CA,0xD5CF};
uint16_t Array_ErrorType_CurrentBreakdown_English[ETE_Size];
char*  Array_ErrorType_CurrentBreakdown_English_string="Cur break";
//EM_CurrentBreakdown  B5E7 C1F7 B9CA D5CF 


uint16_t Array_ErrorType_OverTime[4]={0xCDA8,0xD0C5,0xB3AC,0xCAB1};//通讯超时
uint16_t Array_ErrorType_OverTime_English[ETE_Size];
char*  Array_ErrorType_OverTime_English_string="Com Ov Time";



uint16_t Array_ErrorType_MachineBreakdown[4]={0xBBFA,0xC6F7,0xB9CA,0xD5CF};//机器故障
uint16_t Array_ErrorType_MachineBreakdown_English[ETE_Size];
char*  Array_ErrorType_MachineBreakdown_English_string="MachineBreak";




//uint16_t Array_WorkType_ParrelCharge[4]={0xB3E4,0xB5E7,0xD6D0,0x2020};//并充中//充电
//uint16_t Array_WorkType_ParrelDisCharge[4]={0xB7C5,0xB5E7,0xD6D0,0x2020};//并放中//放电
//uint16_t Array_WorkType_Maintain[4]={0xBEF9,0xBAE2,0xD6D0,0x2020};//维护中//均衡

uint16_t Array_WorkType_Run[4]={0xD4CB,0xD0D0,0xD6D0,0x2020};//运行中//
uint16_t Array_WorkType_Run_English[ETE_Size];//运行中//
char*  Array_WorkType_Run_English_string="Running";



uint16_t Array_WorkType_Finish[4]={0xB9A4,0xD7F7,0xCDEA,0xB3C9};//工作完成//
uint16_t Array_WorkType_Finish_English[ETE_Size];
char*  Array_WorkType_Finish_English_string="Finish";




uint16_t Array_WorkType_wrong[4]={0xB9CA,0xD5CF,0x2020,0x2020};//故障中//均衡
uint16_t Array_WorkType_wrong_English[ETE_Size];
char* Array_WorkType_wrong_English_string="fault";

uint16_t Array_WorkType_wrongstop01[4]={0xD2EC,0xB3A3,0xCDA3,0x3031};//异常停01  ????
uint16_t Array_WorkType_SerialCharge[4]={0xB4AE,0xB3E4,0xD6D0,0x2020};//串充中  ???
uint16_t Array_WorkType_SerialDisCharge[4]={0xB4AE,0xB7C5,0xD6D0,0x2020};//串放中  ???

uint16_t Array_WorkType_Stop[4]={0xB4FD,0xBBFA,0xD6D0,0x2020};//待机中
uint16_t Array_WorkType_Stop_English[ETE_Size];//待机中
char* Array_WorkType_Stop_English_string="stop";


uint16_t Array_WorkType_ParrelChargeFinish[4]={0xB3E4,0xB5E7,0xCDEA,0xB3C9};//并充完成//充电完成
uint16_t Array_WorkType_ParrelChargeFinish_English[ETE_Size];
//char* Array_WorkType_ParrelChargeFinish_English_string="stop";

uint16_t Array_WorkType_ParrelDisChargeFinish[4]={0xB7C5,0xB5E7,0xCDEA,0xB3C9};//并放完成//放电完成????
uint16_t Array_WorkType_ParrelDisChargeFinish_English[ETE_Size];

uint16_t Array_WorkType_MaintainFinish[4]={0xBEF9,0xBAE2,0xCDEA,0xB3C9};//维护完成//均衡完成????
uint16_t Array_WorkType_MaintainFinish_English[ETE_Size];

uint16_t Array_WorkType_NowMessage[4];
uint16_t Array_WorkType_NowMessage_English[ETE_Size];

//uint16_t Array_WorkType_Pause[4]={0xD4DD,0xCDA3,0xD6D0,0x2020};//暂停中//D4DD CDA3 D6D0 
//uint16_t Array_WorkType_Pause_English[ETE_Size];
char* Array_WorkType_Pause_English_string="Pause";

//char* Activate_State_unactive_string="未激活";
//char* Activate_State_active_string="已激活";

//char* Activate_State_English_string="fault";





void StingMessageConvert(void){

ConvertCNtoGBKhexReverse(Array_ErrorType_Nomal_English_string,strlen(Array_ErrorType_Nomal_English_string)+1,(char*)&Array_ErrorType_Nomal_English);
	
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)Array_ErrorType_Nomal_English_string,ETE_Size*2," ******-1---%d---",strlen(Array_ErrorType_Nomal_English_string)+1);
//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)Array_ErrorType_Nomal_English,ETE_Size*2," ******--1-----");
	
	
ConvertCNtoGBKhexReverse(Array_ErrorType_OverCharge_English_string,strlen(Array_ErrorType_OverCharge_English_string)+1,(char*)&Array_ErrorType_OverCharge_English);

ConvertCNtoGBKhexReverse(Array_ErrorType_OverDisCharge_English_string,strlen(Array_ErrorType_OverDisCharge_English_string)+1,(char*)&Array_ErrorType_OverDisCharge_English);
ConvertCNtoGBKhexReverse(Array_ErrorType_BreakLine_English_string,strlen(Array_ErrorType_BreakLine_English_string)+1,(char*)&Array_ErrorType_BreakLine_English);
ConvertCNtoGBKhexReverse(Array_ErrorType_OverCurrent_English_string,strlen(Array_ErrorType_OverCurrent_English_string)+1,(char*)&Array_ErrorType_OverCurrent_English);
ConvertCNtoGBKhexReverse(Array_ErrorType_OverDisCurrent_English_string,strlen(Array_ErrorType_OverDisCurrent_English_string)+1,(char*)&Array_ErrorType_OverDisCurrent_English);
					
	
	

	
ConvertCNtoGBKhexReverse(Array_ErrorType_OverSerialChargeVol_English_string,strlen(Array_ErrorType_OverSerialChargeVol_English_string)+1,(char*)&Array_ErrorType_OverSerialChargeVol_English);
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)Array_ErrorType_OverSerialChargeVol_English_string,ETE_Size*2," ******----4---");	

	
ConvertCNtoGBKhexReverse(Array_ErrorType_OverSerialChargeCurrent_English_string,strlen(Array_ErrorType_OverSerialChargeCurrent_English_string)+1,(char*)&Array_ErrorType_OverSerialChargeCurrent_English);
ConvertCNtoGBKhexReverse(Array_ErrorType_OverSerialDisCurrent_English_string,strlen(Array_ErrorType_OverSerialDisCurrent_English_string)+1,(char*)&Array_ErrorType_OverSerialDisCurrent_English);


//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)Array_ErrorType_OverSerialDisCurrent_English_string,ETE_Size*2," ******----5---");	
ConvertCNtoGBKhexReverse(Array_ErrorType_OverSerialDisVol_English_string,strlen(Array_ErrorType_OverSerialDisVol_English_string)+1,(char*)&Array_ErrorType_OverSerialDisVol_English);



ConvertCNtoGBKhexReverse(Array_ErrorType_ReceiveInterval_English_string,strlen(Array_ErrorType_ReceiveInterval_English_string)+1,(char*)&Array_ErrorType_ReceiveInterval_English);





ConvertCNtoGBKhexReverse(Array_ErrorType_CloseError_English_string,strlen(Array_ErrorType_CloseError_English_string)+1,(char*)&Array_ErrorType_CloseError_English);
	

	
ConvertCNtoGBKhexReverse(Array_ErrorType_OverTime_English_string,strlen(Array_ErrorType_OverTime_English_string)+1,(char*)&Array_ErrorType_OverTime_English);


	
	
	
ConvertCNtoGBKhexReverse(Array_ErrorType_MachineBreakdown_English_string,strlen(Array_ErrorType_MachineBreakdown_English_string)+1,(char*)&Array_ErrorType_MachineBreakdown_English);
ConvertCNtoGBKhexReverse(Array_WorkType_Run_English_string,strlen(Array_WorkType_Run_English_string)+1,(char*)&Array_WorkType_Run_English);
ConvertCNtoGBKhexReverse(Array_WorkType_Finish_English_string,strlen(Array_WorkType_Finish_English_string)+1,(char*)&Array_WorkType_Finish_English);

ConvertCNtoGBKhexReverse(Array_WorkType_wrong_English_string,strlen(Array_WorkType_wrong_English_string)+1,(char*)&Array_WorkType_wrong_English);

ConvertCNtoGBKhexReverse(Array_WorkType_Stop_English_string,strlen(Array_WorkType_Stop_English_string)+1,(char*)&Array_WorkType_Stop_English);






//ConvertCNtoGBKhexReverse(Array_WorkType_Pause_English_string,strlen(Array_WorkType_Pause_English_string)+1,(char*)&Array_WorkType_Pause_English);



//ConvertCNtoGBKhexReverse(Array_ErrorType_CloseError_English_string,strlen(Array_ErrorType_CloseError_English_string),(char*)&Array_ErrorType_CloseError_English);
								
	
	
	
	
	
//SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(2,(uint8_t*)Array_ErrorType_Nomal_English,strlen(Array_ErrorType_Nomal_English_string),"cn=");
}








//BBFA,0xC6F7,0xB9CA,0xD5CF


void StartWork_ClearBuffDataInit(void){

//	  memcpy(&GV_ComuBoardState.commu_SC_ErrorType,&Array_ErrorType_Nomal,4*2);//清除过去异常显示
	
	GV_ComuBoardState.commu_ErrorBatteryNo=0;
	//GV_ComuBoardState.commu_ErrorOvertimeBOARD=0;
	GV_ComuBoardState.commu_ErrorBoardNo=0;
	GV_ComuBoardState.commu_finishwork_FLAG=0;
	GV_ComuBoardState.commu_needfinishwork_FLAG=0;
	GV_ComuBoardState.commu_finishwork_Old_FLAG=0;	
	memset(&GV_ComuBoardState.screenBatCapComuState,0,sizeof(str_ScreenBatCapComuState)*BOARD_COUNT);//清除过去异常显示
	
	
	memcpy(&GV_ComuBoardState.commu_MachineStateMessage,&Array_WorkType_NowMessage,4*2);
	memcpy(&GV_ComuBoardState.commu_MachineStateMessage_English,&Array_WorkType_NowMessage_English,ETE_Size*2);

SCREEN_write_cmd(EM_ALL_ErrorBoardNo,&GV_ComuBoardState.commu_ErrorBoardNo,13);//总板故障
//SCREEN_write_cmd(EM_ALL_ErrorType_English,&GV_ComuBoardState.commu_ErrorBoardNo,10);	
	
		for(uint8_t i=0;i<BOARD_COUNT;i++){
memcpy(&GV_ComuBoardState.screenBatVolComuState[i].SC_ErrorType,&Array_ErrorType_Nomal,4*2);
memcpy(&GV_ComuBoardState.screenBatVolComuState[i].SC_ErrorType_English,&Array_ErrorType_Nomal_English,ETE_Size*2);
			GV_ComuBoardState.commu_overtime[i]=0;
GV_ComuBoardState.screenBatVolComuState[i].SC_ErrorBattNo=0;
		
			
		}
}


/**
  * @brief  复制从机传上来的数据到屏的buff中
  * @param  
  * @retval 无
  */
void CopyVolDataToScreenStruct(void){
//uint16_t boid=0;
	GV_ComuBoardState.check_ErrorType_Nomal_FLAG=0;
//GV_HardwareState.batterystate[ics].SumParallel_vol_Value=SumPvoltemp;
	for(uint16_t ici=0;ici<BOARD_COUNT_CHANGE;ici++){
	memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_Vol[0],&GV_ComuBoardState.getSlaveUpdloadState[ici].BS_Vol,12*2);//复制电压电流
		
		for(uint16_t icu=0;icu<12;icu++){
		
		GV_ComuBoardState.screenBatVolComuState[ici].SC_Cur[icu]=GV_ComuBoardState.getSlaveUpdloadState[ici].BS_Cur[icu]/100;
		}
		

	memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_Cap[0],&GV_ComuBoardState.getSlaveUpdloadState[ici].BS_Cap,12*2);//复制电压电流

		//if()
		
	
	//memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_Vol[0],&GV_ComuBoardState.getSlaveUpdloadState[ici].BS_Cur,12*4);	
		
		
	memcpy(&GV_HardwareState.batterystate[ici].bat_vol[0],&GV_ComuBoardState.getSlaveUpdloadState[ici].BS_Vol,12*2);//复制到信息处理的内存中
		
	memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_BattWrokingStatus,&GV_ComuBoardState.getSlaveUpdloadState[ici].BS_BattWrokingStatus,6);//复制电压电流工作状态位
	GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorBattNo=(uint16_t)GV_ComuBoardState.getSlaveUpdloadState[ici].BS_ErrorBattNo;
	//GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType=(uint16_t)GV_ComuBoardState.getSlaveUpdloadState[ici].BS_ErrorType;
		
//GV_ComuBoardState.screenBatVolComuState[ici].SC_RuningIntervalTime=GV_ComuBoardState.getSlaveUpdloadState[ici].RuningIntervalTime;
//GV_ComuBoardState.screenBatVolComuState[ici].SC_RuningCount=GV_ComuBoardState.getSlaveUpdloadState[ici].RuningCount;	
//		
GV_ComuBoardState.screenBatVolComuState[ici].SC_BitControl=GV_ComuBoardState.getSlaveUpdloadState[ici].BS_BitControl1;	
		
//		if(GV_ComuBoardState.getSlaveUpdloadState[ici].BS_WorkStatus==1||GV_ComuBoardState.getSlaveUpdloadState[ici].BS_WorkStatus==2||
//			GV_ComuBoardState.getSlaveUpdloadState[ici].BS_WorkStatus==3||GV_ComuBoardState.getSlaveUpdloadState[ici].BS_WorkStatus==4||GV_ComuBoardState.getSlaveUpdloadState[ici].BS_WorkStatus==5){
//		GV_HardwareState.HW_NowRealWorkingState=1;		
//		}
		
		
		
		

		
	
		}
	   disposeErrorType();
		
		
		if(GV_ComuBoardState.check_ErrorType_Nomal_FLAG >= BOARD_COUNT_CHANGE){
		GV_ComuBoardState.commu_ErrorBatteryNo=0;
		GV_ComuBoardState.commu_ErrorBoardNo=0;
		}
		
		
		//memcpy(&GV_ComuBoardState.commu_MachineStateMessage,&Array_ErrorType_MachineBreakdown,4*2);
		//SYSTEM_DEBUG(" SumParallel_vol_Value == 0x%d  %d",GV_ComuBoardState.screenBatVolComuState[0].SC_SUMvol,sizeof(str_ScreenBatVolComuState));
//		if(GV_ComuBoardState.commu_ErrorBatteryNo!=0){
		if(GV_ComuBoardState.commu_ErrorBoardNo!=0){

		//commu_MachineStateMessage  boid
			///GV_HardwareState.HW_NeedWorkingState=EM_HW_STOP;
			SYSTEM_DEBUG(" commu_ErrorBatteryNo == 0x%d  d",GV_ComuBoardState.commu_ErrorBatteryNo);
			
memcpy(&Array_WorkType_NowMessage,&Array_ErrorType_MachineBreakdown,4*2);
memcpy(&Array_WorkType_NowMessage_English,&Array_ErrorType_MachineBreakdown_English,ETE_Size*2);	
//if(GetWrongLed_STATE!=1){
//TurnWrong_led_ON;
//}			
			GetWrongLed_STATEAndled_ON;
//			GV_ComuBoardState.commu_ErrorBoardNo = boid;
		
		}else{
		if(GV_ComuBoardState.commu_overtime_FLAG==0){
//			GV_ComuBoardState.commu_ErrorBoardNo = 0;
		TurnWrong_led_OFF;

		}
		
		
		}
		
		
		
		
		
		//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)&GV_ComuBoardState.screenBatVolComuState[0].SC_Vol[0],12*4,"   SC_Vol ");
	
	//}
	
	
}









void disposeErrorType(void){

for(uint16_t ici=0;ici<BOARD_COUNT_CHANGE;ici++){

	if(GV_ComuBoardState.getSlaveUpdloadState[ici].BS_ErrorType!=0){
	
	SYSTEM_ERROR(" BS_ErrorType     %d  %d  ",ici,GV_ComuBoardState.getSlaveUpdloadState[ici].BS_ErrorType);
	}
	
	
		switch(GV_ComuBoardState.getSlaveUpdloadState[ici].BS_ErrorType){

	
	case EM_Nomal:   
				if(GV_ComuBoardState.commu_OT_scrShowErrorMess_flag[ici]==0){
			memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,&Array_ErrorType_Nomal,4*2);
			}
		
			GV_ComuBoardState.check_ErrorType_Nomal_FLAG++;
	GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[ici]=0;
GV_ComuBoardState.commu_F_ErrorBoardNo_EnterInFLAG[ici]=0;
			
			break;
	//case EM_Nomal:   memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,(char *)"无故障 ",4*2); break;		
	case EM_OverCharge:   
		memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,&Array_ErrorType_OverCharge,4*2);
	memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType_English,&Array_ErrorType_OverCharge_English,4*2);
//		memcpy(&GV_ComuBoardState.commu_SC_ErrorType,&Array_ErrorType_OverCharge,4*2);
	
	
	
		GV_ComuBoardState.commu_ErrorBatteryNo=GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorBattNo;//复制故障版号，和故障类型

//		*boid=ici+1;
		GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[ici]=1;
	GV_ComuBoardState.commu_ErrorBoardNo=ici+1;
			break;
	case EM_OverDisChargel:   
		memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,&Array_ErrorType_OverDisCharge,4*2);memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType_English,&Array_ErrorType_OverDisCharge_English,4*2);
//		memcpy(&GV_ComuBoardState.commu_SC_ErrorType,&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,4*2);
		GV_ComuBoardState.commu_ErrorBatteryNo=GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorBattNo;

//		*boid=ici+1;
	GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[ici]=1;
	GV_ComuBoardState.commu_ErrorBoardNo=ici+1;
			break;			
	case EM_BreakLine:  
		memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,&Array_ErrorType_BreakLine,4*2);memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType_English,&Array_ErrorType_BreakLine_English,4*2);
//memcpy(&GV_ComuBoardState.commu_SC_ErrorType,&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,4*2);
	GV_ComuBoardState.commu_ErrorBatteryNo=GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorBattNo;

//				*boid=ici+1;
	GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[ici]=1;
	GV_ComuBoardState.commu_ErrorBoardNo=ici+1;
			break;			
	case EM_OverCurrent:   
		memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,&Array_ErrorType_OverCurrent,4*2);memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType_English,&Array_ErrorType_OverCurrent_English,4*2);
//memcpy(&GV_ComuBoardState.commu_SC_ErrorType,&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,4*2);
	GV_ComuBoardState.commu_ErrorBatteryNo=GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorBattNo;

//				*boid=ici+1;
	GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[ici]=1;
	GV_ComuBoardState.commu_ErrorBoardNo=ici+1;
			break;						
	case EM_OverDisCurrent:   
		memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,&Array_ErrorType_OverDisCurrent,4*2);memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType_English,&Array_ErrorType_OverDisCurrent_English,4*2);
//memcpy(&GV_ComuBoardState.commu_SC_ErrorType,&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,4*2);
	GV_ComuBoardState.commu_ErrorBatteryNo=GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorBattNo;

//	*boid=ici+1;
	GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[ici]=1;
	GV_ComuBoardState.commu_ErrorBoardNo=ici+1;
			break;			


	case EM_ReceiveInterval:   
		memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,&Array_ErrorType_ReceiveInterval,4*2);memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType_English,&Array_ErrorType_ReceiveInterval_English,4*2);
//memcpy(&GV_ComuBoardState.commu_SC_ErrorType,&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,4*2);
GV_ComuBoardState.commu_ErrorBatteryNo=GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorBattNo;

//	*boid=ici+1;
	GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[ici]=1;
	GV_ComuBoardState.commu_ErrorBoardNo=ici+1;
break;
	case EM_CloseError:   
		memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,&Array_ErrorType_CloseError,4*2);memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType_English,&Array_ErrorType_CloseError_English,4*2);
//memcpy(&GV_ComuBoardState.commu_SC_ErrorType,&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,4*2);
GV_ComuBoardState.commu_ErrorBatteryNo=GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorBattNo;

//	*boid=ici+1;
	GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[ici]=1;
	GV_ComuBoardState.commu_ErrorBoardNo=ici+1;
break;

	case EM_CurrentBreakdown:   
		memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType,&Array_ErrorType_CurrentBreakdown,4*2);memcpy(&GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorType_English,&Array_ErrorType_CurrentBreakdown_English,4*2);

GV_ComuBoardState.commu_ErrorBatteryNo=GV_ComuBoardState.screenBatVolComuState[ici].SC_ErrorBattNo;

//	*boid=ici+1;
	GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[ici]=1;
	GV_ComuBoardState.commu_ErrorBoardNo=ici+1;
	
	
	default:  break;

}


}


}



















/*********************************************END OF FILE**********************/

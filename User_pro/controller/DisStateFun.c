
#include "./controller/DisStateFun.h"
#include "./screen/ScreenDataDis.h"
#include "./global/GV_variable.h" 
#include "./global/GV_enum.h" 
 #include "./sys/sysio.h"
//#include "./service/ModeWorkFun.h"
//#include "./io/OUT_io/bsp_gpio_out.h"   
//#include "./screen/ScreenSendFun.h"
//#include "./converter/data_convert.h"
//#include "./service/PowerDownFun.h"
//#include "./power/bsp_power.h"
//#include "./service/Workled.h"
//#include "./controller/StateFun.h"

//#include "./service/StandbyMechineFun.h"
//#include "./service/StartMechineFun.h"
//#include "./service/StopMechineFun.h"



extern str_ScreenSettingDataState GV_ScreenSettingDataState;
extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern str_HardwareState GV_HardwareState; 

extern uint16_t Array_WorkType_Stop[4];//待机中
extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;
extern str_PowerDownState GV_PowerDownState;

extern str_SD_data GV_SD_data;




//extern uint16_t Array_WorkType_ParrelCharge[4];//并充中
//extern uint16_t Array_WorkType_ParrelDisCharge[4];//并放中
//extern uint16_t Array_WorkType_Maintain[4];//维护中

extern uint16_t Array_WorkType_SerialCharge[4];//串充中
extern uint16_t Array_WorkType_SerialDisCharge[4];//串放中

extern uint16_t Array_WorkType_Stop[4];//待机中
extern uint16_t Array_WorkType_ParrelChargeFinish[4];//并充完成
extern uint16_t Array_WorkType_ParrelDisChargeFinish[4];//并放完成
extern uint16_t Array_WorkType_MaintainFinish[4];//维护完成
extern uint16_t Array_WorkType_NowMessage[4];

extern uint16_t Array_WorkType_Run[4];//运行中//
extern uint16_t Array_WorkType_Finish[4];//完成中//
extern uint16_t Array_WorkType_wrong[4];//故障中//均衡
//extern uint16_t Array_WorkType_Pause[4];//暂停中


extern uint16_t Array_WorkType_Run_English[ETE_Size];//运行中//
extern uint16_t Array_WorkType_Finish_English[ETE_Size];//完成中//
//extern uint16_t Array_WorkType_wrong_English[ETE_Size];//故障中//均衡
//extern uint16_t Array_WorkType_Pause_English[ETE_Size];//暂停中


extern uint16_t Array_WorkType_NowMessage_English[ETE_Size];


extern uint16_t Array_WorkType_Pause_English[ETE_Size];



extern str_ComuBoardState GV_ComuBoardState;


/**
  * @brief  设置电池维护状态
  * @param  
  * @retval 实现设置状态参数与板内工作状态参数分离，方便移植(最后一句着重点，把当前的工作状态传入通讯的工作状态中)
  */
void RunWorkMode_ChangeIn(uint8_t changeboard_id){
	
switch(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[changeboard_id]){
	case EM_SC_STOP:	
		
		SYSTEM_INFO("\n **********switch EM_SC_STOP*********%d*\n",changeboard_id);  
	break;
	case EM_SC_CONSTANTCURRENT_CHARGE:	
		
		SYSTEM_INFO("\n **********switch EM_SC_CONSTANTCURRENT_CHARGE*********%d*\n",changeboard_id); 
	break;

	case EM_SC_CONSTANTCURRENT_DISCHARGE:	
		
		SYSTEM_INFO("\n **********switch EM_SC_CONSTANTCURRENT_DISCHARGE*********%d*\n",changeboard_id);  
	break;	

	case EM_SC_CONSTANTCURRENT_BALANCE:	
		
		SYSTEM_INFO("\n **********switch EM_SC_CONSTANTCURRENT_BALANCE*********%d*\n",changeboard_id);  
	break;	

	
	default:
		SYSTEM_INFO("\n **********switch error*********%d*\n",changeboard_id);      break;		
}

 GV_ScreenSettingDataState.Set_Bat_Runworkingmode[changeboard_id]=GV_ScreenSetRunBoardState.Set_ChargeStateSelect[changeboard_id];




}









#include "./usart/bsp_485_usart.h"  

#include "./controller/TaskFun.h"
#include "./controller/TaskSingleWork.h"
#include "./communication/SD_485DataFun.h"
#include "./communication/Comu485Dis.h"
void MaintainStateRun_Start(uint8_t launchboardnum){
	//
//GV_SYSState.Run_HourMintuesSeconds=0;//清除计时时间，重新开始计时
	//clearCalculateVolcapacityAh();/清除容量，重新开始计算
	
	if(GV_ScreenSetRunBoardState.Set_NowRunState!=EM_RunState_Run){
	StartWork_ClearBuffDataInit();
	}
	memcpy(&Array_WorkType_NowMessage,&Array_WorkType_Run,4*2);
	memcpy(&Array_WorkType_NowMessage_English,&Array_WorkType_Run_English,ETE_Size*2);
	
	
	
	if((launchboardnum>=0)&&(launchboardnum<BOARD_COUNT_CHANGE)){
		GV_CommSendToSlaveBoardState.Effective |=(1<<(launchboardnum));
	GV_ScreenSetRunBoardState.Set_NowSingleRunState[launchboardnum]=EM_RunState_Run;
	}else if(launchboardnum==15){
	for (uint8_t i = 0; i < BOARD_COUNT_CHANGE; i++) {
		
    // 如果当前元素不等于0
    if ((GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i] != EM_SC_STOP) && ( GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0){
        // 将Effective的对应位设置为1（使用按位或操作）
        GV_CommSendToSlaveBoardState.Effective |= (1 << i);
			GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]=EM_RunState_Run;
					}
		
					
				}
}else{




}

	
	for(uint16_t i=0;i<GV_HardwareState.hard_board_count;i++){
	
		
	

		GV_ScreenSetRunBoardState.Set_ChargeStateSelect_resume[i]=GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i];
		
		
		
		
		if((GV_ScreenSettingDataState.Set_Bat_SingleBattRun[i]&0x4fff)==0){
	
	//GV_HardwareState.HW_NeedWorkingState[i]=0;
		GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]=EM_RunState_Stop;
		GV_CommSendToSlaveBoardState.S_Run[i]=EM_HW_STOP;
		GV_CommSendToSlaveBoardState.Effective &=(~(1<<(i)));
		
	};
		
		if(GV_PowerDownState.PowerDown_State==1){
			
			
			
		GV_CommSendToSlaveBoardState.S_Run[i]=EM_HW_LOSEPOWER_RESUME;//
		
		}else{
//		GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i]
			if(GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]!=0){
			
				GV_CommSendToSlaveBoardState.S_Run[i]=GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i];
			}else{
					GV_CommSendToSlaveBoardState.S_Run[i]=0;
			
			}
			
	
		}
		
	

		
		
//	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_ScreenSetRunBoardState.Set_ChargeStateSelect,8,"----////=====");
//		SYSTEM_DEBUG("\n **********Set_NowSingleRunState %d ",GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]);
	}
//GV_CommSendToSlaveBoardState
	
	
	SYSTEM_DEBUG_BINARY(GV_HardwareState.hard_board_whichIsActiveBit,"---------v-----");
	SYSTEM_DEBUG_BINARY(GV_ComuBoardState.commu_needfinishwork_FLAG,"---------OOO-----");

	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_ScreenSetRunBoardState.Set_NowSingleRunState,16,"Set_NowSingleRunState ==");
	
SendCommu485data();

	
	//SuspendTaskHandler(Handle_CheckStop_CommuOverTimeFalg);  
			if(GV_ScreenSetRunBoardState.Set_NowRunState!=EM_RunState_Run){
						GV_ScreenSetRunBoardState.Set_NowRunState=EM_RunState_Run;
//SetTaskHandlerTimeRload
				SetTaskHandlerTimeRloadAndChangeState(&Handle_SendVolDataToScreenAndAskSlave,ASK_SLAVE_WORK_DUTY_TIME,1);
				ResumeTaskHandler(Handle_CheckStop_ALLBoard_FinishWork);
//				SetTaskHandlerTimeRloadAndChangeState(&Handle_CheckStop_ALLBoard_FinishWork,4000,1);
       TurnFan_ON;
					SYSTEM_DEBUG("\n **********TurnFan_ON ");
			}
}







void MaintainStateRun_SingleBoard_Stop(uint8_t launchboardnum,uint8_t ifsend){

	if((launchboardnum>=0)&&(launchboardnum<BOARD_COUNT_CHANGE)){
		GV_CommSendToSlaveBoardState.Effective=(1<<(launchboardnum));
	GV_CommSendToSlaveBoardState.S_Run[launchboardnum]=EM_HW_STOP;
		
	}

		if(ifsend==1){
	SendCommu485data();
	}


	
//SD_STOP_Fun();
//	SetTaskHandlerTimeRloadAndChangeState(&Handle_SendVolDataToScreenAndAskSlave,ASK_SLAVE_NOTWORK_DUTY_TIME,1);//999999
//	TurnFan_OFF;
	SYSTEM_INFO("\n **********SingleBoardStop******** b= %d **\n",launchboardnum);
	

}





void MaintainStateRun_AllBoard_Stop(uint8_t ifsend){

	for (uint8_t i = 0; i < BOARD_COUNT_CHANGE; i++) {
    // 如果当前元素不等于0
    if (GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i] != 0) {
        // 将Effective的对应位设置为1（使用按位或操作）
        GV_CommSendToSlaveBoardState.Effective |= (1 << i);
    }
		
		GV_CommSendToSlaveBoardState.S_Run[i]=EM_HW_STOP;
		GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]=EM_RunState_Stop;
		
		}
		
		
			if(GV_ScreenSetRunBoardState.Set_NowRunState!=EM_RunState_Stop){
		
	GV_ScreenSetRunBoardState.Set_NowRunState=EM_RunState_Stop;

memcpy(&Array_WorkType_NowMessage_English,&Array_WorkType_Finish_English,ETE_Size*2);
memcpy(&Array_WorkType_NowMessage,&Array_WorkType_Finish,4*2);
		
//SYSTEM_INFO("\n **********switch Mode_MaintainState_Stop**********\n");
	}
			
	
GV_ComuBoardState.commu_finishwork_Old_FLAG=0;
GV_ComuBoardState.commu_needfinishwork_FLAG=0;
	
			if(ifsend==1){
	SendCommu485data();
	}
	
SYSTEM_INFO("\n **********ALLboardStop**********\n");

}












//断电恢复
void MaintainState_lostPowerResume(void){

	StartWork_ClearBuffDataInit();
	
	memcpy(&Array_WorkType_NowMessage,&Array_WorkType_Run,4*2);
	memcpy(&Array_WorkType_NowMessage_English,&Array_WorkType_Run_English,ETE_Size*2);
	
	for(uint16_t i=0;i<GV_HardwareState.hard_board_count;i++){
		
//	TurnOnLed_ByMode_fun(i);
		turnOnLight(i);
	//GV_HardwareState.HW_NowRealWorkingState[i]=GV_HardwareState.HW_NeedWorkingState[i];
	GV_CommSendToSlaveBoardState.S_Run[i]=EM_HW_LOSEPOWER_RESUME;

	if(GV_CommSendToSlaveBoardState.S_Run[i]==EM_HW_LOSEPOWER_RESUME){//有问题，需改
	
		GV_ComuBoardState.commu_needfinishwork_FLAG|=(1<<i);

	}else{
	
	GV_ComuBoardState.commu_needfinishwork_FLAG&=(~(1<<i));
	
	}
	
	}

	SYSTEM_DEBUG_BINARY(GV_ComuBoardState.commu_needfinishwork_FLAG,"");
	//SYSTEM_DEBUG("%B\n", GV_ComuBoardState.commu_needfinishwork_FLAG);
	
	GV_ScreenSetRunBoardState.Set_NowRunState=EM_RunState_Run;



	//_485_A_TX_EN();Delay(200);
//		for(uint16_t i=0;i<0;i++){
//	comu_Sendslave_Rundata();
//		delay_ms(200);
//	}
	SendCommu485data();
	SYSTEM_DEBUG("\n **********here ");
	//SD_BeginSave_Fun();
	
	//SuspendTaskHandler(Handle_CheckStop_CommuOverTimeFalg);  
SetTaskHandlerTimeRloadAndChangeState(&Handle_SendVolDataToScreenAndAskSlave,ASK_SLAVE_WORK_DUTY_TIME,1);
	ResumeTaskHandler(Handle_CheckStop_ALLBoard_FinishWork);
	
}















//void Runwhile_CheckStop(void){
//	
//if((checkNeedStateNotEquRealState()==1)&&(GV_HardwareState.HW_NeedAllWorkingState==EM_HW_STOP)){//只有在启动运行中才判断是否停止
//	MaintainStateRun_SingleBoard_Stop();
//return;
//}



//}



void CheckErrorStop(void){

if(GV_ComuBoardState.commu_ErrorBoardNo!=0){
	
	for(uint16_t i=0;i<4;i++){
	
	if((GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[GV_ComuBoardState.commu_ErrorBoardNo-1]!=0)
		&&(GV_ComuBoardState.commu_F_ErrorBoardNo_EnterInFLAG[GV_ComuBoardState.commu_ErrorBoardNo-1]==0)){
	if(GV_ScreenSetRunBoardState.Set_NowSingleRunState[GV_ComuBoardState.commu_ErrorBoardNo-1]!=EM_RunState_Stop){
	
	StopBoardFunByBoardId(GV_ComuBoardState.commu_ErrorBoardNo-1,EM_NotSend);
//	TurnWrong_led_ON;
		GetWrongLed_STATEAndled_ON;
		GV_ComuBoardState.commu_F_ErrorBoardNo_EnterInFLAG[GV_ComuBoardState.commu_ErrorBoardNo-1]=1;
	}
	}
	}
	
	
	
//	if((GV_ScreenSetRunBoardState.Set_NowSingleRunState[GV_ComuBoardState.commu_ErrorBoardNo-1]!=EM_RunState_Stop)
//		&&(GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[GV_ComuBoardState.commu_ErrorBoardNo-1]==1)){

//	
//			StopBoardFunByBoardId(GV_ComuBoardState.commu_ErrorBoardNo-1,EM_NotSend);
//			GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[GV_ComuBoardState.commu_ErrorBoardNo-1]=0;
//	TurnWrong_led_ON;
//	
//	}

	
	
	
//}else if((GV_ComuBoardState.commu_ErrorBoardNo==0)&&(GV_ComuBoardState.commu_overtime_FLAG!=1)){
	}else if((GV_ComuBoardState.commu_ErrorBoardNo==0)&&(GV_ComuBoardState.commu_overtime_FLAG!=1)){
TurnWrong_led_OFF;
	clearAllErrorBoardNoFlag();
}

}



void clearAllErrorBoardNoFlag(void){

//GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[ici]=1;
for(uint16_t i=0;i<4;i++){
GV_ComuBoardState.commu_F_ErrorBoardNo_FLAG[i]=0;

}

}




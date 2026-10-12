


#include "./service/StopMechineFun.h"

#include "./controller/StateFun.h"

#include "./controller/DisStateFun.h"
//#include "./controller/StateFun.h"

#include "./screen/ScreenDataDis.h"
#include "./controller/DisStateFun.h"
#include "./global/GV_enum.h" 
 #include "./sys/sysio.h"
//#include "./io/OUT_io/bsp_gpio_out.h" 
#include "./communication/SD_485DataFun.h"
#include "./service/StandbyMechineFun.h"
#include "./service/StartMechineFun.h"
#include "./power/bsp_power.h"
//#include "./controller/TaskFun.h"
//#include "./controller/TaskSingleWork.h"
#include "./service/PowerDownFun.h"
#include "./service/StartMechineFun.h"
#include "./project_API/hardware_interface_api.h"   
//extern ScreenCommu_Item_t  GV_ScreenCommu;
//extern str_HardwareState GV_HardwareState; 
//extern str_BatterySetCalibration GV_BatterySetCalibration;
//extern str_ComuBoardState GV_ComuBoardState;
#include <string.h>
extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
//extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;
extern str_HardwareState GV_HardwareState; 


extern uint16_t Array_WorkType_Stop[4];//待机中
extern uint16_t Array_WorkType_Stop_English[ETE_Size];//待机中

extern uint16_t Array_WorkType_NowMessage[4];
extern uint16_t Array_WorkType_NowMessage_English[ETE_Size];



//通过板号停止对应的板
void StopBoardFunByBoardId(uint8_t launchboardnum,uint8_t ifsend){
//uint16_t tmpdata=0;
	
	if(GV_ScreenSetRunBoardState.Set_NowRunState!=EM_RunState_Stop){
	
	
	
			MaintainStateRun_SingleBoard_Stop(launchboardnum,ifsend);
			GV_ScreenSetRunBoardState.Set_NowSingleRunState[launchboardnum]=EM_RunState_Stop;
				launch_Machine_PowerDownUpdatetmpdata();	
			Set_NeedfinishWorkBit_StateFun(launchboardnum);
			ChangeNboardTo_stopORstart_IconState(launchboardnum,0);
			LedClose_ByBoardidFun(launchboardnum);
//	SYSTEM_getAllLightsStateCounters();
	
			// 遍历Set_ChargeStateSelect数组的前4个元素
			if(AutoChangeOneStartIconFun()==0){
		disable_pwr();
//				Set_NeedfinishWorkBit_AllBoard_StateFun();
				GV_ScreenSetRunBoardState.Set_NowRunState=EM_RunState_Stop;
//			TurnFan_OFF;
				SD_STOP_FunOuter();
//	SetTaskHandlerTimeRloadAndChangeState(&Handle_SendVolDataToScreenAndAskSlave,ASK_SLAVE_NOTWORK_DUTY_TIME,1);
//				
//		SuspendTaskHandler(Handle_CheckStop_ALLBoard_FinishWork);
//				
memcpy(&Array_WorkType_NowMessage,&Array_WorkType_Stop,4*2);
memcpy(&Array_WorkType_NowMessage_English,&Array_WorkType_Stop_English,ETE_Size*2);
			}

			}
}



//变为停止状态下的图标//0-停止运行后状态下图标 1-启动运行后下的图标
void ChangeNboardTo_stopORstart_IconState(uint8_t launchboardnum,uint16_t stopORstart){
uint16_t tmpdata=stopORstart;
//		SCREEN_write_cmd(EM_Icon_RunState_START01+(launchboardnum),&tmpdata,1);
//		SCREEN_write_cmd(EM_ButtonIcon_Mode_Select_1+(launchboardnum),&tmpdata,1);	
//		SCREEN_write_cmd(EM_Button_RunState_Switch01+(launchboardnum),&tmpdata,1);	

}

void ChangeNboardTo_stopORstart_ALL_IconState(uint16_t stopORstart){
	
	for(uint8_t i=0;i<BOARD_COUNT_CHANGE;i++){
	if((GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i]==EM_HW_STOP)&&(GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]==EM_RunState_Stop)){
	

	}else{
	
		ChangeNboardTo_stopORstart_IconState(i,stopORstart);
	
	}


	}

}

void AutoChangeNboardTo_stopORstart_ALL_IconState(void){
	
	for(uint8_t i=0;i<BOARD_COUNT_CHANGE;i++){
		
	if(GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]==EM_RunState_Run){
	
			ChangeNboardTo_stopORstart_IconState(i,EM_RunState_Run);
	}else{
	
		ChangeNboardTo_stopORstart_IconState(i,EM_RunState_Stop);
	
	}


	}

}





//关闭某个板号的的灯
void LedClose_ByBoardidFun(uint8_t boardid){

	if((GV_ScreenSetRunBoardState.Set_NowSingleRunState[boardid]==EM_RunState_Stop)	&&	(GV_HardwareState.hard_board_whichIsActiveBit & (1 << boardid)) != 0){
	
	
	switch(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[boardid]){
	case EM_SC_STOP:	
		
  break;
	case EM_SC_CONSTANTCURRENT_CHARGE:
		
//		TurnParrelCharge_led_ON;
	 Turnled_OFF(01); break;

	case EM_SC_CONSTANTCURRENT_DISCHARGE:	
		Turnled_OFF(02); break;

	case EM_SC_CONSTANTCURRENT_BALANCE:	
		Turnled_OFF(03); break;
     break;	
		
	default:
		    break;		
}
	}

}

void LedClose_All_ByBoardidFun(void){

	for(uint8_t i=0;i<4;i++){
	
LedClose_ByBoardidFun(i);

	}


}





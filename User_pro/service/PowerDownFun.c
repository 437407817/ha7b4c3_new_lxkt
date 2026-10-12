






#include "./service/PowerDownFun.h"
#include "./global/GV_variable.h" 
#include "./global/GV_enum.h" 
#include "./sys/sysio.h"
#include "./service/EepromFun.h"
//#include "./screen/Screen.h"
#include "./screen/DW_com_send.h"
#include "./screen/ScreenDataDis.h"
#include "./controller/DisStateFun.h"
#include "./power/bsp_power.h"
#include "./controller/StateFun.h"
#include "./service/StandbyMechineFun.h"
#include "./service/StartMechineFun.h"
#include "./service/StopMechineFun.h"
#include "./communication/SD_485DataFun.h"



extern str_HardwareState GV_HardwareState; 
extern ScreenCommu_Item_t  GV_ScreenCommu;
extern str_PowerDownState GV_PowerDownState;

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
//extern str_ComuBoardState
extern  uint16_t Array_Page_PowerDown[2];
extern  str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;


void initPowerDownData(void){

//	for(uint16_t i=0;i<8;i++){
//	GV_PowerDownState.PowerDown_resumeState[i]=GV_ScreenSetRunBoardState.Set_ChargeStateSelect_resume[i];

//	}
//GV_PowerDownState.PowerDown_State=0;

	
	GetPowerDownData();
//	GV_PowerDownState.PowerDown_BoardNowRunBit
//	GV_PowerDownState.PowerDown_State
//		for(uint16_t i=0;i<8;i++){
//	GV_PowerDownState.PowerDown_resumeState[i]=GV_ScreenSetRunBoardState.Set_ChargeStateSelect_resume[i];

//	}


}








//跳转页面
void disposeLaunchPageIfJumptoPowerDownPage(void){
	
if(GV_PowerDownState.powerdown_first_enter==0){

//GetPowerDownData();
	if(GV_PowerDownState.PowerDown_State==1){//上次是断电状态


				  GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_PowerDown;

		GV_ScreenCommu.ScreenDataSize=2;
//		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面
	
	
	}

GV_PowerDownState.powerdown_first_enter=1;
	
}



}




void launch_Machine_PowerDownUpdatetmpdata(void){

//GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i]

			for(uint16_t i=0;i<8;i++){
				
				GV_PowerDownState.PowerDown_resumeState[i]=	GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i];
				
				if((GV_ScreenSetRunBoardState.Set_NowSingleRunState[i] != EM_RunState_Stop) 
					&&(( GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0)
				){
				
				GV_PowerDownState.PowerDown_BoardNowRunBit|=(1<<i);
					
				}else{
				GV_PowerDownState.PowerDown_BoardNowRunBit &=(~(1<<i));
				
				}
	
				
	}
			
	SYSTEM_DEBUG_BINARY(GV_PowerDownState.PowerDown_BoardNowRunBit,"PowerDown_BoardNowRunBit \n");
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t*)GV_ScreenSetRunBoardState.Set_NowSingleRunState,16," Set_NowSingleRunState  \n");
	    
			enable_pwr();
}


void ResumeLaunchPowerDownFun(void){
	uint16_t tmpdata;
	
//	GV_PowerDownState.PowerDown_BoardNowRunBit
	
	SYSTEM_DEBUG_BINARY(GV_PowerDownState.PowerDown_BoardNowRunBit,"  ****--- PowerDown_BoardNowRunBit \n");
			for(uint16_t i=0;i<8;i++){
	GV_ScreenSetRunBoardState.Set_ChargeStateSelect_resume[i]=GV_PowerDownState.PowerDown_resumeState[i];
	if((GV_PowerDownState.PowerDown_BoardNowRunBit & (1 << i)) != 0){
		
		GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]=EM_RunState_Run;
		GV_CommSendToSlaveBoardState.Effective |= (1 << i);
	}
			
	

	}
			SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t*)GV_ScreenSetRunBoardState.Set_NowSingleRunState,16," Set_NowSingleRunState  \n");
			SYSTEM_DEBUG_BINARY(GV_CommSendToSlaveBoardState.Effective,"  ****--- Effective \n");
	
	
		memcpy(&GV_ScreenSetRunBoardState.Set_ChargeStateSelect,&GV_ScreenSetRunBoardState.Set_ChargeStateSelect_resume,16);
	
//	SCREEN_write_cmd(EM_Icon_Mode_Select_1,GV_ScreenSetRunBoardState.Set_ChargeStateSelect,8);//改图标

			MaintainStateRun_Start(16);
    LedOpen_All_ByBoardidFun();
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t*)GV_ScreenSetRunBoardState.Set_NowSingleRunState,16," Set_NowSingleRunState 2 \n");
	Set_NeedfinishWorkBit_AllBoard_StateFun();
launch_Machine_PowerDownUpdatetmpdata();
	
//	ChangeNboardTo_stopORstart_ALL_IconState(EM_RunState_Run);
	 AutoChangeNboardTo_stopORstart_ALL_IconState();
	AutoChangeOneStartIconFun();//改变一键启动板图标
	
	
			GV_PowerDownState.PowerDown_State=0;
		SavePowerDownData();
	SD_BeginSave_Fun();
		

	
}


void CancelLaunchPowerDownFun(void){
//		memcpy(&GV_ScreenSetRunBoardState.Set_ChargeStateSelect,&GV_ScreenSetRunBoardState.Set_ChargeStateSelect_resume,16);
//	SCREEN_write_cmd(EM_Icon_Mode_Select_1,GV_ScreenSetRunBoardState.Set_ChargeStateSelect,8);

//			MaintainStateRun_Start();
	
			GV_PowerDownState.PowerDown_State=0;
		SavePowerDownData();
	
//		GV_ScreenSetRunBoardState.Set_NowRunState=EM_RunState_Run;
//		SCREEN_write_cmd(EM_Icon_RunState_Switch,&GV_ScreenSetRunBoardState.Set_NowRunState,1);
	
}







void TEST_PowerDownSAVE(void){
enable_pwr();
	GV_PowerDownState.powerdown_first_enter=0;
			GV_PowerDownState.PowerDown_State=1;
		SavePowerDownData();
	

	
}











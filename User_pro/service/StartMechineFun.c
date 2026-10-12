


#include "./service/StartMechineFun.h"

#include "./controller/StateFun.h"

#include "./controller/DisStateFun.h"
//#include "./controller/StateFun.h"




#include "./screen/ScreenDataDis.h"
#include "./controller/DisStateFun.h"
#include "./global/GV_enum.h" 
 #include "./sys/sysio.h"
//#include "./io/OUT_io/bsp_gpio_out.h" 
//extern ScreenCommu_Item_t  GV_ScreenCommu;
//extern str_HardwareState GV_HardwareState; 
//extern str_BatterySetCalibration GV_BatterySetCalibration;
//extern str_ComuBoardState GV_ComuBoardState;

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;


extern str_ComuBoardState GV_ComuBoardState;

extern str_HardwareState GV_HardwareState; 






uint16_t StartMechineFunBoardFunByBId(uint8_t launchboardnum){



}




//打开某个板号的的灯,无需考虑设置状态
void LedOpen_ByBoardidFun(uint8_t boardid){

//	TurnFinish_led_OFF;
	if((GV_ScreenSetRunBoardState.Set_NowSingleRunState[boardid] == EM_RunState_Run)	&&	(GV_HardwareState.hard_board_whichIsActiveBit & (1 << boardid)) != 0){
	
			switch(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[boardid]){
	case EM_SC_STOP:	
		
  break;
	case EM_SC_CONSTANTCURRENT_CHARGE:
		
//		TurnParrelCharge_led_ON;
//	 turnOnLight(1); break;

	case EM_SC_CONSTANTCURRENT_DISCHARGE:	
//		turnOnLight(2); break;

	case EM_SC_CONSTANTCURRENT_BALANCE:	
//		turnOnLight(3); break;
     break;	
		
	default:
		    break;		
}
		
		
	}



}

void LedOpen_All_ByBoardidFun(void){

	for(uint8_t i=0;i<4;i++){
	
LedOpen_ByBoardidFun(i);

	}


}







void Set_NeedfinishWorkBit_StateFun(uint8_t i){


	
	if(GV_ScreenSetRunBoardState.Set_NowSingleRunState[i] != EM_RunState_Stop  && (GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0){
		GV_ComuBoardState.commu_needfinishwork_FLAG|=(1<<i);

		
//    TurnOnLed_ByMode_fun(i);
//		 turnOnLight(1);
		
	}else{
	
	GV_ComuBoardState.commu_needfinishwork_FLAG&=(~(1<<i));

	}

	
	GV_ComuBoardState.commu_finishwork_Old_FLAG&=(~(1<<i));
	GV_ComuBoardState.commu_finishwork_FLAG&=(~(1<<i));
	
	
}


void Set_NeedfinishWorkBit_AllBoard_StateFun(void){


	for(uint8_t i=0;i<4;i++){
	
Set_NeedfinishWorkBit_StateFun(i);

	}

}

















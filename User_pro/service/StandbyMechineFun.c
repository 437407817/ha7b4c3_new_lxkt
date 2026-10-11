


#include "./service/StandbyMechineFun.h"

#include "./controller/StateFun.h"

#include "./controller/DisStateFun.h"
//#include "./controller/StateFun.h"

#include "./global/GV_enum.h" 
#include "./screen/DW_com_send.h"
#include "./defines.h"

//#include "./screen/ScreenDS1302.h"
#include "./converter/data_convert.h"
#include <stdlib.h>

 #include "./sys/sysio.h"
#include "./screen/ScreenDataDis.h"
#include "./service/StandbyMechineFun.h"
#include "./service/StopMechineFun.h"
#include "./power/bsp_power.h"

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern str_HardwareState GV_HardwareState; 


uint16_t AutoChangeOneStartIconFun(void){
uint16_t tmpdata;
for (uint16_t i = 0; i < BOARD_COUNT_CHANGE; i++) {
    // 如果当前元素不等于0
    if ((GV_ScreenSetRunBoardState.Set_NowSingleRunState[i] != EM_RunState_Stop)&&(GV_HardwareState.hard_board_whichIsActiveBit & (1 << i)) != 0) {
        // 将Effective的对应位设置为1（使用按位或操作）
			
			tmpdata=1;
//	GV_ScreenSetRunBoardState.Set_NowRunState=EM_RunState_Run;
			SCREEN_write_cmd(EM_Icon_RunState_SwitchAll,&tmpdata,1);
        return 1;
    }
}
tmpdata=0;
//	GV_ScreenSetRunBoardState.Set_NowRunState=EM_RunState_Stop;
			SCREEN_write_cmd(EM_Icon_RunState_SwitchAll,&tmpdata,1);
			return 0;
		

}



//overtime
void ChangeOneClickCloseIconAndNboardIcon(void){


			ChangeNboardTo_stopORstart_ALL_IconState(EM_RunState_Stop);
			AutoChangeOneStartIconFun();//改变一键启动板图标
	
	
//				for (uint8_t i = 0; i < BOARD_COUNT_CHANGE; i++) {
//    // 如果当前元素不等于0
//    if (GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i] != EM_SC_STOP) {
//        // 将Effective的对应位设置为1（使用按位或操作）
////       			GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]=EM_RunState_Stop;

//			
//			ChangeNboardTo_stopORstart_IconState(i,EM_RunState_Stop);
//    }
//}
//				
//AutoChangeOneStartIconFun();

disable_pwr();
}




void ChangeOneClickOpenIconAndNboardIcon(void){

ChangeNboardTo_stopORstart_ALL_IconState(EM_RunState_Run);
		
//				for (uint8_t i = 0; i < BOARD_COUNT_CHANGE; i++) {
//    // 如果当前元素不等于0
//    if (GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i] != EM_SC_STOP) {
//        // 将Effective的对应位设置为1（使用按位或操作）
//        		GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]=EM_RunState_Run;

//			ChangeNboardTo_stopORstart_IconState(i,EM_RunState_Run);
//    }
//}
			AutoChangeOneStartIconFun();

enable_pwr();

}







uint16_t StandbyMechineFunMechineFunBoardFunByBId(uint8_t launchboardnum){



}



#include "./service/CapacityFun.h"

#include "./sys/bsp_systime.h"   

#include "./global/GV_variable.h" 
#include "./screen/DW_com_send.h"

#include "./screen/ScreenDataDis.h"
#include <string.h>

extern ScreenCommu_Item_t  GV_ScreenCommu;
extern str_HardwareState GV_HardwareState; 
extern str_BatterySetCalibration GV_BatterySetCalibration;
extern str_ComuBoardState GV_ComuBoardState;

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;


void ScreenCapacityDataToScreen(void){

	for(uint16_t ici=0;ici<GV_HardwareState.hard_board_count;ici++){
	memcpy(&GV_ComuBoardState.screenBatCapComuState[ici].SC_Energy[0],&GV_ComuBoardState.getSlaveUpdloadState[ici].BS_Energy,12*2);//复制电压电流

		//if()
		
	}

	
	//SYSTEM_DEBUG("\n **********Capacity **********\n");
for(uint8_t ici=0;ici<=GV_HardwareState.hard_board_count;ici++){
	
	GV_ComuBoardState.screenBatVolComuState[ici].SC_SUMvol=GV_HardwareState.batterystate[ici].SumParallel_vol_Value;
//	if(GV_ScreenSetRunBoardState.Set_NowShowPageNum==ici+1){
	
//	SCREEN_write_cmd(EM_CAPACITY_01_01+24*ici,GV_ComuBoardState.screenBatCapComuState[ici].SC_Energy,sizeof(str_ScreenBatCapComuState)/2);
		
//	}
}

}


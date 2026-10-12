


#include "./service/ActiveMechineFun.h"

#include "./controller/StateFun.h"

#include "./controller/DisStateFun.h"
//#include "./controller/StateFun.h"

#include "./crypt/bsp_encode.h"


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
extern ScreenCommu_Item_t  GV_ScreenCommu;
//extern str_ScreenSettingDataState OldGV_ScreenSettingDataState;





  uint16_t Array_Page_Active[2]={0x5A01,0x006E};  
 uint16_t Array_Page_Active_English[2]={0x5A01,0x006E}; 



uint16_t ActiveMechineFailure(void){

			if(NotTryOutAndActiveMechineReturn()){

					SYSTEM_INFO("\n not TryOut active \n");
				
			if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){
			GV_ScreenCommu.ScreenData =Array_Page_Active;
			}else{
			GV_ScreenCommu.ScreenData =Array_Page_Active_English;
			}
		GV_ScreenCommu.ScreenDataSize=2;
//		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//ÇÐ»»Ò³Ãæ12
			return 1;
			
			}
return 0;
}














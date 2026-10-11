#include "./controller/StateFun.h"

#include "./sys/bsp_systime.h"   

#include "./global/GV_variable.h" 
//#include "./screen/Screen.h"
//#include "./defines.h"

//#include "./screen/ScreenDS1302.h"
//#include "./converter/data_convert.h"
//#include <stdlib.h>

 #include "./sys/sysio.h"
#include "./global/GV_enum.h" 
#include "./controller/DisStateFun.h"
//#include "./screen/ScreenDataDis.h"

//#include "./service/EepromFun.h"
//#include "./controller/DisStateFun.h"
//#include "./communication/Comu485Dis.h"
//#include "./service/DisposeDataFun.h"
//#include "./controller/TaskFun.h"
//#include "./controller/TaskSingleWork.h"

//#include "./usart/bsp_usart_sd.h"
//#include "./communication/SD_485Dis.h"
//#include "./usart/bsp_485_usart.h" 
//#include "./screen/ScreenSendFun.h"
//#include "./io/OUT_io/bsp_gpio_out.h" 
//#include "./service/PowerDownFun.h"
//#include "./communication/SD_485DataFun.h"
//#include "./crypt/chipID_dis.h"
//#include "./crypt/bsp_encode.h"
//#include "./power/bsp_power.h"

//#include "./service/StopMechineFun.h"
//#include "./service/ActiveMechineFun.h"
//#include "./service/StandbyMechineFun.h"
//#include "./service/StartMechineFun.h"
//#include "./io/IN_io/bsp_gpio_in.h"
#include "./screen/DW_com_send.h"
#include "./screen/ScreenDataDis.h"

extern str_ScreenSettingDataState GV_ScreenSettingDataState;

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern str_HardwareState GV_HardwareState; 
extern str_EEpromSettingState TMP_EEpromSettingState;

extern str_EEprom_SYS_SettingDataState TMP_EEprom_SYS_SettingState;
extern str_ScreenSettingDataState OldGV_ScreenSettingDataState;

extern ScreenCommu_Item_t  GV_ScreenCommu;
extern str_BatterySetCalibration GV_BatterySetCalibration;

extern str_ScreenAllCalibration GV_ScreenAllCalibration;

extern str_BatterySetCalibration BigD_GV_BatterySetCalibration;
extern str_Screen_SYS_SettingDataState Screen_SYS_SettingDataState;
extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;


//extern SYSState_Item_t GV_SYSState;


 uint16_t Array_Page01[2]={0x5A01,0x0001};
  uint16_t Array_Page02[2]={0x5A01,0x0002};
	 uint16_t Array_Page03[2]={0x5A01,0x0003};
	  uint16_t Array_Page04[2]={0x5A01,0x0004};
		
		
 uint16_t Array_Page01_English[2]={0x5A01,0x0065};
 
 
 uint16_t Array_Page_Language[2]={0x5A01,0x0009};
 
 
 uint16_t Array_Page_Main[2]={0x5A01,0x000A};  
 uint16_t Array_Page_Main_English[2]={0x5A01,0x003C}; 
 

 
 
 
 uint16_t Array_Page_Uploading[2]={0x5A01,0x0050};
 uint16_t Array_Page_SaveSucceed[2]={0x5A01,0x0054};
 uint16_t Array_Page_SaveFailure[2]={0x5A01,0x0055};
// uint16_t Array_Page_SaveFailure[2]={0x5A01,0x000E}; 
 uint16_t Array_Page_RunningCanntSave[2]={0x5A01,0x0056};
 uint16_t Array_Page_SettingWrong[2]={0x5A01,0x0057};
 
 uint16_t Array_Page_PowerDown[2]={0x5A01,0x0063};
 
 uint16_t Array_Page_ActiveFailure[2]={0x5A01,0x0070}; 
 uint16_t Array_Page_ActiveSuccess[2]={0x5A01,0x0071}; 
 
// uint16_t Array_Page_Uploading[2]={0x5A01,0x0050};
 
 
extern uint16_t Array_WorkType_Stop[4];//待机中
extern uint16_t Array_WorkType_NowMessage[4];

static uint16_t tmpdata=0;










 
/**
  * @brief  设置电池维护状态
  * @param  
  * @retval 无
  */


void MaintainState_Select(uint8_t num,uint8_t* p_data){
	
	uint16_t i=0;
	
	i=(*(p_data+1));
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)GV_ScreenSetRunBoardState.Set_NowSingleRunState,8,"  --------0000   ");
if(GV_ScreenSetRunBoardState.Set_NowSingleRunState[i]==EM_RunState_Stop){//停的时候才能设置


GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i]++;
GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i]=GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i];
	
//	if(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i]==EM_HW_CHARGE)//恒流并充
//{
////GV_ScreenSetRunBoardState.Set_ChargeStateSelect=2;

//}if(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i]==EM_HW_DISCHARGE){

// //GV_ScreenSetRunBoardState.Set_ChargeStateSelect=0;
//}if(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i]==EM_HW_MAINTAIN){

// //GV_ScreenSetRunBoardState.Set_ChargeStateSelect=0;
//}


if(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i]>EM_SC_CONSTANTCURRENT_BALANCE){  //EM_SC_CONSTANTVOL_SERIALDISCHARGE
//TurnWrong_led_OFF;
 GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i]=EM_HW_STOP;
	GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i]=EM_HW_STOP;
}
	
		//GV_HardwareState.HW_NeedWorkingState[i]=GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i];
	
	

	RunWorkMode_ChangeIn( i);
//SYSTEM_DEBUG("\n SCREEN  %d  ICON IS  %d \n",i,GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i]);
//SCREEN_write_cmd_print(EM_Icon_Mode_Select_1+i,&GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i],1,0);//修改屏幕的模式图标
DW_SCREEN_write_cmd_print(EM_Icon_Mode_Select_1+i,&GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i],1,0);//修改屏幕的模式图标
//SYSTEM_DEBUG("\n SCREEN  %d  ICON IS  0X%X \n",GV_ScreenSetRunBoardState.Set_ChargeStateSelect[i],EM_Icon_Mode_Select_1+i);
}
}











//if((*++p_data)==2){//按暂停处理
/**
  * @brief  设置电池启动状态
  * @param  
  * @retval 无
  */

void RunState_Switch(uint8_t num,uint8_t* p_data,uint8_t launchboardnum){

	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)p_data,num,"change work board %d",launchboardnum);
	
	if((launchboardnum>=0)&&(launchboardnum<BOARD_COUNT_CHANGE)){
	GV_ScreenSetRunBoardState.Set_NowShowPageNum=launchboardnum+1;
	}

if((*(p_data+1))==0x00F1){//按启动按钮	
		if(GV_ScreenSetRunBoardState.Set_NowSingleRunState[launchboardnum]==EM_RunState_Stop){
		//启动++++++++++++//判断如果是不可启动状态
		if((GV_HardwareState.ReadyRunState!=1)||(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[launchboardnum]==EM_HW_STOP)||
			((GV_ScreenSettingDataState.Set_Bat_SingleBattRun[launchboardnum]&0x4fff)==0)){
		
				Jump_to_now_page();
			//999 屏幕输出不可启动汉字
				SYSTEM_INFO("\n ReadyRunState !=1 cannot run\n");
			return;
		}		
			

			
//			if(ActiveMechineFailure()==1){
//			return;
//			
//			}
			
			
		
		GV_ScreenSetRunBoardState.Set_NowSingleRunState[launchboardnum]=EM_RunState_Run;
		
			MaintainStateRun_Start(launchboardnum);
//			LedOpen_ByBoardidFun(launchboardnum);
//			Set_NeedfinishWorkBit_StateFun(launchboardnum);
//			launch_Machine_PowerDownUpdatetmpdata();
//		  
//			ChangeNboardTo_stopORstart_IconState(launchboardnum,1);//改变单板图标
//			AutoChangeOneStartIconFun();//改变一键启动板图标
//			
//			SD_BeginSave_Fun();
//			
//			
//			SYSTEM_getAllLightsStateCounters();
			SYSTEM_INFO("\n RunState = Run   1 state\n");
			
		}else if(GV_ScreenSetRunBoardState.Set_NowSingleRunState[launchboardnum]==EM_RunState_Run){
		//停止

//			StopBoardFunByBoardId(launchboardnum,EM_Send);
			
			
			
			
//			SYSTEM_getAllLightsStateCounters();
			
//			SYSTEM_DEBUG_BINARY(getAllLightsState(),"****led **=\n");
			SYSTEM_INFO("\n RunState = STOP   0 state\n");
		}
	//启动
		

SYSTEM_INFO("\n EM_ButtonIcon_Mode_Select_1 %d  %d\n",EM_ButtonIcon_Mode_Select_1+(launchboardnum),tmpdata);
		Jump_to_now_page();
		
		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)GV_ScreenSetRunBoardState.Set_NowSingleRunState,8,"  --------1111----  ");
		
	}
	
}





















void RunState_SwitchAll(uint8_t num,uint8_t* p_data){
//RunState_Switch(uint8_t num,uint8_t* p_data,uint8_t launchboardnum)
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)p_data,num,"change work board ");
	

	GV_ScreenSetRunBoardState.Set_NowShowPageNum=1;


if((*(p_data+1))==0x00F1){//按启动按钮	
		if(GV_ScreenSetRunBoardState.Set_NowRunState==EM_RunState_Stop){
		//启动++++++++++++//判断如果是不可启动状态
		if((GV_HardwareState.ReadyRunState!=1)||((GV_ScreenSetRunBoardState.Set_ChargeStateSelect[0]==EM_HW_STOP)&&(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[1]==EM_HW_STOP)
			&&(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[2]==EM_HW_STOP)&&(GV_ScreenSetRunBoardState.Set_ChargeStateSelect[3]==EM_HW_STOP))){
		

				Jump_to_now_page();
			//999 屏幕输出不可启动汉字
		SYSTEM_INFO("\n ReadyRunState !=1 cannot run\n");
			return;
		}		
		
		
//					if(ActiveMechineFailure()==1){
//			      return;
//			     }
					
		  
			MaintainStateRun_Start(15);
//					LedOpen_All_ByBoardidFun(); 
//					 
//			Set_NeedfinishWorkBit_AllBoard_StateFun();
//					 launch_Machine_PowerDownUpdatetmpdata();
//					 
//			ChangeNboardTo_stopORstart_ALL_IconState(EM_RunState_Run);
//			AutoChangeOneStartIconFun();//改变一键启动板图标

//			

//			
//			SD_BeginSave_Fun();
//					 SYSTEM_getAllLightsStateCounters();
//			SYSTEM_DEBUG("\n RunState = Run   1 state\n");
			
		}else if(GV_ScreenSetRunBoardState.Set_NowRunState==EM_RunState_Run){
		//停止

			
		
			MaintainStateRun_AllBoard_Stop(EM_Send);
//				LedClose_All_ByBoardidFun();
//			Set_NeedfinishWorkBit_AllBoard_StateFun();
//			
//			ChangeOneClickCloseIconAndNboardIcon();
//			SetTaskHandlerTimeRloadAndChangeState(&Handle_SendVolDataToScreenAndAskSlave,ASK_SLAVE_NOTWORK_DUTY_TIME,1);
//			SuspendTaskHandler(Handle_CheckStop_ALLBoard_FinishWork);
//			TurnWork_led_OFF;
//			TurnFan_OFF;
//SD_STOP_FunOuter();
//			SYSTEM_getAllLightsStateCounters();
			
//SCREEN_write_cmd(EM_Icon_RunState_SwitchAll,&tmpdata,1);	

			

			
//			SYSTEM_DEBUG("\n RunState = STOP   0 state\n");
		}

//SYSTEM_INFO("\n EM_ButtonIcon_Mode_Select_1 %d  %d\n",EM_ButtonIcon_Mode_Select_1+(launchboardnum),tmpdata);
		Jump_to_now_page();
		
		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)GV_ScreenSetRunBoardState.Set_NowSingleRunState,8,"  --------1111----  ");
		
	}
	

}




//上传后跳回本页
void Jump_to_now_page(void){
	


		 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   			if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){
						
			SYSTEM_DEBUG("\n ===Jump_to_now_page==%d==\n",GV_ScreenSetRunBoardState.Set_NowShowPageNum);
						
		switch(GV_ScreenSetRunBoardState.Set_NowShowPageNum){
		case 1: GV_ScreenCommu.ScreenData =Array_Page01;  break;
		case 2: GV_ScreenCommu.ScreenData =Array_Page02;  break;
		case 3: GV_ScreenCommu.ScreenData =Array_Page03;  break;
		case 4: GV_ScreenCommu.ScreenData =Array_Page04;  break;			
			default:break;
	}
						
						
						

			}else{
					switch(GV_ScreenSetRunBoardState.Set_NowShowPageNum){
		case 1: GV_ScreenCommu.ScreenData =Array_Page01_English;  break;
		case 2: GV_ScreenCommu.ScreenData =Array_Page02;  break;
		case 3: GV_ScreenCommu.ScreenData =Array_Page03;  break;
		case 4: GV_ScreenCommu.ScreenData =Array_Page04;  break;			
			default:break;
	}
	
			}

		GV_ScreenCommu.ScreenDataSize=2;
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1

SYSTEM_INFO("\n =====OX%x==0X%X\n",GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData[1]);
}








void RunMaintainWork_AlwayIn(void){
	
//if(GV_HardwareState.HS_Serial_parallel_Delay_Nowstate==0)
{
	//判断电压达到上限了吗？？999 单体上限和总体上限，如果达到了，则返回，并进入状态2
//  MasterBoardSerialChargeOFF;

	//关并充
	
}








}







void PageMainSelect(uint8_t num,uint8_t* p_data){
//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)p_data,num,"   xxxxxxxxxx =");
	
//	SYSTEM_DEBUG("*p_data=  %x,%x",*p_data,*(p_data+1));
//		SYSTEM_DEBUG("*p_data=  %d,%d",*p_data,*(p_data+1));
	
	
	
	
	
if((*p_data==0x00)&&(*(p_data+1)==0x00)){//电压页


disposeLaunchPageIfJumptoPowerDownPage();
	

	SYSTEM_INFO("\n Jump PowerDown page \n");
	return;
}else	
if((*p_data==0x00)&&(*(p_data+1)==0x01)){//设置页

ScreenSettingParaSend();//从eeprom复制参数到设置页，修改，然后保存

	

	SYSTEM_INFO("\n Jump page setting \n");
	return;
}else
if((*p_data==0x00)&&(*(p_data+1)==0x02)){//校正页区
GV_ScreenSetRunBoardState.Set_NowShowPageRegionNum=2;
//读取数据
if(GV_ScreenSetRunBoardState.Set_NowRunState!=EM_RunState_Stop){

//机器还在启动中，请停止运行后再进行参数设置。
//		 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
//	   GV_ScreenCommu.ScreenData =Array_Page_RunningCanntSave;

//		GV_ScreenCommu.ScreenDataSize=2;
//		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面12
//	 delay_ms(1000);
//		  GV_ScreenCommu.ScreenData =Array_Page_Main;
//		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
	
	
	SYSTEM_INFO("\n machine is running ,setting is not permit\n");
	
return;
}

	

	SYSTEM_INFO("\n calibrate VOL page setting \n");
	return;
}else
if((*p_data==0x00)&&(*(p_data+1)==0x03)){//显示页

GV_ScreenSetRunBoardState.Set_NowShowPageRegionNum=3;





	

	SYSTEM_INFO("\n show page  \n");
	return;
}else
if((*p_data==0x00)&&(*(p_data+1)==0x00)){//电压页


GV_ScreenSetRunBoardState.Set_NowShowPageRegionNum=0;

	


	

	SYSTEM_INFO("\n show vol page  \n");
	return;
}else
if((*p_data==0x00)&&(*(p_data+1)==0x04)){//容量页

GV_ScreenSetRunBoardState.Set_NowShowPageRegionNum=4;

ResumeTaskHandler(&Handle_SendCapacityDataToScreen);
	


	

	SYSTEM_INFO("\n show capicity page  \n");
	return;
}else
if((*p_data==0x00)&&(*(p_data+1)==0x05)){//系统设置页

GV_ScreenSetRunBoardState.Set_NowShowPageRegionNum=5;


	ScreenSYS_SettingParaSend();


	

	SYSTEM_INFO("\n show sys page  \n");
	return;
}else
if((*p_data==0x00)&&(*(p_data+1)==0x06)){//系统设置页

GV_ScreenSetRunBoardState.Set_NowShowPageRegionNum=6;


	ScreenChip_IDParaSend();


	

	SYSTEM_INFO("\n show chipid page  \n");
	return;
}








}





//extern str_ScreenSetRunState TMP_ScreenSetState;

//extern str_EEpromSettingState TMP_EEpromSettingState;

//uint16_t 	EEP_AllUpperLimitParallelVol;//并充截止电压 mV
//uint16_t  EEP_AllUpperAlarmParallelVol;	//并充报警上限电压 mV
//uint16_t 	EEP_AllLowerLimitParallelVol;//并放截止电压 mV
//uint16_t  EEP_AllLowerAlarmParallelVol;	//并放报警上限电压 mV	

void PageSettingSave(uint8_t num,uint8_t** p_data){

SYSTEM_INFO("PageSettingSave******");
Delay(100);
VpChange16HL((uint16_t*)*p_data,num);
SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)*p_data,num,"PageSettingSave ***************");

Delay(100);


	
	
memcpy(&TMP_EEpromSettingState.EEP_Bat_SingleBattRun,*p_data,num);
	SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)&TMP_EEpromSettingState.EEP_Bat_SingleBattRun,num,"PageSettingSave2 ***************");
for(uint8_t i=0;i<8;i++){

		if(
//			(TMP_EEpromSettingState.EEP_Bat_UpperLimitParallelVol[i]<=TMP_EEpromSettingState.EEP_Bat_LowerLimitParallelVol[i]+100)||
		(TMP_EEpromSettingState.EEP_Bat_UpperLimitParallelVol[i]>=TMP_EEpromSettingState.EEP_Bat_UpperAlarmParallelVol[i])
	){
		
		SYSTEM_DEBUG("\n EEP_Bat_UpperLimitParallelVol=%d EEP_Bat_UpperAlarmParallelVol=%d,%d\n",TMP_EEpromSettingState.EEP_Bat_UpperLimitParallelVol[i],TMP_EEpromSettingState.EEP_Bat_UpperAlarmParallelVol[i],i);
		
			GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_SettingWrong;

	GV_ScreenCommu.ScreenDataSize=2;
SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
			Delay(1500);
			if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){
			
			GV_ScreenCommu.ScreenData =Array_Page_Main;
			}else{
			
			GV_ScreenCommu.ScreenData =Array_Page_Main_English;
			}
			
	   

		
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
		
		
		
	return;
	}
	
//	EEP_Bat_BattNum   memcpy(&OldGV_ScreenSettingDataState.Set_Bat_BattNum,&TMP_EEpromSettingState.EEP_Bat_BattNum,144);
	

	
//			for(uint8_t i=0;i<8;i++){
//	
//		GV_ScreenSetRunBoardState.Set_Bat_OldBattNum[i]=GV_ScreenSettingDataState.Set_Bat_BattNum[i];
//			
//		}
	
	

}

	//SYSTEM_DEBUG("\n current0=%d =%d\n",TMP_EEpromSettingState.EEP_Bat_ChargeCurrent_xmA[0],TMP_EEpromSettingState.EEP_Bat_DischargeCurrent_xmA[0]);
	//SYSTEM_DEBUG("\n current1=%d =%d\n",TMP_EEpromSettingState.EEP_Bat_ChargeCurrent_xmA[1],TMP_EEpromSettingState.EEP_Bat_DischargeCurrent_xmA[1]);
	//memcpy(&GV_BatterySetTargetState.Battery_SET_SumlimitValue,&GV_EEprom.Battery_SET_SumlimitValue,num);
		memcpy(&GV_ScreenSettingDataState.Set_Bat_SingleBattRun,*p_data,num);
	if(GV_ScreenSetRunBoardState.Set_NowRunState==1){//运行中
		SYSTEM_DEBUG("setting run-----");
for(uint8_t i=0;i<8;i++){

				SYSTEM_DEBUG("\n **** idboard %d ,Run %d, oldnum %d, nownum %d , oldmod %d, nowmod %d\n",
			i,GV_ScreenSetRunBoardState.Set_NowSingleRunState[i],
	OldGV_ScreenSettingDataState.Set_Bat_BattNum[i],GV_ScreenSettingDataState.Set_Bat_BattNum[i],
	OldGV_ScreenSettingDataState.Set_Bat_Runworkingmode[i],GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i]);
		
	
			
			
	if(((OldGV_ScreenSettingDataState.Set_Bat_BattNum[i]==0)&&(GV_ScreenSettingDataState.Set_Bat_BattNum[i]!=0))
		||((OldGV_ScreenSettingDataState.Set_Bat_BattNum[i]!=0)&&(GV_ScreenSettingDataState.Set_Bat_BattNum[i]==0))
	){
	
//							SYSTEM_DEBUG("\n **** idboard %d ,Run %d, oldnum %d, nownum %d , oldmod %d, nowmod %d\n",
//			i,GV_ScreenSetRunBoardState.Set_NowSingleRunState[i],OldGV_ScreenSettingDataState.Set_Bat_BattNum[i],GV_ScreenSettingDataState.Set_Bat_BattNum[i],
//	OldGV_ScreenSettingDataState.Set_Bat_Runworkingmode[i],GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i]);
		
			

//		ScreenJumpToAimPage(Array_Page_SettingWrong);
//			Delay(1500);
//			if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){

//				ScreenJumpToAimPage(Array_Page_Main);
//			}else{

//				ScreenJumpToAimPage(Array_Page_Main_English);
//			}
		
		ScreenJumpToAimPageAndReturnMain(Array_Page_SettingWrong);
			return;
			
			
			
	}
	
	
	if((GV_ScreenSetRunBoardState.Set_NowSingleRunState[i] ==1)&&(
		(OldGV_ScreenSettingDataState.Set_Bat_SingleBattRun[i]!=GV_ScreenSettingDataState.Set_Bat_SingleBattRun[i])||
	(OldGV_ScreenSettingDataState.Set_Bat_Run_Vol_difference[i]!=GV_ScreenSettingDataState.Set_Bat_Run_Vol_difference[i])||
	(OldGV_ScreenSettingDataState.Set_Bat_RunCount_polar[i]!=GV_ScreenSettingDataState.Set_Bat_RunCount_polar[i])||
	(OldGV_ScreenSettingDataState.Set_Bat_BattNum[i]!=GV_ScreenSettingDataState.Set_Bat_BattNum[i])||
	(OldGV_ScreenSettingDataState.Set_Bat_UpperLimitParallelVol[i]!=GV_ScreenSettingDataState.Set_Bat_UpperLimitParallelVol[i])||
	(OldGV_ScreenSettingDataState.Set_Bat_UpperAlarmParallelVol[i]!=GV_ScreenSettingDataState.Set_Bat_UpperAlarmParallelVol[i])||
	(OldGV_ScreenSettingDataState.Set_Bat_LowVoltageAlarm[i]!=GV_ScreenSettingDataState.Set_Bat_LowVoltageAlarm[i])||
	(OldGV_ScreenSettingDataState.Set_Bat_ChargeCurrent_xmA[i]!=GV_ScreenSettingDataState.Set_Bat_ChargeCurrent_xmA[i])||
	(OldGV_ScreenSettingDataState.Set_Bat_StopCapacity[i]!=GV_ScreenSettingDataState.Set_Bat_StopCapacity[i])||
	(OldGV_ScreenSettingDataState.Set_Bat_StopTime[i]!=GV_ScreenSettingDataState.Set_Bat_StopTime[i])
	)){

//GV_ScreenSettingDataState.Set_Bat_SingleBattRun[i]=OldGV_ScreenSettingDataState.Set_Bat_SingleBattRun[i];
//GV_ScreenSettingDataState.Set_Bat_Run_Vol_difference[i]=OldGV_ScreenSettingDataState.Set_Bat_Run_Vol_difference[i];
//GV_ScreenSettingDataState.Set_Bat_RunCount_polar[i]=OldGV_ScreenSettingDataState.Set_Bat_RunCount_polar[i];
//GV_ScreenSettingDataState.Set_Bat_BattNum[i]=OldGV_ScreenSettingDataState.Set_Bat_BattNum[i];
//	(OldGV_ScreenSettingDataState.Set_Bat_UpperLimitParallelVol[i]!=GV_ScreenSettingDataState.Set_Bat_UpperLimitParallelVol[i])||
//	(OldGV_ScreenSettingDataState.Set_Bat_UpperAlarmParallelVol[i]!=GV_ScreenSettingDataState.Set_Bat_UpperAlarmParallelVol[i])||
//	(OldGV_ScreenSettingDataState.Set_Bat_LowVoltageAlarm[i]!=GV_ScreenSettingDataState.Set_Bat_LowVoltageAlarm[i])||
//	(OldGV_ScreenSettingDataState.Set_Bat_ChargeCurrent_xmA[i]!=GV_ScreenSettingDataState.Set_Bat_ChargeCurrent_xmA[i])||
//	(OldGV_ScreenSettingDataState.Set_Bat_StopCapacity[i]!=GV_ScreenSettingDataState.Set_Bat_StopCapacity[i])||
//	(OldGV_ScreenSettingDataState.Set_Bat_StopTime[i]!=GV_ScreenSettingDataState.Set_Bat_StopTime[i]
		
	memcpy(&GV_ScreenSettingDataState.Set_Bat_SingleBattRun,&TMP_EEpromSettingState.EEP_Bat_SingleBattRun,48);
	memcpy(&GV_ScreenSettingDataState.Set_Bat_BattNum,&TMP_EEpromSettingState.EEP_Bat_BattNum,144);
		
//	memcpy(&OldGV_ScreenSettingDataState.Set_Bat_SingleBattRun,&TMP_EEpromSettingState.EEP_Bat_SingleBattRun,48);
//	memcpy(&OldGV_ScreenSettingDataState.Set_Bat_BattNum,&TMP_EEpromSettingState.EEP_Bat_BattNum,112);	//144
//		GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i]=OldGV_ScreenSettingDataState.Set_Bat_Runworkingmode[i];
		ScreenJumpToAimPageAndReturnMain(Array_Page_SettingWrong);
			return;
	
	}
	
	
		if((GV_ScreenSetRunBoardState.Set_NowSingleRunState[i] ==1)&&(OldGV_ScreenSettingDataState.Set_Bat_Runworkingmode[i]!=GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i])){

		
		GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i]=OldGV_ScreenSettingDataState.Set_Bat_Runworkingmode[i];
		ScreenJumpToAimPageAndReturnMain(Array_Page_SettingWrong);
			return;
	
	}
	
	
	
	
//	OldGV_ScreenSettingDataState
	
//	GV_CommSendToSlaveBoardState.Effective=0X0F;
		if(GV_ScreenSetRunBoardState.Set_NowSingleRunState[i] ==0){
	 // 清除第i位（设置为0）
                GV_CommSendToSlaveBoardState.Effective &= ~(1 << i);
	}else{
	                // 设置第i位（设置为1）
                GV_CommSendToSlaveBoardState.Effective |= (1 << i);
	
	
	}




}

}else{
SYSTEM_DEBUG("setting run off-----");
GV_CommSendToSlaveBoardState.Effective=0x0f;

}
	




	memcpy(&TMP_EEpromSettingState.EEP_Bat_SingleBattRun,&GV_ScreenSettingDataState.Set_Bat_SingleBattRun,num);


	memcpy(&GV_ScreenSetRunBoardState.Set_ChargeStateSelect,&GV_ScreenSettingDataState.Set_Bat_Runworkingmode,16);


SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_ScreenSetRunBoardState.Set_ChargeStateSelect,8,"----=====");

	SCREEN_write_cmd(EM_Icon_Mode_Select_1,GV_ScreenSetRunBoardState.Set_ChargeStateSelect,8);


//			for(uint16_t i=0;i<8;i++)
//{
//	GV_HardwareState.HW_NeedWorkingState[i]=0;

//}



//GV_HardwareState.HW_NeedWorkingState[i]
	



//	DisposeSaveSettingData();
	CopySettingDataToSlaveBuff(EM_HW_LOSEPOWER_PARA);
	
	///======================
	

	Setboadnumfun();//设置板数量
///======================




//				for(uint16_t i=0;i<0;i++){
//	comu_Sendslave_Rundata();
//		delay_ms(200);
//	}
SendCommu485data();
	SYSTEM_DEBUG("\n **********here ");
	//extern str_EEpromSettingState TMP_EEpromSettingState;
	
	StartWork_ClearBuffDataInit();
	clearScreenDataVol();
	 //memcpy(&GV_EEprom,*p_data,num);
//Delay(100);
//SYSTEM_DEBUG("Battery_SET_SumlimitValue = %d",(uint16_t)GV_EEprom.Battery_SET_SumlimitValue);
Delay(100);
SYSTEM_DEBUG("\n GV_ScreenSetRunBoardState.Set_NowShowLanguage=%d \n",GV_ScreenSetRunBoardState.Set_NowShowLanguage);
  if(SaveBoardSettingEEpromData()==0){
	 
		
		
		
		
//		 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
//		
//		
//		
//	   GV_ScreenCommu.ScreenData =Array_Page_SaveSucceed;

//		GV_ScreenCommu.ScreenDataSize=2;
//		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面12
//		
		
		ScreenJumpToAimPage(Array_Page_SaveSucceed);
		 delay_ms(1000);
		
					if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){
			
//			GV_ScreenCommu.ScreenData =Array_Page_Main;
						ScreenJumpToAimPage(Array_Page_Main);
			}else{
			
//			GV_ScreenCommu.ScreenData =Array_Page_Main_English;
				ScreenJumpToAimPage(Array_Page_Main_English);
			}
		
//		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面12
//		  GV_ScreenCommu.ScreenData =Array_Page_Main;
//		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
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
		
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面12
//		 GV_ScreenCommu.ScreenData =Array_Page_Main;
//		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
	  return;
	 }

	 
	 
	 
	 
	 
}








void PageCapPageReturn(uint8_t num,uint8_t* p_data){

SYSTEM_INFO("\n *********PageCapPageReturn *********\n");


if((*p_data==0x00)&&(*(p_data+1)==0x00)){

SuspendTaskHandler(&Handle_SendCapacityDataToScreen);

}



}



void PageSYSSettingConfirm(uint8_t num,uint8_t* p_data){

SYSTEM_INFO("\n *********PageSYSSettingConfirm *********\n");


if((*p_data==0x00)&&(*(p_data+1)==0xF1)){//点击保存上传按钮，先从屏幕某地址开始读取数据，返回后进行参数保存，
SYSTEM_INFO("\n **********f1  \n");
		//读取数据
if(GV_ScreenSetRunBoardState.Set_NowRunState!=0){
SYSTEM_INFO("\n *****machine is running ,setting is not permit\n");
//机器还在启动中，请停止运行后再进行参数设置。
//SuspendTaskHandler(&Handle_SendSDTimeDataToScreen);
			 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_RunningCanntSave;

		GV_ScreenCommu.ScreenDataSize=2;
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面15
	SYSTEM_INFO("\n machine is running ,setting is not permit\n");
	
return;
}
	

				if(ActiveMechineFailure()==1){
			return;
			
			}

	
			  GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_Uploading;

		GV_ScreenCommu.ScreenDataSize=2;
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面13（保存中）
	
		 GV_ScreenCommu.ScreenAddr = EM_SYS_SET_YEAR;///12345
	  
		GV_ScreenCommu.ScreenDataSize=EEPROM_SYS_SettingSaveDataCount/2+6;//12345
		SCREEN_read_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenDataSize);//


	//ScreenParameter_SettingReturnCopyMomery(&GV_BatterySetTargetReturn,&GV_EEprom);
	

	
	
	SYSTEM_DEBUG("\n PageSYSSettingConfirm *********** \n");
	

	
	

}else if((*p_data==0x00)&&(*(p_data+1)==0x00)){

SuspendTaskHandler(&Handle_SendSDTimeDataToScreen);

SYSTEM_DEBUG("\n SuspendTaskHandler *********** \n");


}











	
	
	


}



//PagePowerDownConfirm
void PagePowerDownConfirm(uint8_t num,uint8_t* p_data){
	
//Jump_to_now_page();
if((*p_data==0x00)&&(*(p_data+1)==0x01)){//点击保存上传按钮，先从屏幕某地址开始读取数据，返回后进行参数保存，
SYSTEM_INFO("\n PagePowerDownConfirm  \n");

	
	
	
	
ResumeLaunchPowerDownFun();
	
	
	
}else if((*p_data==0x00)&&(*(p_data+1)==0x00)){

CancelLaunchPowerDownFun();

}


GV_ScreenSetRunBoardState.Set_NowShowPageNum=1;
Jump_to_now_page();




}















void PageSettingConfirm(uint8_t num,uint8_t* p_data){
	SYSTEM_INFO("\n *********PageSettingConfirm *********\n");

//读取数据
//if(GV_ScreenSetRunBoardState.Set_NowRunState!=0){

////机器还在启动中，请停止运行后再进行参数设置。

//			 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
//	   GV_ScreenCommu.ScreenData =Array_Page_RunningCanntSave;

//		GV_ScreenCommu.ScreenDataSize=2;
//		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面15
//	SYSTEM_INFO("\n machine is running ,setting is not permit\n");
//	
//return;
//}

if((*p_data==0x00)&&(*(p_data+1)==0xF1)){//点击保存上传按钮，先从屏幕某地址开始读取数据，返回后进行参数保存，

			  GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_Uploading;

		GV_ScreenCommu.ScreenDataSize=2;
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面13（保存中）
	
		 GV_ScreenCommu.ScreenAddr = EM_SaveSettingFrom;///12345
	  
		GV_ScreenCommu.ScreenDataSize=EEPROM_SettingSaveDataCount/2;//12345
		SCREEN_read_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenDataSize);//
SYSTEM_DEBUG("\n PageSettingConfirm *********** 0x%x   %d\n",GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenDataSize);

	//ScreenParameter_SettingReturnCopyMomery(&GV_BatterySetTargetReturn,&GV_EEprom);
	

	
	

	

}else if((*p_data==0x00)&&(*(p_data+1)==0x01)){//点击保存上传按钮，先从屏幕某地址开始读取数据，返回后进行参数保存，

			SYSTEM_DEBUG("\n PageSetting  fuwei *********** \n");
	
	memset_uint16_Small(&GV_ScreenSettingDataState.Set_Bat_BattNum[0], 0x0000, 8);
	
	//SYSTEM_DEBUG("\n **** --- %d \n",GV_ScreenSettingDataState.Set_BoardBat[0]);
	memset_uint16_Small(&GV_ScreenSettingDataState.Set_Bat_UpperLimitParallelVol[0], (uint16_t)3600, 8);
	
	
	memset_uint16_Small(GV_ScreenSettingDataState.Set_Bat_UpperAlarmParallelVol, (uint16_t)4200, 8);	

	

	memset_uint16_Small(GV_ScreenSettingDataState.Set_Bat_ChargeCurrent_xmA, (uint16_t)30, 8);	

	memset_uint16_Small(GV_ScreenSettingDataState.Set_Bat_StopCapacity, (uint16_t)9999, 8);	
	memset_uint16_Small(GV_ScreenSettingDataState.Set_Bat_StopTime, (uint16_t)999, 8);	
	memset_uint16_Small(GV_ScreenSettingDataState.Set_Bat_Runworkingmode, (uint16_t)0, 8);
	memset_uint16_Small(GV_ScreenSettingDataState.Set_Bat_LowVoltageAlarm, (uint16_t)1000, 8);
	memset_uint16_Small(GV_ScreenSettingDataState.Set_Bat_Run_Vol_difference, (uint16_t)10, 8);
	
memset_uint16_Small(GV_ScreenSettingDataState.Set_Bat_SingleBattRun, (uint16_t)0x4FFF, 8);	
	//memcpy(&GV_ScreenSettingDataState.Set_BoardCount,&TMP_EEpromSettingState.EEP_BoardCount,EEPROM_SettingSaveDataCount);
	
	SCREEN_write_cmd_print(EM_SingleBattRun01,GV_ScreenSettingDataState.Set_Bat_SingleBattRun,EEPROM_SettingSaveDataCount/2,0);


	



	//ScreenParameter_SettingReturnCopyMomery(&GV_BatterySetTargetReturn,&GV_EEprom);
	

	
	
	SYSTEM_DEBUG("\n PageSettingConfirm *********** \n");
	

}









}





void VolRectifyConfirm(uint8_t num,uint8_t* p_data){
	SYSTEM_INFO("\n *********VolRectifyConfirm *********\n");

////读取数据
//if(GV_ScreenSetRunBoardState.Set_NowRunState!=0){

////机器还在启动中，请停止运行后再进行参数设置。

//	
//	SYSTEM_INFO("\n machine is running ,setting is not permit\n");
//	
//return;
//}


	GV_ScreenAllCalibration.Rectify_data_integrality=0;
	
	
	
if(*p_data==0x00){

			  GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_Uploading;

		GV_ScreenCommu.ScreenDataSize=2;
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面13
	GV_ScreenAllCalibration.Rectify_pagenum=(*(p_data+1));
		 GV_ScreenCommu.ScreenAddr = EM_rectifyShowChargeCurrent01_01+GV_ScreenAllCalibration.Rectify_pagenum*EM_RECTIFY_JUMP;
	  
		GV_ScreenCommu.ScreenDataSize=52;
		SCREEN_read_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenDataSize);//
	  delay_ms(200);//空闲中断时间
		 GV_ScreenCommu.ScreenAddr = EM_rectifyShowChargeCurrent01_01+52+GV_ScreenAllCalibration.Rectify_pagenum*EM_RECTIFY_JUMP;
	  
		GV_ScreenCommu.ScreenDataSize=66-52;
		SCREEN_read_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenDataSize);//

	//ScreenParameter_SettingReturnCopyMomery(&GV_BatterySetTargetReturn,&GV_EEprom);
	

	
	
	SYSTEM_DEBUG("\n VolRectifyConfirm *********** \n");
	

	
	

}

}


void VolRectifyHeadSave(uint8_t num,uint8_t** p_data)
{



VpChange16HL((uint16_t*)*p_data,num);
SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)*p_data,num,"VolRectifyHeadSave ***************");

 //memcpy(&GV_EEprom,*p_data,num);

//GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCur[0]=(uint16_t)(*p_data);

	//memcpy(&GV_BatterySetTargetState.Battery_SET_SumlimitValue,&GV_EEprom.Battery_SET_SumlimitValue,num);
		memcpy(&GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCur[0],*p_data,num);
	GV_ScreenAllCalibration.Rectify_data_integrality=1;
//	SlaveCalibrationcalculate();
	//StructCopyAndVpReserve((uint8_t *)&BigD_GV_CommSlaveBoardState,(uint8_t *)&GV_CommSendToSlaveBoardState,sizeof(GV_CommSendToSlaveBoardState),BigD_GV_CommSlaveBoardState.StopParalleltChargeVol,16*12);
	
//	StructOnlyVpReserve((uint8_t *)&BigD_GV_BatterySetCalibration.BX_SetCCurrent[0],120);
//	comu_SendslaveRectifydata();
	
//Delay(100);
//SYSTEM_DEBUG("Battery_SET_SumlimitValue = %d",(uint16_t)GV_EEprom.Battery_SET_SumlimitValue);
//Delay(100);
//comu_SendslaveRectifydata();
//  if(1){
//	 
//		 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
//	   GV_ScreenCommu.ScreenData =Array_Page_SaveSucceed;

//		GV_ScreenCommu.ScreenDataSize=2;
//		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面12
////		 delay_ms(1000);
////		  GV_ScreenCommu.ScreenData =Array_Page_Main;
////		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
//		 return;
//	 }else{
//	 
//	 		 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
//	   GV_ScreenCommu.ScreenData =Array_Page_SaveFailure;

//		GV_ScreenCommu.ScreenDataSize=2;
//		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面13
////		 delay_ms(1000);
////		 GV_ScreenCommu.ScreenData =Array_Page_Main;
////		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
//	  return;
//	 }

	 
}



void VolRectifyTailSave(uint8_t num,uint8_t** p_data)
{



VpChange16HL((uint16_t*)*p_data,num);
SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)*p_data,num,"VolRectifyTailSave ***************");

 //memcpy(&GV_EEprom,*p_data,num);

//GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_ShowChargeCur[0]=(uint16_t)(*p_data);

	//memcpy(&GV_BatterySetTargetState.Battery_SET_SumlimitValue,&GV_EEprom.Battery_SET_SumlimitValue,num);
		memcpy(&GV_ScreenAllCalibration.S_SingleCalibration[GV_ScreenAllCalibration.Rectify_pagenum].S_Rectify_TrueVoltage[0],*p_data,num);
	
	if(GV_ScreenAllCalibration.Rectify_data_integrality!=1){
	GV_ScreenAllCalibration.Rectify_data_integrality=0;
			GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_SaveFailure;

		GV_ScreenCommu.ScreenDataSize=2;
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面13

	  return;

	}
	
	
	
	SlaveCalibrationcalculate();
	
		//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_BatterySetCalibration.BX_SetCCurrent,24,"*****cde  b **");
	
	
	StructCopyAndVpReserve((uint8_t *)&BigD_GV_BatterySetCalibration,(uint8_t *)&GV_BatterySetCalibration,sizeof(GV_BatterySetCalibration),BigD_GV_BatterySetCalibration.BX_DisplaySampleChargeCurrent,120);

	
	///StructOnlyVpReserve((uint8_t *)BigD_GV_BatterySetCalibration.BX_SetCCurrent,120);
	//VpChange16HL(BigD_GV_BatterySetCalibration.BX_SetCCurrent,120);
	//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)BigD_GV_BatterySetCalibration.BX_SetCCurrent,24,"*****cde  bb **");
	
	
	comu_SendslaveRectifydata();
	GV_ScreenAllCalibration.Rectify_data_integrality=0;
//Delay(100);
//SYSTEM_DEBUG("Battery_SET_SumlimitValue = %d",(uint16_t)GV_EEprom.Battery_SET_SumlimitValue);
//Delay(100);
//comu_SendslaveRectifydata();

//SYSTEM_DEBUG("success **12345***** 1789");
  
//	 delay_ms(500);
		 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_SaveSucceed;

		GV_ScreenCommu.ScreenDataSize=2;
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面12
//		 delay_ms(1000);
//		SYSTEM_DEBUG("success ******* 3789");
				 delay_ms(1000);
		
					if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){
//			SYSTEM_DEBUG("success ******* 1789");
			GV_ScreenCommu.ScreenData =Array_Page_Main;
			}else{
			
			GV_ScreenCommu.ScreenData =Array_Page_Main_English;
			}
//			SYSTEM_DEBUG("success ******* 178*9");
		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
			
//			SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);
//			SYSTEM_DEBUG("success ******* 7-89");
			
		 return;
			
//		if(1){	
//	 }else{//失败
//	 
//	 		 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
//	   GV_ScreenCommu.ScreenData =Array_Page_SaveFailure;

//		GV_ScreenCommu.ScreenDataSize=2;
//		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面13
//				 delay_ms(1000);
//		
//					if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){
//			
//			GV_ScreenCommu.ScreenData =Array_Page_Main;
//			}else{
//			
//			GV_ScreenCommu.ScreenData =Array_Page_Main_English;
//			}
//		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面1
//	  return;
//	 }

	 
}




void MechineActiveConfirm(uint8_t num,uint8_t* p_data){

//VpChange16HL((uint16_t*)*p_data,num);
//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)*p_data,num,"MechineActiveConfirm ***************");
if((*p_data==0x00)&&(*(p_data+1)==0xF1)){//点击保存上传按钮，先从屏幕某地址开始读取数据，返回后进行参数保存，

			  GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	   GV_ScreenCommu.ScreenData =Array_Page_Uploading;

		GV_ScreenCommu.ScreenDataSize=2;
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面13（保存中）
	
		 GV_ScreenCommu.ScreenAddr = EM_SEQUENCE_ENTERNEW_NUMBER;///12345
	  
		GV_ScreenCommu.ScreenDataSize=16;//12345
		SCREEN_read_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenDataSize);//
SYSTEM_DEBUG("\n MechineActiveConfirm *********** 0x%x   %d\n",GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenDataSize);


}

}

void MechineActiveSave(uint8_t num,uint8_t** p_data){

uint8_t checkis=0;
//VpChange16HL((uint16_t*)*p_data,num);
SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)*p_data,num,"MechineActiveSave ***************%d",num);
//	delay_ms(10);
SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,(uint8_t*)*p_data,num,"MechineActiveSave3 \n");
//	delay_ms(10);
//SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,(uint8_t*)*p_data,num,"MechineActiveSave2 \n");

checkis=checkIsRightChipData( num, p_data);
//	delay_ms(10);
	SYSTEM_DEBUG("chip id MechineActiveSave ");
		 		 GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
	  

		GV_ScreenCommu.ScreenDataSize=2;
	if(checkis==0){
		
	 GV_ScreenCommu.ScreenData =Array_Page_ActiveSuccess;
	
	}else{
	
	 GV_ScreenCommu.ScreenData =Array_Page_ActiveFailure;
	
	}
	

		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面13
		 delay_ms(2000);
		 
		 					if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){
			
			GV_ScreenCommu.ScreenData =Array_Page_Main;
			}else{
			
			GV_ScreenCommu.ScreenData =Array_Page_Main_English;
			}
		
		SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面12

}







void ScreenJumpToMainPage(void){

GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
		GV_ScreenCommu.ScreenDataSize=2;
		 GV_ScreenCommu.ScreenData =Array_Page_Language;
		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面7
SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize*2,"*****JUMP TO****pagemain *******");
}



void ScreenJumpToAimPage(uint16_t*  ScreenData){

GV_ScreenCommu.ScreenAddr = EM_PAGE_ADDRESS;
		GV_ScreenCommu.ScreenDataSize=2;
		 GV_ScreenCommu.ScreenData =ScreenData;
		 SCREEN_write_cmd(GV_ScreenCommu.ScreenAddr,GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize);//切换页面7
SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_ScreenCommu.ScreenData,GV_ScreenCommu.ScreenDataSize*2,"*****JUMP TO****pagemain *******");
	
	
}



//			ScreenJumpToAimPage(Array_Page_SettingWrong);
//			Delay(1500);
//			if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){

//				ScreenJumpToAimPage(Array_Page_Main);
//			}else{

//				ScreenJumpToAimPage(Array_Page_Main_English);
//			}

void ScreenJumpToAimPageAndReturnMain(uint16_t*  ScreenData){

			ScreenJumpToAimPage(ScreenData);
			Delay(1500);
			if(GV_ScreenSetRunBoardState.Set_NowShowLanguage==0){

				ScreenJumpToAimPage(Array_Page_Main);
			}else{

				ScreenJumpToAimPage(Array_Page_Main_English);
			}
	
//	return 1;
}





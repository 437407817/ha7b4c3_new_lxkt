#include "./screen/ScreenDataDis.h"

#include "./sys/bsp_systime.h"   

#include "./global/GV_variable.h" 
#include "./screen/DW_com_send.h"

#include "./io/bsp_io_output.h" 

#include "./DataConvert/data_convert.h"
#include <stdlib.h>
//#include "./usart/rx_data_queue.h"

 #include "./sys/sysio.h"
#include "./screen/Screen.h"
#include "./controller/StateFun.h"
#include "./service/EepromFun.h"

#include "./screen/ScreenSendFun.h"
#include "./controller/DisStateFun.h"
#include "./service/EepromFun.h"
#include "./usart/p_data_queue.h"

#include "./service/DisposeDataFun.h"
#include "./controller/TaskFun.h"
#include "./controller/TaskSingleWork.h"
#include "./usart/bsp_usart_sd.h"
#include "./screen/ScreenDS1302.h"

#include "./crypt/bsp_encode.h"



extern str_Screen_SYS_SettingDataState Screen_SYS_SettingDataState;
extern str_ScreenSettingDataState OldGV_ScreenSettingDataState;
extern str_EEpromSettingState TMP_EEpromSettingState;
extern str_EEprom_SYS_SettingDataState TMP_EEprom_SYS_SettingState;

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
//extern str_ScreenSetRunState OldGV_ScreenSetRunBoardState;



extern str_ScreenSettingDataState GV_ScreenSettingDataState;
extern str_ComuBoardState GV_ComuBoardState;

extern str_ScreenSetRunState GV_ScreenSetRunBoardState;
extern str_CommSendToSlaveBoardState GV_CommSendToSlaveBoardState;

extern str_HardwareState GV_HardwareState; 


extern QUEUE_DATA_BUFF GV_qdf_1;
extern uint16_t Array_WorkType_NowMessage[4];
extern uint16_t Array_WorkType_NowMessage_English[ETE_Size];





void screendataswitch(uint16_t addr,uint8_t num,uint8_t** p_data){
SYSTEM_DEBUG_ARRAY_MESSAGE(*p_data,num,"   screen_ADDR =%x ",addr);
	//SYSTEM_DEBUG("Battery_SET_SumlimitValue p_data = %p",(uint8_t*)*p_data);
	
switch(addr){

	case EM_Button_Mode_Select:   MaintainState_Select(num,*p_data); break;

	case EM_Button_RunState_Switch01:      RunState_Switch(num,*p_data,0); break;//运行
		case EM_Button_RunState_Switch02:      RunState_Switch(num,*p_data,1); break;//运行
		case EM_Button_RunState_Switch03:      RunState_Switch(num,*p_data,2); break;//运行
		case EM_Button_RunState_Switch04:      RunState_Switch(num,*p_data,3); break;//运行
	
	
	
			case EM_Button_RunState_SwitchAll:      RunState_SwitchAll(num,*p_data); break;//运行
  case EM_Address_Page_Select:   PageMainSelect(num,*p_data);    break;//主页面选择
	
  case EM_PageSettingConfirm:    PageSettingConfirm(num,*p_data);  break;	
	
	case EM_PagePowerDown:    PagePowerDownConfirm(num,*p_data);  break;	
	
	case EM_SaveSettingFrom: PageSettingSave(num,p_data); break;
	
	
	case EM_CapPageReturn:   PageCapPageReturn(num,*p_data);  break;	
	case EM_PageSYSSettingConfirm:    PageSYSSettingConfirm(num,*p_data);  break;

	case EM_VolRectifyConfirm: VolRectifyConfirm(num,*p_data); break;
	
	
	case EM_ActiveConfirm: MechineActiveConfirm(num,*p_data); break;
	case EM_SEQUENCE_ENTERNEW_NUMBER:   MechineActiveSave(num,p_data);  break;
	
	
	case EM_rectifyShowChargeCurrent01_01: VolRectifyHeadSave(num,p_data); break;
	case EM_rectifyTrueVol01_01: VolRectifyTailSave(num,p_data); break;
	
	case EM_rectifyShowChargeCurrent02_01: VolRectifyHeadSave(num,p_data); break;
	case EM_rectifyTrueVol02_01: VolRectifyTailSave(num,p_data); break;
	
	case EM_rectifyShowChargeCurrent03_01: VolRectifyHeadSave(num,p_data); break;
	case EM_rectifyTrueVol03_01: VolRectifyTailSave(num,p_data); break;	
	
	case EM_rectifyShowChargeCurrent04_01: VolRectifyHeadSave(num,p_data); break;
	case EM_rectifyTrueVol04_01: VolRectifyTailSave(num,p_data); break;
	

	
	case EM_SYS_SET_YEAR:    PageSysTimeSettingSave(num,p_data);  break;
	
	
	case EM_Button_Language_Select: *(*p_data+1)==0?(GV_ScreenSetRunBoardState.Set_NowShowLanguage=0):(GV_ScreenSetRunBoardState.Set_NowShowLanguage=1); 
//	SYSTEM_DEBUG("\n GV_ScreenSetRunBoardState.Set_NowShowLanguage=%d %d\n",GV_ScreenSetRunBoardState.Set_NowShowLanguage,*(*p_data+1));
	break;
	
	
	default:  break;

}

}









/**
  * @brief  从系统内存复制24电压到屏幕内存
  * @param  
  * @retval 无
  */
void ScreenMemoryVolCopy(){

}


/**
* @brief  复制数据 ===发送电压，图标，参数数据（时刻发送）
  * @param  
  * @retval 无
  */
//static uint32_t starttime2=0,overtime2=0;
void ScreenSendVolDataToScreen(void){
//	Calculate_StartRunTime(&starttime2);
	//SYSTEM_DEBUG("ScreenSendVolDataToScreen---- \n");
CopyVolDataToScreenStruct();
calculateVolDataFun();
	//GV_HardwareState.batterystate[ics].SumParallel_vol_Value
//	Runwhile_CheckStop();	
	CheckErrorStop();
//	SYSTEM_INFO("ScreenSendVolDataToScreen 2  %d \n",Calculate_diffRunTime(&starttime2,&overtime2));
//for(uint8_t ici=0;ici<=GV_CommSendToSlaveBoardState.BoardNum;ici++){
for(uint8_t ici=0;ici<GV_HardwareState.hard_board_count;ici++){	
	GV_ComuBoardState.screenBatVolComuState[ici].SC_SUMvol=GV_HardwareState.batterystate[ici].SumParallel_vol_Value;
	#if 1
	GV_ComuBoardState.screenBatVolComuState[ici].SC_Maxvol=GV_HardwareState.batterystate[ici].Max_vol_Value;
	GV_ComuBoardState.screenBatVolComuState[ici].SC_Minvol=GV_HardwareState.batterystate[ici].Min_vol_Value;
	GV_ComuBoardState.screenBatVolComuState[ici].SC_MaxDiffvol=GV_HardwareState.batterystate[ici].MaxDiff_vol_Value;
	#endif
//	if(GV_ScreenSetRunBoardState.Set_NowShowPageNum==ici+1){

//SYSTEM_DEBUG("%d SumPvoltemp %d %d %d %d ",ici,GV_ComuBoardState.screenBatVolComuState[ici].SC_SUMvol,GV_ComuBoardState.screenBatVolComuState[ici].SC_Maxvol,
//	GV_ComuBoardState.screenBatVolComuState[ici].SC_Minvol,		GV_ComuBoardState.screenBatVolComuState[ici].SC_MaxDiffvol);


	SCREEN_write_cmd(EM_VOL_01_01+EM_VOL_JUMP*ici,GV_ComuBoardState.screenBatVolComuState[ici].SC_Vol,sizeof(str_ScreenBatVolComuState)/2);
//		SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)GV_ComuBoardState.screenBatVolComuState[ici].SC_Vol,sizeof(str_ScreenBatVolComuState)," ***-*--*-  Usart_SendArray data =%d",ici);
//	}


}
//SYSTEM_INFO("ScreenSendVolDataToScreen 3  %d \n",Calculate_diffRunTime(&starttime2,&overtime2));
//SYSTEM_DEBUG(" *9*9*9* 0x%x 0x%x",GV_ComuBoardState.screenBatVolComuState[0].SC_Maxvol,GV_ComuBoardState.screenBatVolComuState[0].SC_Minvol);
//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)GV_ComuBoardState.screenBatVolComuState[0].SC_Vol,sizeof(str_ScreenBatVolComuState)," ***-*--*-  Usart_SendArray data =");

//SYSTEM_DEBUG(" SumParallel_vol_Value == 0x%d  %d",GV_ComuBoardState.screenBatVolComuState[0].SC_SUMvol,sizeof(str_ScreenBatVolComuState));
memcpy(&GV_ComuBoardState.commu_MachineStateMessage,&Array_WorkType_NowMessage,4*2);
memcpy(&GV_ComuBoardState.commu_MachineStateMessage_English,&Array_WorkType_NowMessage_English,ETE_Size*2);
//SYSTEM_INFO("ScreenSendVolDataToScreen 4  %d \n",Calculate_diffRunTime(&starttime2,&overtime2));

//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)GV_ComuBoardState.commu_MachineStateMessage,4*2," commu_MachineStateMessage-------");
//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)GV_ComuBoardState.commu_MachineStateMessage_English,ETE_Size*2," commu_MachineStateMessage_English-------");
//SCREEN_write_cmd(EM_ALL_ErrorBoardNo,&GV_ComuBoardState.commu_ErrorBatteryNo,10);//总板故障
SCREEN_write_cmd(EM_ALL_ErrorBoardNo,&GV_ComuBoardState.commu_ErrorBoardNo,13);//总板故障
//SYSTEM_INFO("ScreenSendVolDataToScreen 5  %d \n",Calculate_diffRunTime(&starttime2,&overtime2));
}










/**
* @brief  发送设置数据（点开设置按钮只发一次）
  * @param  
  * @retval 无
  */
void ScreenSettingParaSend(void){

	
	GetBoardSettingEEpromData();
	//GV_ScreenSetRunBoardState.Set_AllUpperLimitParallelVol
//	CopyEEpromDatatoSettingData();
//	memcpy(&GV_ScreenSettingDataState.Set_Bat_SingleBattRun,&TMP_EEpromSettingState.EEP_Bat_SingleBattRun,EEPROM_SettingSaveDataCount);
	memcpy(&GV_ScreenSettingDataState.Set_Bat_SingleBattRun,&TMP_EEpromSettingState.EEP_Bat_SingleBattRun,48);
	memcpy(&GV_ScreenSettingDataState.Set_Bat_BattNum,&TMP_EEpromSettingState.EEP_Bat_BattNum,144);
	
	memcpy(&OldGV_ScreenSettingDataState.Set_Bat_SingleBattRun,&TMP_EEpromSettingState.EEP_Bat_SingleBattRun,48);
	memcpy(&OldGV_ScreenSettingDataState.Set_Bat_BattNum,&TMP_EEpromSettingState.EEP_Bat_BattNum,112);	//144
for(uint16_t i=0;i<4;i++){
//	if(GV_ScreenSetRunBoardState.Set_NowSingleRunState[i] ==0){
	
	OldGV_ScreenSettingDataState.Set_Bat_Runworkingmode[i]=GV_ScreenSettingDataState.Set_Bat_Runworkingmode[i];
//	}

}
	
	//Setboadnumfun();//设置板数量
	SCREEN_write_cmd(EM_SaveSettingFrom,GV_ScreenSettingDataState.Set_Bat_SingleBattRun,EEPROM_SettingSaveDataCount/2);
	
	void* frame=&GV_ScreenSettingDataState.Set_Bat_SingleBattRun;
	
	
		SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,EEPROM_SettingSaveDataCount," void--------   Usart_SendArray 82 write TO SCREEN =");
	
   frame=NULL;
}

/**
  * @brief  从串口数据中取出数据
  * @param  frame：整个数据指针 length：整个数据指针长度 Data处理后截取的数据 addr数据中第一个地址 Datanum数据中有多少字节
  * @retval 无
  */
void SCREEN_copy_data_from_queue(QUEUE_DATA_BUFF *qdf,uint8_t **frame,uint16_t* length,uint8_t** Data,uint8_t* Datanum,uint16_t* addr){
	
	Q_QUEUE_DATA_TYPE *rx_data;	
	/*从缓冲区读取数据，进行处理，*/
	rx_data = p_cbRead(&(qdf)->q_rx_queue); 

	if(rx_data != NULL)//缓冲队列非空
	{		
		*length=rx_data->q_len;

		*frame = (uint8_t *)malloc(rx_data->q_len);
		
    memcpy(*frame,rx_data->q_head,rx_data->q_len);
		//*(*frame+rx_data->q_len) = '\0';

		
	
		//SYSTEM_DEBUG_ARRAY((uint8_t*)*frame,rx_data->q_len);
//		if((frame[0][0]!=0x5A)||(frame[0][1]!=0xA5)||(frame[0][3]!=0x83)){
//			*Datanum=0;
//			SYSTEM_ERROR("screen data is not 5A A5 83 \n");
//			p_cbReadFinish(&(qdf)->q_rx_queue);
//	return;
//		}
//				if(frame[0][2]!=(rx_data->q_len)-3){
//					*Datanum=0;
//			SYSTEM_ERROR("screen data num is not right\n");
//			p_cbReadFinish(&(qdf)->q_rx_queue);
//	return;
//		}
				
					if(DW_SCREEN_ReadfromQ_dataFromatVerify(frame,*length)==1){
		
			*Datanum=0;
			
			SYSTEM_DEBUG_ARRAY_MESSAGE(*frame,*length,"get wrong data is ");
			//SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)frame,*length,"wrong data is ");//wrong
			p_cbReadFinish(&(qdf)->q_rx_queue);
		return;
		}
		
		
		
		
		
		
		//SYSTEM_DEBUG("11 frame[4]: %02x frame[5]: %02X  ",(uint8_t)*(*frame+4),(uint8_t)*(*frame+5));//正确
		//SYSTEM_DEBUG("22 frame[4]: %02x frame[5]: %02x  ",(uint8_t)frame[0][4],(uint8_t)frame[0][5]);//正确
		*addr=((uint16_t)*(*frame+4)<<8)|frame[0][5];
		//SYSTEM_DEBUG(" *addr: %04x   ",*addr);
		
	#if (DW_CRC16==1)	
	*Datanum =*length-9;
	#else	
	*Datanum =*length-7;
	#endif	

		//SYSTEM_DEBUG_ARRAY((uint8_t*)*frame,rx_data->q_len);
		*Data=(uint8_t *)malloc(*Datanum );
				if(Data == NULL) {
				SYSTEM_ERROR("memory malloc wrong\n");
					p_cbReadFinish(&(qdf)->q_rx_queue);
		return;
	}
		// SYSTEM_DEBUG("frame ....address：%p ",*frame+7);

		memcpy(*Data,*frame+7,*Datanum );

	
	
	//SYSTEM_DEBUG(" D_addr: 0x%x__D_length: %d",*addr,*length);
		//SYSTEM_DEBUG_ARRAY((uint8_t*)*Data,*length-9);
	
	
//		SYSTEM_DEBUG("received data：%s  ,length: %d",*frame,rx_data->q_len);
//		SYSTEM_DEBUG_ARRAY((uint8_t*)*frame,rx_data->q_len);

		//使用完数据必须调用cbReadFinish更新读指针
		p_cbReadFinish(&(qdf)->q_rx_queue);
	}
}




void pull_data_from_screen(void)
{
	
uint8_t *frame = NULL;
uint8_t* f_data=NULL;	
uint16_t s_leng=0;//必须为0
uint16_t s_addr=0;
uint8_t s_num = 0;
//copy_data_from_queue(&frame,&leng);
	//SCREEN_DataSolveDV_cmd(&s_addr,&f_data,leng,frame);
	
	
			SCREEN_copy_data_from_queue(&GV_qdf_1,&frame,&s_leng,&f_data,&s_num,&s_addr);
	if(s_num!=0){
		SYSTEM_DEBUG("received data: %s  ,length: %d",frame,s_leng);
		//SYSTEM_DEBUG("received data：s_addr %x  ,length: %d",s_addr,s_num);
			 
		SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t*)frame,s_leng,"screen data : s_addr %x  ,length: %d",s_addr,s_num);
		
		
		screendataswitch(s_addr,s_num,&f_data);



	}	
		free(frame);
	free(f_data);
	f_data= NULL;
	frame = NULL;
	s_leng=0;

}






//MaintainState_Serial(void)



















/**
  * @brief  设置屏幕电池并充图标位
  * @param  
  * @retval 无
  */
void Screen_SetParrleChargeIconState(void){
	

}









/**
* @brief  发送屏幕电池电压和图标状态
  * @param  
  * @retval 无
  */

void ScreenSendALLVolToDis(void){


	
}


void ScreenSendChargeMessageToDis(void){




}





/*********************************************END OF FILE**********************/

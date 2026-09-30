#ifndef __SCREENDATADIS_H
#define	__SCREENDATADIS_H


#include "stm32_FH_xxx.h"

//#include "./usart/p_data_queue.h"

#define EM_VOL_JUMP 0x0040
#define EM_RECTIFY_JUMP 0x0050



enum SCREEN_DataAddress{

EM_PAGE_ADDRESS=0x0084,
	
EM_VOL_01_01=0x6001,
EM_VOL_01_02,
EM_VOL_01_03,	
EM_VOL_01_04,
EM_VOL_01_05,
EM_VOL_01_06,
EM_VOL_01_07,
EM_VOL_01_08,	
EM_VOL_01_09,
EM_VOL_01_10,
EM_VOL_01_11,	
EM_VOL_01_12,		
	
EM_CUR_01_01=0x600D,
EM_CUR_01_02,
EM_CUR_01_03,	
EM_CUR_01_04,
EM_CUR_01_05,
EM_CUR_01_06,
EM_CUR_01_07,
EM_CUR_01_08,	
EM_CUR_01_09,
EM_CUR_01_10,
EM_CUR_01_11,	
EM_CUR_01_12,

EM_CAPACITY_01_01=0x6019,



EM_01_CHARGE_STATE=0x6025,
EM_01_DISCHARGE_STATE=0x6026,
EM_01_COMPLATE_STATE=0x6027,

EM_01_SUMVOL=0x6028,
EM_01_MAXVOL=0x6029,
EM_01_MINVOL=0x602a,
EM_01_DIFFVOL=0x602b,
//EM_01_RUNINTTIME=0x602c,
//EM_01_RUNcount=0x602d,


EM_01_ERRORNUM=0x602c,
EM_01_ERROR_STATE=0x602d,
EM_01_ERROR_ENGLISH_STATE=0x6032,
EM_01_BitControl=0x603c,//B0正反须


EM_VOL_02_01=(0x6001+EM_VOL_JUMP),

EM_VOL_03_01=(0x6001+EM_VOL_JUMP*2),

EM_VOL_04_01=(0x6001+EM_VOL_JUMP*3),







//EM_01_WORK_STATE=0x601F,



//EM_CAPACITY_01_01=0x6501,

//EM_CAPACITY_02_01=0x6501+12*2,
//EM_ENERGY_02_01=0x6501+12*3,
//EM_CAPACITY_03_01=0x6501+12*4,
//EM_ENERGY_03_01=0x6501+12*5,
//EM_CAPACITY_04_01=0x6501+12*6,
//EM_ENERGY_04_01=0x6501+12*7,
//EM_CAPACITY_05_01=0x6501+12*8,
//EM_ENERGY_05_01=0x6501+12*9,
//EM_CAPACITY_06_01=0x6501+12*10,
//EM_ENERGY_06_01=0x6501+12*11,
//EM_CAPACITY_07_01=0x6501+12*12,
//EM_ENERGY_07_01=0x6501+12*13,
//EM_CAPACITY_08_01=0x6501+12*14,
//EM_ENERGY_08_01=0x6501+12*15,

//EM_CAPACITY_09_01=0x6501+12*16,
//EM_ENERGY_09_01=0x6501+12*17,
//EM_CAPACITY_10_01=0x6501+12*18,
//EM_ENERGY_10_01=0x6501+12*19,
//EM_CAPACITY_11_01=0x6501+12*20,
//EM_ENERGY_11_01=0x6501+12*21,
//EM_CAPACITY_12_01=0x6501+12*22,
//EM_ENERGY_12_01=0x6501+12*23,


EM_ALL_ErrorBoardNo=0x6F00,
EM_ALL_ErrorType=0x6F01,
EM_ALL_ErrorType_English=0x6F05,

EM_Button_Mode_Select=0x7001,//维护，串，并切换按钮
//EM_Icon_RunState_Switch=0x7002,//启动，停止，暂停指示图标
EM_Button_RunState_Switch=0x7003,//启动，停止，暂停按钮
EM_Address_Page_Select=0x7004,//主页面的几个按钮跳转，不同值代表不同页
EM_PageSettingConfirm=0x7005,//点击设置确认保存按钮
EM_VolRectifyConfirm=0x7006,//点击校正确认保存按钮

EM_PageNum=0x7007,//跳转那一页

EM_CapPageReturn=0x7008,//跳转那一页


EM_PageSYSSettingConfirm=0x7009,//系统设置保存
EM_PagePowerDown=0x700A,

EM_Icon_Mode_Select_1=0x7011,//并充，并放，维护切换模式图标
EM_Icon_Mode_Select_2=0x7012,//并充，并放，维护切换模式图标
EM_Icon_Mode_Select_3=0x7013,//并充，并放，维护切换模式图标
EM_Icon_Mode_Select_4=0x7014,//并充，并放，维护切换模式图标




EM_Icon_RunState_START01=0x7019,//启动，停止，指示图标
EM_Icon_RunState_START02=0x701a,//启动，停止，指示图标
EM_Icon_RunState_START03=0x701b,//启动，停止，指示图标
EM_Icon_RunState_START04=0x701c,//启动，停止，指示图标


EM_Button_RunState_Switch01=0x7021,//启动，停止，按钮
EM_Button_RunState_Switch02=0x7022,//启动，停止，按钮
EM_Button_RunState_Switch03=0x7023,//启动，停止，按钮
EM_Button_RunState_Switch04=0x7024,//启动，停止，按钮


EM_ButtonIcon_Mode_Select_1=0x7031,//并充，并放，维护切换按钮
EM_ButtonIcon_Mode_Select_2=0x7032,//并充，并放，维护切换按钮
EM_ButtonIcon_Mode_Select_3=0x7033,//并充，并放，维护切换按钮
EM_ButtonIcon_Mode_Select_4=0x7034,//并充，并放，维护切换按钮


//EM_Icon_RunState_START01=0x7029,//启动，停止，图标
//EM_Icon_RunState_START02=0x702a,//启动，停止，图标
//EM_Icon_RunState_START03=0x702b,//启动，停止，图标
//EM_Icon_RunState_START04=0x702c,//启动，停止，图标




EM_START_STOP_ICON_01=0x7051,
EM_START_STOP_ICON_02=0x7052,
EM_START_STOP_ICON_03=0x7053,
EM_START_STOP_ICON_04=0x7054,

EM_Icon_RunState_SwitchAll=0x7071,//启动，停止，暂停指示图标
EM_Button_RunState_SwitchAll=0x7072,//启动，停止，暂停按钮

EM_Button_Language_Select=0x7080,//中英切换
EM_ActiveConfirm=0x7081,//点击激活确认保存按钮



//EM_Icon_Mode_Select_1=0x70F9,//维护，串，并切换指示图标
//EM_Icon_Mode_Select_2=0x70FA,//维护，串，并切换指示图标
//EM_Icon_Mode_Select_3=0x70FB,//维护，串，并切换指示图标
//EM_Icon_Mode_Select_4=0x70FC,//维护，串，并切换指示图标

//EM_Icon_Mode_Select_5=0x70FD,//维护，串，并切换指示图标
//EM_Icon_Mode_Select_6=0x70FE,//维护，串，并切换指示图标
//EM_Icon_Mode_Select_7=0x70FF,//维护，串，并切换指示图标
//EM_Icon_Mode_Select_8=0x7100,//维护，串，并切换指示图标







EM_SaveSettingFrom=0x70E1,



EM_SingleBattRun01=0x70E1,



EM_Run_Vol_difference01=0x70E9,

EM_polarization=0x70F1,//极化

EM_RunWorkingMode01=0x70F9,//工作模式

EM_BoardBat01=0x7101,/*< 充电单体数量 */	
EM_BoardBat02,/*< 充电板数量 */		
EM_BoardBat03,/*< 充电板数量 */	
EM_BoardBat04,/*< 充电板数量 */
EM_BoardBat05,/*< 充电板数量 */	
EM_BoardBat06,/*< 充电板数量 */		
EM_BoardBat07,/*< 充电板数量 */	
EM_BoardBat08,/*< 充电板数量 */




EM_Bat_UpperLimitParallelVol01=0x7109,//并充截止电压 mV
EM_Bat_UpperLimitParallelVol02,//并充截止电压 mV
EM_Bat_UpperLimitParallelVol03,//并充截止电压 mV
EM_Bat_UpperLimitParallelVol04,//并充截止电压 mV
EM_Bat_UpperLimitParallelVol05,//并充截止电压 mV
EM_Bat_UpperLimitParallelVol06,//并充截止电压 mV
EM_Bat_UpperLimitParallelVol07,//并充截止电压 mV
EM_Bat_UpperLimitParallelVol08,//并充截止电压 mV


EM_Bat_UpperAlarmParallelVol01=0x7111,	//并充报警上限电压 mV
EM_Bat_UpperAlarmParallelVol02,	//并充报警上限电压 mV
EM_Bat_UpperAlarmParallelVol03,	//并充报警上限电压 mV
EM_Bat_UpperAlarmParallelVol04,	//并充报警上限电压 mV
EM_Bat_UpperAlarmParallelVol05,	//并充报警上限电压 mV
EM_Bat_UpperAlarmParallelVol06,	//并充报警上限电压 mV
EM_Bat_UpperAlarmParallelVol07,	//并充报警上限电压 mV
EM_Bat_UpperAlarmParallelVol08,	//并充报警上限电压 mV

//EM_Bat_LowerLimitParallelVol01=0x7119,//并放截止电压 mV

EM_Bat_LowAlarmParallelVol01=0x7119,	//并充报警上限电压 mV
//EM_Bat_BalanceVol01_xmV=0x7129,/*< 充电电压 */




EM_Bat_ChargeCurrent01_xmA=0x7121,/*< 充电电流 */
EM_Bat_ChargeCurrent02_xmA,/*< 充电电流 */
EM_Bat_ChargeCurrent03_xmA,/*< 充电电流 */
EM_Bat_ChargeCurrent04_xmA,/*< 充电电流 */
EM_Bat_ChargeCurrent05_xmA,/*< 充电电流 */
EM_Bat_ChargeCurrent06_xmA,/*< 充电电流 */
EM_Bat_ChargeCurrent07_xmA,/*< 充电电流 */
EM_Bat_ChargeCurrent08_xmA,/*< 充电电流 */




EM_Bat_StopCapacity01_AH=0x7129,/*< 容量 */	
EM_Bat_StopCapacity02_AH,
EM_Bat_StopCapacity03_AH,
EM_Bat_StopCapacity04_AH,
EM_Bat_StopCapacity05_AH,
EM_Bat_StopCapacity06_AH,
EM_Bat_StopCapacity07_AH,
EM_Bat_StopCapacity08_AH,

EM_Bat_voldifferent01=0x7131,
EM_Bat_voldifferent02,
EM_Bat_voldifferent03,
EM_Bat_voldifferent04,
EM_Bat_voldifferent05,
EM_Bat_voldifferent06,
EM_Bat_voldifferent07,
EM_Bat_voldifferent08,









EM_rectifyShowChargeCurrent01_01=0x7200,	
EM_rectifyShowChargeCurrent01_02,	
EM_rectifyShowChargeCurrent01_03,	
EM_rectifyShowChargeCurrent01_04,	
EM_rectifyShowChargeCurrent01_05,	
EM_rectifyShowChargeCurrent01_06,	
EM_rectifyShowChargeCurrent01_07,	
EM_rectifyShowChargeCurrent01_08,	
EM_rectifyShowChargeCurrent01_09,	
EM_rectifyShowChargeCurrent01_10,	
EM_rectifyShowChargeCurrent01_11,	
EM_rectifyShowChargeCurrent01_12,
//EM_rectifyShowChargeCurrentEnable,
//EM_rectifyShowChargeCurrentReset,


EM_rectifyRealChargeCurrent01_01,
EM_rectifyRealChargeCurrent01_02,	
EM_rectifyRealChargeCurrent01_03,	
EM_rectifyRealChargeCurrent01_04,	
EM_rectifyRealChargeCurrent01_05,	
EM_rectifyRealChargeCurrent01_06,	
EM_rectifyRealChargeCurrent01_07,	
EM_rectifyRealChargeCurrent01_08,	
EM_rectifyRealChargeCurrent01_09,	
EM_rectifyRealChargeCurrent01_10,	
EM_rectifyRealChargeCurrent01_11,	
EM_rectifyRealChargeCurrent01_12,
EM_rectifyRealChargeCurrentEnable,
EM_rectifyRealChargeCurrentReset,



EM_rectifyShowDisChargeCurrent01_01,	
EM_rectifyShowDisChargeCurrent01_02,	
EM_rectifyShowDisChargeCurrent01_03,	
EM_rectifyShowDisChargeCurrent01_04,	
EM_rectifyShowDisChargeCurrent01_05,	
EM_rectifyShowDisChargeCurrent01_06,	
EM_rectifyShowDisChargeCurrent01_07,	
EM_rectifyShowDisChargeCurrent01_08,	
EM_rectifyShowDisChargeCurrent01_09,	
EM_rectifyShowDisChargeCurrent01_10,	
EM_rectifyShowDisChargeCurrent01_11,	
EM_rectifyShowDisChargeCurrent01_12,	



EM_rectifyRealDisChargeCurrent01_01,	
EM_rectifyRealDisChargeCurrent01_02,	
EM_rectifyRealDisChargeCurrent01_03,	
EM_rectifyRealDisChargeCurrent01_04,	
EM_rectifyRealDisChargeCurrent01_05,	
EM_rectifyRealDisChargeCurrent01_06,	
EM_rectifyRealDisChargeCurrent01_07,	
EM_rectifyRealDisChargeCurrent01_08,	
EM_rectifyRealDisChargeCurrent01_09,	
EM_rectifyRealDisChargeCurrent01_10,	
EM_rectifyRealDisChargeCurrent01_11,	
EM_rectifyRealDisChargeCurrent01_12,	


EM_rectifyRealDisChargeCurrentEnable,	
EM_rectifyRealDisChargeCurrentReset,






EM_rectifyTrueVol01_01,	
EM_rectifyTrueVol01_02,	
EM_rectifyTrueVol01_03,	
EM_rectifyTrueVol01_04,	
EM_rectifyTrueVol01_05,	
EM_rectifyTrueVol01_06,	
EM_rectifyTrueVol01_07,	
EM_rectifyTrueVol01_08,	
EM_rectifyTrueVol01_09,	
EM_rectifyTrueVol01_10,	
EM_rectifyTrueVol01_11,	
EM_rectifyTrueVol01_12,
EM_rectifyTrueVolEnable,
EM_rectifyTrueVolReset,






EM_rectifyShowChargeCurrent02_01=0x7200+EM_RECTIFY_JUMP*1,	

EM_rectifyTrueVol02_01=EM_rectifyTrueVol01_01+EM_RECTIFY_JUMP*1,	

EM_rectifyShowChargeCurrent03_01=0x7200+EM_RECTIFY_JUMP*2,	

EM_rectifyTrueVol03_01=EM_rectifyTrueVol01_01+EM_RECTIFY_JUMP*2,	

EM_rectifyShowChargeCurrent04_01=0x7200+EM_RECTIFY_JUMP*3,	

EM_rectifyTrueVol04_01=EM_rectifyTrueVol01_01+EM_RECTIFY_JUMP*3,	







EM_rectifyShowChargeCurrent05_01=0x7200+EM_RECTIFY_JUMP*4,	

EM_rectifyTrueVol05_01=EM_rectifyTrueVol01_01+EM_RECTIFY_JUMP*4,	

EM_rectifyShowChargeCurrent06_01=0x7200+EM_RECTIFY_JUMP*5,	

EM_rectifyTrueVol06_01=EM_rectifyTrueVol01_01+EM_RECTIFY_JUMP*5,	

EM_rectifyShowChargeCurrent07_01=0x7200+EM_RECTIFY_JUMP*6,	

EM_rectifyTrueVol07_01=EM_rectifyTrueVol01_01+EM_RECTIFY_JUMP*6,	

EM_rectifyShowChargeCurrent08_01=0x7200+EM_RECTIFY_JUMP*7,	

EM_rectifyTrueVol08_01=EM_rectifyTrueVol01_01+EM_RECTIFY_JUMP*7,	



EM_SYS_SHOW_SET=0x7401,
EM_SYS_SHOW_YEAR=0x7401,
EM_SYS_SHOW_MONTH=0x7402,
EM_SYS_SHOW_DAY=0x7403,
EM_SYS_SHOW_HOUR=0x7404,
EM_SYS_SHOW_MIUTE=0x7405,
EM_SYS_SHOW_SECOND=0x7406,


EM_SYS_SET_YEAR=0x7407,
EM_SYS_SET_MONTH,
EM_SYS_SET_DAY,
EM_SYS_SET_HOUR,
EM_SYS_SET_MIUTE,
EM_SYS_SET_SECOND,

EM_SYS_SAVE_HOUR,
EM_SYS_SAVE_MIUTE,
EM_SYS_SAVE_SECOND,


EM_SEQUENCE_NUMBER=0x7420,
EM_SEQUENCE_ACTIVE=0x7430,
EM_SEQUENCE_ENTERNEW_NUMBER=0x7434,

//uint16_t    sys_sd_month;
//uint16_t    sys_sd_day;
//uint16_t    sys_sd_hour;
//uint16_t    sys_sd_minute;	
//uint16_t    sys_sd_second;	

//uint16_t    sys_sd_save_hour;
//uint16_t    sys_sd_save_minute;	
//uint16_t    sys_sd_save_second;	



PB_ChaBitstate_16=0x6118,
PB_ChaBitstate_32=0x6119,


EM_SumSerialVol  =0x6200,
EM_SingleMaxVol,
EM_SingleMinVol,	
EM_MaxDiffVol,	
EM_SerialCurrent,	
EM_SingleMaxNUM=0x6205,
EM_R1A_S2A    =0x6206,
EM_ChargeTime=0x6207,






PB_BC_SumlimitValue=0x6300,
PB_BC_UpperlimitValue,
PB_BC_lowerlimitValue,
PB_BC_UpperAlarmValue,
PB_BC_lowerAlarmValue,

PB_BC_R1A_S2A,
PB_BC_Maintain_Num,
PB_BC_Delay_Mintues,
EM_StateMessageDelay = 0x6400,
EM_StateMessage = 0x6401,




EM_REAL_Vol_B01_S01=0x6500,
PB_BC_instrumentVol02,
PB_BC_instrumentVol03,
PB_BC_instrumentVol04,
PB_BC_instrumentVol05,
PB_BC_instrumentVol06,
PB_BC_instrumentVol07,
PB_BC_instrumentVol08,
PB_BC_instrumentVol09,
PB_BC_instrumentVol10,
PB_BC_instrumentVol11,
PB_BC_instrumentVol12,
PB_BC_instrumentVol13,
PB_BC_instrumentVol14,
PB_BC_instrumentVol15,
PB_BC_instrumentVol16,
PB_BC_instrumentVol17,
PB_BC_instrumentVol18,
PB_BC_instrumentVol19,
PB_BC_instrumentVol20,
PB_BC_instrumentVol21,
PB_BC_instrumentVol22,
PB_BC_instrumentVol23,
PB_BC_instrumentVol24,
PB_BC_instrumentSumVol=0x6518,
PB_BC_instrumentCurrent=0x6519,
PB_BC_rectify1_16=0x651A,
PB_BC_rectify16_24=0x651B,
PB_BC_reset1_16=0x651C,
PB_BC_reset16_24=0x651D,








PB_BC_Volcapacity_01=0x6600,
PB_BC_Volcapacity_02,
PB_BC_Volcapacity_03,
PB_BC_Volcapacity_04,
PB_BC_Volcapacity_05,
PB_BC_Volcapacity_06,
PB_BC_Volcapacity_07,
PB_BC_Volcapacity_08,
PB_BC_Volcapacity_09,
PB_BC_Volcapacity_10,
PB_BC_Volcapacity_11,
PB_BC_Volcapacity_12,
PB_BC_Volcapacity_13,
PB_BC_Volcapacity_14,
PB_BC_Volcapacity_15,
PB_BC_Volcapacity_16,
PB_BC_Volcapacity_17,
PB_BC_Volcapacity_18,
PB_BC_Volcapacity_19,
PB_BC_Volcapacity_20,
PB_BC_Volcapacity_21,
PB_BC_Volcapacity_22,
PB_BC_Volcapacity_23,
PB_BC_Volcapacity_24,




};
#define NUM_ScreenSetPara  16
#define NUM_ScreenMaxVolPara  24
//uint16_t Battery_SET_SumlimitValue;   /*< 维护电池上限电压 */
//uint16_t Battery_SET_UpperlimitValue;   /*< 维护电池上限电压 */
//uint16_t Battery_SET_lowerlimitValue;	 /*< 维护电池下限电压 */
//uint16_t Battery_SET_UpperAlarmValue;   /*< 维护电池上限电压 */
//uint16_t Battery_SET_lowerAlarmValue;	 /*< 维护电池下限电压 */
//	
//uint16_t Battery_SET_Function_n;  /*< 0均衡1串2并 */	
//uint16_t Battery_SET_R1A_S2A;    /*< 充电模块1A = 0 ，2A =1 . */		
//uint8_t Battery_SET_Maintain_Num;	/*< 维护电池数量 */	
//uint8_t  no01;	
//uint8_t	 Battery_SET_Delay_Mintues;	 /*< 并充延迟分钟 */
//uint8_t	 Battery_SET_Delay_Seconds;	 /*< 并充延迟秒 */	


enum SCREEN_VolGrade{
VOL_NONE=0,
VOL_TooLow,
VOL_Low,
VOL_Middle,
VOL_High,	
VOL_Saturate,		
VOL_OverSaturate,
};




typedef struct SCREEN_ADDRESSdata_ITEM{
uint16_t* s_address;
uint16_t* s_data;

}SCREEN_ADDRESSdata_ITEM_T;











void ScreenSendVolDataToScreen(void);

//void SCREEN_copy_data_from_queue(QUEUE_DATA_BUFF *qdf,uint8_t **frame,uint16_t* length,uint8_t** Data,uint8_t* Datanum,uint16_t* addr);

void Screen_SetParrleChargeIconState(void);

void screendataswitch(uint16_t addr,uint8_t num,uint8_t** p_data);

//void screendataswitch(uint16_t addr,uint8_t num,uint8_t* p_data);

void ScreenSendALLVolToDis(void);

void SCREEN_write_cmd(uint16_t addr, uint16_t *buf, uint16_t size);

void SCREEN_read_cmd(uint16_t addr, uint8_t num);

void ScreenSettingParaSend(void);

void pull_data_from_screen(void);
//void ScreenSendParaVol(void);





	


#endif /* __SCREENDATADIS_H */


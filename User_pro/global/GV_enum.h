#ifndef __GV_ENUM_H
#define	__GV_ENUM_H



#include "./stm32_FH_xxx_hal.h"









//enum COM485_232_REGAddress{
//EM_GET_DATA_ADDRESS=0x5000,
//	
//EM_SEND_RUNSTOP_ADDRESS=0x6020,	
//EM_SEND_SETTING_ADDRESS=0x6030,		
//};


enum ErrorType{
EM_Nomal=0,
EM_OverCharge=1,
EM_OverDisChargel=2,
EM_BreakLine=3,
EM_OverCurrent=4,
EM_OverDisCurrent=5,

EM_ReceiveInterval=6,
EM_CloseError=7,
EM_CurrentBreakdown=8,	
};




enum SCREEN_WorkState{

EM_SC_STOP=0,
EM_SC_CONSTANTCURRENT_CHARGE=1,
EM_SC_CONSTANTCURRENT_DISCHARGE=2,
EM_SC_CONSTANTCURRENT_BALANCE=3,




};


enum HWWorkState{
EM_HW_STOP=0,
EM_HW_CONSTANTCURRENT_CHARGE=1,

EM_HW_CONSTANTCURRENT_DISCHARGE=2,

EM_HW_CONSTANTCURRENT_BALANCE=3,


EM_HW_LOSEPOWER_RESUME=4,	//¶Ïµç»Ö¸´
EM_HW_LOSEPOWER_PARA=5,	//¶Ïµç»Ö¸´	
};



enum RunStateType{
EM_RunState_Stop=0,
EM_RunState_Run=1,

	
};	
	
	

enum SD_SHOW_Type{
EM_SHOW_VOL=0,
EM_SHOW_CURRENT=1,
EM_SHOW_CAPICITY=2,

	
};	

enum IFsendType{
EM_NotSend=0,
EM_Send=1,

	
};	


enum IFPrintType{
EM_NotPrint=0,
EM_Print=1,

	
};	




















#endif /* __BASICMOTION_H */


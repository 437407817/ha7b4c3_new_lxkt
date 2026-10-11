#ifndef __PWR_STATEMACHINE_H
#define	__PWR_STATEMACHINE_H

#include "stm32_FH_xxx.h"
// 顶层功率主状态机
//typedef enum
//{
//    PWR_STOP,
//    PWR_CHARGE,
//    PWR_DISCHARGE,
//    PWR_BALANCE,
//    PWR_FAULT
//} PwrState_t;




//// 故障码枚举，单独定义，用来区分是什么故障
//typedef enum
//{
//    FAULT_NONE = 0,      // 无故障
//    FAULT_OV_VOLT = 1,   // 故障1：过压
//    FAULT_UV_VOLT = 2,   // 故障2：欠压
//    FAULT_OT_TEMP = 3    // 故障3：超温
//} FaultCode_t;


// 通道主状态
typedef enum
{
    CH_STOP,
    CH_CHARGE,
    CH_DISCHARGE,
    CH_BALANCE,
    CH_FAULT       // 故障主状态，内部带有子状态
} ChMainState_t;

// 故障子状态（仅 CH_FAULT 主状态下有效）
typedef enum
{
    FAULT_SUB_NONE = 0,
    FAULT_SUB_1,   // 故障1
    FAULT_SUB_2,   // 故障2
    FAULT_SUB_3    // 故障3
} ChFaultSubState_t;

// 单通道结构体，每组独立全套信息
typedef struct
{
    ChMainState_t         main_state;        // 本组主状态
    ChFaultSubState_t     fault_sub;         // 故障子状态（仅main_state==CH_FAULT有效）
    uint8_t entry_flag;   // 新增：进入状态一次性执行标记
		uint8_t nop;   
    uint16_t volt;
    uint16_t curr;

    // 本组控制参数
    uint16_t para_chg_v;
    uint16_t para_bal_time;

    // 事件标记（命令事件，边沿触发，消费清零）
    uint8_t evt_start;
    uint8_t evt_stop;
} Channel_t;

#define CH_NUM 4




#endif /* __WORKLED_H */


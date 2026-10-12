#include "./service/Pwr_StateMachine.h"

#include "./sys/bsp_systime.h"   


 #include "./sys/sysio.h"
  

#include "./io/bsp_io_output.h" 




#include <stdint.h>


static Channel_t ch[CH_NUM];   // 4组通道实例


/**
 * @brief 通道公共预处理：采样、故障检测、保护判断
 * @retval 0=无故障；非0=故障码（故障触发）
 */
ChFaultSubState_t Channel_Common_Check(Channel_t *ch)
{
//    Sample_VI(ch);                // 采集电压电流
//    return CheckChannelFault(ch); // 故障检测，返回故障类型
}



void ChFsm_Run(Channel_t *ch)
{
    switch(ch->main_state)
    {
        case CH_STOP:
            // 收到启动事件，本组无故障，才能启动
            if(ch->evt_start && ch->fault_sub == FAULT_SUB_NONE)
            {
                ch->evt_start = 0;
                ch->main_state = CH_CHARGE;
            }
            break;

        case CH_CHARGE:
            if(ch->entry_flag)
            {
//                Charge_Start();
                ch->entry_flag = 0;
            }

            // ========= 公共检测，三个运行态共用这一行 =========
//            fault_ret = Channel_Common_Check(ch);
//            if(fault_ret != FAULT_SUB_NONE)
//            {
//                ch->fault_sub = fault_ret;
//                ch->main_state = CH_FAULT;
//                ch->entry_flag = 1;
////                PowerChannelOff(ch);
//                break;
//            }

            // ===== 只有充电独有的逻辑 =====
//            Charge_Pid_Ctrl(ch);
//            if(IsChargeFull(ch))
//            {
//                ch->evt_stop = 1;
//            }

            // 收到停止事件
            if(ch->evt_stop)
            {
                ch->evt_stop = 0;
//                Charge_Stop();
                ch->main_state = CH_STOP;
                ch->entry_flag = 1;
            }
            break;

        case CH_DISCHARGE:
            if(ch->entry_flag)
            {
//                Discharge_Start();
                ch->entry_flag = 0;
            }

            // ========= 同样调用公共检测 =========
//            fault_ret = Channel_Common_Check(ch);
//            if(fault_ret != FAULT_SUB_NONE)
//            {
//                ch->fault_sub = fault_ret;
//                ch->main_state = CH_FAULT;
//                ch->entry_flag = 1;
//                PowerChannelOff(ch);
//                break;
//            }

            // 放电独有逻辑
//            Discharge_Pid_Ctrl(ch);
//            if(IsDischargeEnd(ch))
//            {
//                ch->evt_stop = 1;
//            }

            if(ch->evt_stop)
            {
                ch->evt_stop = 0;
//                Discharge_Stop();
                ch->main_state = CH_STOP;
                ch->entry_flag = 1;
            }
            break;

        case CH_BALANCE:
            if(ch->entry_flag)
            {
//                Balance_Start();
                ch->entry_flag = 0;
            }

            // ========= 公共检测 =========
//            fault_ret = Channel_Common_Check(ch);
//            if(fault_ret != FAULT_SUB_NONE)
//            {
//                ch->fault_sub = fault_ret;
//                ch->main_state = CH_FAULT;
//                ch->entry_flag = 1;
//                PowerChannelOff(ch);
//                break;
//            }

            // 均衡独有逻辑
//            Balance_Ctrl(ch);
//            if(IsBalanceDone(ch))
//            {
//                ch->evt_stop = 1;
//            }

            if(ch->evt_stop)
            {
                ch->evt_stop = 0;
//                Balance_Stop();
                ch->main_state = CH_STOP;
                ch->entry_flag = 1;
            }
            break;

        case CH_FAULT:
            // 故障状态：功率回路保持关闭
//            PowerChannelOff(ch);

            // 根据故障子状态做差异化处理
            switch(ch->fault_sub)
            {
                case FAULT_SUB_1:
                    // 故障1专属逻辑，例如：故障1必须手动复位
                    break;
                case FAULT_SUB_2:
                    // 故障2专属逻辑，例如：故障2条件恢复后可自动复位
                    break;
                case FAULT_SUB_3:
                    break;
                default:
                    break;
            }

            // 故障复位逻辑
            if(ch->evt_stop)
            {
                // 判断故障条件已经消失
//                if( FaultConditionRecover(ch, ch->fault_sub) )
//                {
//                    ch->evt_stop = 0;
//                    ch->fault_sub = FAULT_SUB_NONE;
//                    ch->main_state = CH_STOP;
//                }
            }
            break;

        default:
            ch->main_state = CH_STOP;
            ch->fault_sub = FAULT_SUB_NONE;
            break;
    }
}



















/*********************************************END OF FILE**********************/

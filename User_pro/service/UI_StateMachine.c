#include "./service/UI_StateMachine.h"
#include "./service/Pwr_StateMachine.h"

#include "./sys/bsp_systime.h"   


 #include "./sys/sysio.h"
  

#include "./io/bsp_io_output.h" 




#include <stdint.h>


static UiState_t ui_state;
static uint8_t ui_sel_ch;   // 当前屏幕查看通道：0~3

static UiState_t ui_state = UI_MAIN_PAGE;


void UiFsm_Run(void)
{
    switch(ui_state)
    {
        case UI_CH_VIEW_PAGE:
            // 页面切换按键：只切换查看通道，**不改变任何通道运行状态**
            if(key_page_switch)
            {
                ui_sel_ch = (ui_sel_ch + 1) % CH_NUM;
            }

            // ========== 本组启动：只发给当前选中通道 ==========
            if(key_group_start)
            {
                ch[ui_sel_ch].evt_start = 1;
            }
            // 本组停止
            if(key_group_stop)
            {
                ch[ui_sel_ch].evt_stop = 1;
            }

            // ========== 全部组启动：广播给4通道 ==========
            if(key_all_start)
            {
                for(int i=0; i<CH_NUM; i++)
                {
                    // 已经故障的组，不启动
                    if(ch[i].main_state != CH_FAULT)
                    {
                        ch[i].evt_start = 1;
                    }
                }
            }
            // 全部停止：所有通道下发停止事件
            if(key_all_stop)
            {
                for(int i=0; i<CH_NUM; i++)
                {
                    ch[i].evt_stop = 1;
                }
            }

            // 屏幕渲染：读取 ch[ui_sel_ch] 的main_state 和 fault_sub，显示状态文字
            Screen_RefreshChannel(&ch[ui_sel_ch]);
            break;

        case UI_SET_PARAM_PAGE:
            // 修改参数，只修改ui_sel_ch选中通道的参数
            break;
        default:
            break;
    }
}

















/*********************************************END OF FILE**********************/

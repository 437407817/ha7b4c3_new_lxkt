#ifndef __UI_STATEMACHINE_H
#define	__UI_STATEMACHINE_H




#include "stm32_FH_xxx.h"



// UI子状态机（独立并行）
//typedef enum
//{
//    UI_MAIN_PAGE,
//    UI_SET_PARAM_PAGE,
//    UI_HIST_PAGE,
//    UI_CALIB_PAGE
//} UiState_t;


// UI相关
typedef enum
{
    UI_MAIN_PAGE,
    UI_CH_VIEW_PAGE,    // 通道查看页面，依靠ui_sel_ch区分4个页面
    UI_SET_PARAM_PAGE
} UiState_t;









#endif /* __WORKLED_H */


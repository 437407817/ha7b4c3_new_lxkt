#ifndef __STATEFUN_H
#define	__STATEFUN_H

#include "stm32_FH_xxx.h"





#include "./global/GV_variable.h" 
void MaintainState_Select(uint8_t num,uint8_t* p_data);
void PageMainSelect(uint8_t num,uint8_t* p_data);
//void RunState_Switch(uint8_t num,uint8_t* p_data);
void RunState_Switch(uint8_t num,uint8_t* p_data,uint8_t launchboardnum);

void RunState_SwitchAll(uint8_t num,uint8_t* p_data);
void Jump_to_now_page(void);


void MaintainState_Maintain(void);
void MaintainState_Serial(void);
void MaintainState_Parallel(void);


void ScreenJumpToMainPage(void);







void PageSettingConfirm(uint8_t num,uint8_t* p_data);


void PagePowerDownConfirm(uint8_t num,uint8_t* p_data);


void PageSettingSave(uint8_t num,uint8_t** p_data);

void VolRectifyConfirm(uint8_t num,uint8_t* p_data);


void VolRectifyHeadSave(uint8_t num,uint8_t** p_data);

void VolRectifyTailSave(uint8_t num,uint8_t** p_data);

void PageCapPageReturn(uint8_t num,uint8_t* p_data);

void PageSYSSettingConfirm(uint8_t num,uint8_t* p_data);

void MechineActiveSave(uint8_t num,uint8_t** p_data);
void MechineActiveConfirm(uint8_t num,uint8_t* p_data);

void ScreenJumpToAimPage(uint16_t*  ScreenData);
void ScreenJumpToAimPageAndReturnMain(uint16_t*  ScreenData);

#endif /* __STATEFUN_H */


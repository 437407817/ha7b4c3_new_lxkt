#ifndef __BSP_ENCODE_H
#define	__BSP_ENCODE_H


#include "stm32_FH_xxx.h"












void init_encodechipid(void);


void showChipID(void);


void saveEEpromChip(void);


void FirstSaveChipData(void);

void AnalyzeEEpromSaveChipData(void);

void ClearEEpromSaveChipData(void);

uint8_t checkIsRightChipData(uint8_t num,uint8_t** p_data);

void saveAndUpdateEepromChip_Code(void);
uint16_t CheckOverDateChipData(void);
void PowerUpCheckOverData(void);
uint8_t segger_encodedata(uint8_t* p_data,uint8_t num) ;

uint8_t NotActiveMechineReturn(void);
uint8_t NotTryOutAndActiveMechineReturn(void);


#endif /* __BSP_CRYPT_AES_H */


#ifndef __DISPOSEDATAFUN_H
#define	__DISPOSEDATAFUN_H


#include "stm32_FH_xxx.h"








//void DisposeSaveSettingData(void);


void CopySettingDataToSlaveBuff(uint8_t itv);



void CopyEEpromDatatoSettingData(void);



void calculateVolDataFun(void);
void SlaveCalibrationcalculate(void);

void AskUploadDataovertimefun(void);


void clearScreenDataVol(void);



#endif /* __DISPOSEDATAFUN_H */



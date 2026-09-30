#ifndef __SCREEN_H
#define	__SCREEN_H


#include "stm32_FH_xxx.h"







#define DW_CRC16   0

void DW_SCREEN_write_cmd(uint16_t addr, uint16_t *buf, uint16_t size);
void DW_SCREEN_write_cmd_print(uint16_t addr, uint16_t *buf, uint16_t size, uint16_t debugprint) ;
void DW_SCREEN_read_cmd_print(uint16_t addr, uint8_t num, uint16_t debugprint);

void DW_DGUSI_SCREEN_write_Reg_cmd_print(uint8_t REGaddr, uint16_t REGADATA,  uint16_t debugprint);




#endif /* __SCREEN_H */


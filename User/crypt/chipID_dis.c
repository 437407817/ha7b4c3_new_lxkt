/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESmodes/bsp_crypt_aes.c 
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 0 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "./crypt/bsp_encode.h"
#include "./sys/sysio.h"
#include "./sys/bsp_systime.h"  
#include "./global/GV_variable.h" 
#include "./crypt/crypt_aes.h" 

#include "./DataConvert/data_compare.h"
#include "./i2c/factory_iic.h"
#include "./crypt/chipID_dis.h"
#include "./screen/DW_com_send.h"
#include "./screen/ScreenDataDis.h"
 #include "./sys/sysio.h"
#include "./DataConvert/data_convert.h"
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup CRYP_AESmodes
  * @{
  */ 


extern str_ChipEncode GV_EEpromChipEncode;
extern str_ChipEncode GV_SaveEEpChipEncode;

extern str_SrceenChipEncode_data	GV_ScreenChipEncode;

//extern DS_time_ITEM_t  DS_time_ITEM;
extern str_DS1302_data	DS1302_data;

#define ChipUniqueID1   *(__IO uint32_t *)(0x1FFF7A10) // 低字节
#define ChipUniqueID2   *(__IO uint32_t *)(0x1FFF7A14)
#define ChipUniqueID3   *(__IO uint32_t *)(0x1FFF7A18) // 高字节
#define ChipSumFlashSize   *(__IO uint16_t *)(0X1FFFF7E0) //flash大小


#define STRUCT_SIZE 15

//extern str_ChipEncode GV_EEpromChipEncode;
//extern str_ChipEncode GV_SaveEEpChipEncode;









void ScreenChip_IDParaSend(void){
FirstSaveChipData();
//SendScreenChipData();
char asiic_code[32];
char asiic_code_reverse[32];
		SYSTEM_DEBUG_ARRAY_MESSAGE((uint8_t *)GV_EEpromChipEncode.code,sizeof(GV_EEpromChipEncode.code),"sizeof(GV_EEpromChipEncode.code)==  length %d",sizeof(GV_EEpromChipEncode.code));
//		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(2,encrypted_output,encrypted_olen,"encrypted_output \n");
	HexToAscii((uint8_t *)GV_EEpromChipEncode.code,(uint8_t *)asiic_code,16);
//	hex_to_ascii((char *)GV_EEpromChipEncode.code,(char *)asiic_code,16);
	memcpy(asiic_code_reverse, asiic_code, 32);
SYSTEM_DEBUG_STRING(asiic_code,sizeof(asiic_code));
	EndianSwap_VpChange16HL((uint16_t *)asiic_code_reverse,32);
DW_SCREEN_write_cmd_print(EM_SEQUENCE_NUMBER,(uint16_t *)asiic_code_reverse,16,0);
	 AnalyzeEEpromSaveChipData();
	
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(2,(uint8_t *)&GV_ScreenChipEncode.Srceen_ChipisActive,8,"encrypted_output ---- \n");
DW_SCREEN_write_cmd_print(EM_SEQUENCE_ACTIVE,(uint16_t *)&GV_ScreenChipEncode.Srceen_ChipisActive,4,0);	
	

//	SYSTEM_DEBUG_STRING_2(EM_SEGGER_PRINT,asiic_code,32);
};





















/*********************************************END OF FILE**********************/

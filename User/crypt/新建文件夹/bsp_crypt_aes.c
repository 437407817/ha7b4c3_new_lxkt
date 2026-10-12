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
#include "./crypt/bsp_crypt_aes.h"
#include "./sys/sysio.h"
#include "./sys/bsp_systime.h"  


/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup CRYP_AESmodes
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
 uint8_t AES128key[16] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
                          0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c}; /* key size 128 bytes */
  
 uint8_t AES192key[24] = {0x8e,0x73,0xb0,0xf7,0xda,0x0e,0x64,0x52,
                          0xc8,0x10,0xf3,0x2b,0x80,0x90,0x79,0xe5,
                          0x62,0xf8,0xea,0xd2,0x52,0x2c,0x6b,0x7b}; /* key size 192 bytes */

 uint8_t AES256key[32] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,
                          0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,
                          0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,
                          0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4}; /* key size 256 bytes */

 uint8_t IV_1[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                     0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f}; /* initialization vector */

uint8_t Plaintext[AES_TEXT_SIZE] = 
                        {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
                         0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
                         0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,
                         0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
                         0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,
                         0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,
                         0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,
                         0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10}; /* plaintext */

uint8_t Ciphertext[AES_TEXT_SIZE] = 
                        {0x76,0x49,0xab,0xac,0x81,0x19,0xb2,0x46,
                         0xce,0xe9,0x8e,0x9b,0x12,0xe9,0x19,0x7d,
                         0x50,0x86,0xcb,0x9b,0x50,0x72,0x19,0xee,
                         0x95,0xdb,0x11,0x3a,0x91,0x76,0x78,0xb2,
                         0x73,0xbe,0xd6,0xb8,0xe3,0xc1,0x74,0x3b,
                         0x71,0x16,0xe6,0x9e,0x22,0x22,0x95,0x16,
                         0x3f,0xf1,0xca,0xa1,0x68,0x1f,0xac,0x09,
                         0x12,0x0e,0xca,0x30,0x75,0x86,0xe1,0xa7}; /* ciphertext */

uint8_t Encryptedtext[AES_TEXT_SIZE]; /* Encrypted text */
uint8_t Decryptedtext[AES_TEXT_SIZE]; /* Decrypted text */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//static void USART_Config(void);
static void Display_PlainData(uint32_t datalength);
static void Display_EncryptedData(uint8_t mode,uint16_t keysize,uint32_t datalength);
static void Display_DecryptedData(uint8_t mode,uint16_t keysize,uint32_t datalength);
//static char PressToContinue(void);

static void AES128_Encrypt_init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void AES_test1(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40_41xxx.s/startup_stm32f427_437xx.s/startup_stm32f429_439xx.s)
       before to branch to application main.
     */     
       
  /* USARTx configured as follows:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  //USART_Config();
  
  /* Enable CRYP clock */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_CRYP, ENABLE);
  AES128_Encrypt_init();
  while(1)
  { 
    /* Display Plain Data*/
    Display_PlainData(AES_TEXT_SIZE);

/******************************************************************************/
/*                             AES mode ECB                                   */
/******************************************************************************/

/*=====================================================
  Encryption ECB mode                                        
======================================================*/
    //PressToContinue();
		delay_ms(3000);
/****************************************/
/*                           AES 128   **/
/****************************************/
    /* Encrypt the plaintext message*/
    if(CRYP_AES_ECB(MODE_ENCRYPT, AES128key, 128, Plaintext, AES_TEXT_SIZE, Encryptedtext) == SUCCESS)
    {
      /* Display encrypted Data*/
			SYSTME_DEBUG("   SUCCESS");
      Display_EncryptedData(ECB,128,AES_TEXT_SIZE);
    }else {
        SYSTME_DEBUG("   FAILURE");
    }
/****************************************/
/*                           AES 192   **/
/****************************************/
    /* Encrypt the plaintext message*/
    if(CRYP_AES_ECB(MODE_ENCRYPT,AES192key,192,Plaintext,AES_TEXT_SIZE,Encryptedtext) == SUCCESS)
    {
      /* Display encrypted Data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_EncryptedData(ECB,192,AES_TEXT_SIZE);
    }else {
        SYSTME_DEBUG("   FAILURE");
    }
/****************************************/
/*                           AES 256   **/
/****************************************/
    /* Encrypt the plaintext message*/
    if(CRYP_AES_ECB(MODE_ENCRYPT,AES256key,256,Plaintext,AES_TEXT_SIZE,Encryptedtext) == SUCCESS)
    {
      /* Display encrypted Data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_EncryptedData(ECB, 256,AES_TEXT_SIZE);
    }else {
        SYSTME_DEBUG("   FAILURE");
    }
/*=====================================================
    Decryption in ECB mode                                       
======================================================*/
   //PressToContinue();
				delay_ms(3000);
/****************************************/
/*                           AES 128   **/
/****************************************/
    /* Decrypt the plaintext message  */
    if(CRYP_AES_ECB(MODE_DECRYPT,AES128key,128,Ciphertext,AES_TEXT_SIZE,Decryptedtext) == SUCCESS) 
    {
      /* Display decrypted data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_DecryptedData(ECB,128,AES_TEXT_SIZE);
    }
/****************************************/
/*                           AES 192   **/
/****************************************/
    /* Decrypt the plaintext message  */
    if(CRYP_AES_ECB(MODE_DECRYPT,AES192key, 192,Ciphertext, AES_TEXT_SIZE,Decryptedtext) == SUCCESS) 
    {
      /* Display decrypted data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_DecryptedData(ECB, 192,AES_TEXT_SIZE);
    }
/****************************************/
/*                           AES 256   **/
/****************************************/
    /* Decrypt the plaintext message  */
    if(CRYP_AES_ECB(MODE_DECRYPT,AES256key, 256,Ciphertext, AES_TEXT_SIZE,Decryptedtext) == SUCCESS)
    {
      /* Display decrypted data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_DecryptedData(ECB,256,AES_TEXT_SIZE);
    }

/******************************************************************************/
/*                             AES mode CBC                                   */
/******************************************************************************/
    //PressToContinue();
				delay_ms(3000);
/*=====================================================
  Encryption CBC mode                                        
======================================================*/

/****************************************/
/*                           AES 128   **/
/****************************************/
    /* Encrypt the plaintext message*/
    if(CRYP_AES_CBC(MODE_ENCRYPT,IV_1,AES128key,128,Plaintext,AES_TEXT_SIZE,Encryptedtext) == SUCCESS)
    {
      /* Display encrypted Data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_EncryptedData(CBC,128,AES_TEXT_SIZE);
    }
/****************************************/
/*                           AES 192   **/
/****************************************/
    /* Encrypt the plaintext message*/
    if(CRYP_AES_CBC(MODE_ENCRYPT,IV_1,AES192key,192,Plaintext,AES_TEXT_SIZE,Encryptedtext) == SUCCESS)
    {
      /* Display encrypted Data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_EncryptedData(CBC,192,AES_TEXT_SIZE);
    }
/****************************************/
/*                           AES 256   **/
/****************************************/
    /* Encrypt the plaintext message*/
    if(CRYP_AES_CBC(MODE_ENCRYPT,IV_1,AES256key,256,Plaintext,AES_TEXT_SIZE,Encryptedtext) == SUCCESS)
    {
      /* Display encrypted Data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_EncryptedData(CBC, 256,AES_TEXT_SIZE);
    }
/*=====================================================
    Decryption in CBC mode                                       
======================================================*/
    //PressToContinue();
				delay_ms(3000);
    /* Deinitializes the CRYP peripheral */
    CRYP_DeInit();
					SYSTME_DEBUG(" *****  SUCCESS");
/****************************************/
/*                           AES 128   **/
/****************************************/
    /* Decrypt the plaintext message  */
    if(CRYP_AES_CBC(MODE_DECRYPT,IV_1,AES128key,128,Ciphertext,AES_TEXT_SIZE,Decryptedtext) == SUCCESS)
    {
      /* Display decrypted data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_DecryptedData(CBC,128,AES_TEXT_SIZE);
    }
/****************************************/
/*                           AES 192   **/
/****************************************/
    /* Deinitializes the CRYP peripheral */
    CRYP_DeInit();

    /* Decrypt the plaintext message  */
    if(CRYP_AES_CBC(MODE_DECRYPT,IV_1,AES192key, 192,Ciphertext, AES_TEXT_SIZE,Decryptedtext) == SUCCESS)
    {
      /* Display decrypted data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_DecryptedData(CBC, 192,AES_TEXT_SIZE);
    }
/****************************************/
/*                           AES 256   **/
/****************************************/
    /* Deinitializes the CRYP peripheral */
    CRYP_DeInit();

    /* Decrypt the plaintext message  */
    if(CRYP_AES_CBC(MODE_DECRYPT,IV_1,AES256key, 256,Ciphertext, AES_TEXT_SIZE,Decryptedtext) == SUCCESS)
    {
      /* Display decrypted data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_DecryptedData(CBC,256,AES_TEXT_SIZE);
    }

/******************************************************************************/
/*                             AES mode CTR                                   */
/******************************************************************************/

/*=====================================================
  Encryption CTR mode                                        
======================================================*/
   //PressToContinue();
				delay_ms(3000);
/****************************************/
/*                           AES 128   **/
/****************************************/
    /* Encrypt the plaintext message*/
    if(CRYP_AES_CTR(MODE_ENCRYPT,IV_1,AES128key,128,Plaintext,AES_TEXT_SIZE,Encryptedtext) == SUCCESS)
    {
      /* Display encrypted Data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_EncryptedData(CTR,128, AES_TEXT_SIZE);
    }
/****************************************/
/*                           AES 192   **/
/****************************************/
    /* Encrypt the plaintext message*/
    if(CRYP_AES_CTR(MODE_ENCRYPT,IV_1,AES192key,192,Plaintext,AES_TEXT_SIZE,Encryptedtext) == SUCCESS)
    {
      /* Display encrypted Data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_EncryptedData(CTR,192, AES_TEXT_SIZE);
    }
/****************************************/
/*                           AES 256   **/
/****************************************/
    /* Encrypt the plaintext message*/
    if(CRYP_AES_CTR(MODE_ENCRYPT,IV_1,AES256key,256,Plaintext,AES_TEXT_SIZE,Encryptedtext) == SUCCESS)
    {
      /* Display encrypted Data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_EncryptedData(CTR, 256, AES_TEXT_SIZE);
    }
/*=====================================================
    Decryption in CTR mode                                       
======================================================*/
   //PressToContinue();
				delay_ms(3000);
/****************************************/
/*                           AES 128   **/
/****************************************/
    /* Decrypt the plaintext message  */
    if(CRYP_AES_CTR(MODE_DECRYPT,IV_1,AES128key,128,Ciphertext,AES_TEXT_SIZE,Decryptedtext) == SUCCESS)
    {
      /* Display decrypted data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_DecryptedData(CTR, 128, AES_TEXT_SIZE);
    }
/****************************************/
/*                           AES 192   **/
/****************************************/
    /* Decrypt the plaintext message  */
    if(CRYP_AES_CTR(MODE_DECRYPT,IV_1,AES192key,192,Ciphertext,AES_TEXT_SIZE,Decryptedtext) == SUCCESS)
    {
      /* Display decrypted data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_DecryptedData(CTR, 192, AES_TEXT_SIZE);
    }
/****************************************/
/*                           AES 256   **/
/****************************************/
    /* Decrypt the plaintext message  */
    if(CRYP_AES_CTR(MODE_DECRYPT,IV_1,AES256key, 256,Ciphertext, AES_TEXT_SIZE,Decryptedtext) == SUCCESS)
    {
      /* Display decrypted data*/
						SYSTME_DEBUG("   SUCCESS");
      Display_DecryptedData(CTR, 256, AES_TEXT_SIZE);
    }
/******************************************************************************/
    //PressToContinue();
				delay_ms(3000);
    SYSTME_INFO("\n\r Example restarted...\n ");
  }
}

/**
  * @brief  Display Plain Data 
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_PlainData(uint32_t datalength)
{
  uint32_t BufferCounter =0;
  uint32_t count = 0;
  
  SYSTME_INFO("\n\r =============================================================\n\r");
  SYSTME_INFO(" ================= Crypt Using HW Crypto  ====================\n\r");
  SYSTME_INFO(" ============================================================\n\r");
  SYSTME_INFO(" ---------------------------------------\n\r");
  SYSTME_INFO(" Plain Data :\n\r");
  SYSTME_INFO(" ---------------------------------------\n\r");
  
  for(BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    SYSTME_INFO("[0x%02X]", Plaintext[BufferCounter]);
    count++;

    if(count == 16)
    { 
      count = 0;
      SYSTME_INFO("  Block %d \n\r", BufferCounter/16);
    }
  }
}

/**
  * @brief  Display Encrypted Data 
  * @param  mode: chaining mode
  * @param  keysize: AES key size used
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_EncryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  SYSTME_INFO("\n\r =======================================\n\r");
  SYSTME_INFO(" Encrypted Data with AES %d  Mode  ",keysize );

  if(mode == ECB)
  {
    SYSTME_INFO("ECB\n\r");
  }    
  else if(mode == CBC)     
  {
    SYSTME_INFO("CBC\n\r");
  }      
  else /* if(mode == CTR)*/ 
  {
    SYSTME_INFO("CTR\n\r");
  }   

  SYSTME_INFO(" ---------------------------------------\n\r");
  
  for(BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    SYSTME_INFO("[0x%02X]", Encryptedtext[BufferCounter]);

    count++;
    if(count == 16)
    { 
      count = 0;
      SYSTME_INFO(" Block %d \n\r", BufferCounter/16);
    }
  }
}

/**
  * @brief  Display Decrypted Data 
  * @param  mode: chaining mode
  * @param  keysize: AES key size used
  * @param  datalength: length of the data to display
  * @retval None
  */
static void Display_DecryptedData(uint8_t mode, uint16_t keysize, uint32_t datalength)
{
  uint32_t BufferCounter = 0;
  uint32_t count = 0;

  SYSTME_INFO("\n\r =======================================\n\r");
  SYSTME_INFO(" Decrypted Data with AES %d  Mode  ",keysize );

  if(mode == ECB)
  {
    SYSTME_INFO("ECB\n\r");
  }    
  else if(mode == CBC)     
  {
    SYSTME_INFO("CBC\n\r");
  }      
  else /* if(mode == CTR)*/ 
  {
    SYSTME_INFO("CTR\n\r");
  }   
 
  SYSTME_INFO(" ---------------------------------------\n\r");
  
  for(BufferCounter = 0; BufferCounter < datalength; BufferCounter++)
  {
    SYSTME_INFO("[0x%02X]", Decryptedtext[BufferCounter]);
    count++;

    if(count == 16)
    { 
      count = 0;
      SYSTME_INFO(" Block %d \n\r", BufferCounter/16);
    }
  }
}


/**
  * @brief  Wait till a character is received by the USART
  * @param  None
  * @retval Keyboard pressed character
  */
static void AES128_Encrypt_init(void)
{
  CRYP_InitTypeDef CRYP_InitStructure;
  CRYP_KeyInitTypeDef CRYP_KeyInitStructure;

  /* Enable CRYP clock */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_CRYP, ENABLE);

  /* CRYP configuration********************************************************/
  /* Crypto key structure initialisation */ 
  CRYP_KeyStructInit(&CRYP_KeyInitStructure);
  
  /* Crypto Init for Encryption process */      
  CRYP_InitStructure.CRYP_AlgoDir  = CRYP_AlgoDir_Encrypt;
  CRYP_InitStructure.CRYP_AlgoMode = CRYP_AlgoMode_AES_ECB;
  CRYP_InitStructure.CRYP_DataType = CRYP_DataType_32b;
  CRYP_InitStructure.CRYP_KeySize  = CRYP_KeySize_128b;
  CRYP_Init(&CRYP_InitStructure);

  /* Key Initialisation */
  CRYP_KeyInitStructure.CRYP_Key2Left = AES128key[0];
  CRYP_KeyInitStructure.CRYP_Key2Right= AES128key[1];
  CRYP_KeyInitStructure.CRYP_Key3Left = AES128key[2];
  CRYP_KeyInitStructure.CRYP_Key3Right= AES128key[3];
  CRYP_KeyInit(&CRYP_KeyInitStructure);

  /* Enable Crypto processor **************************************************/
  CRYP_Cmd(ENABLE);

}
//void AES_Configure(void) {
//    AES_InitTypeDef AES_InitStruct;


//    AES_InitStruct.AES_KeySize = AES_KeySize_128b;
//    AES_InitStruct.AES_OperationMode = AES_OperationMode_ECB;
//    AES_InitStruct.AES_ChainingMode = AES_ChainingMode_ECB;
//    AES_InitStruct.AES_Mode = AES_Mode_Encrypt;


//    AES_Init(&AES_InitStruct);


//    // 配置密钥
//    uint8_t key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
//    AES_SetKey(key);
//}



/**
  * @}
  */ 

/**
  * @}
  */ 








#include "stm32f4xx.h" // 根据你的MCU选择正确的头文件

#include <stdint.h>



#define AES_KEYSIZE_128 16
#define AES_BLOCK_SIZE 16

uint8_t key[AES_KEYSIZE_128] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x25, 0x22, 0x40, 0x9c, 0xf0, 0xd0};
uint8_t input_data[AES_BLOCK_SIZE] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
uint8_t encrypted_data[AES_BLOCK_SIZE];
uint8_t decrypted_data[AES_BLOCK_SIZE];






void AES_Encrypt_Init(void)
{
    /* Enable CRYP clock */
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_CRYP, ENABLE);

    /* Key preparation for encryption */
    CRYP_KeyInitTypeDef CRYP_KeyInitStruct;
    CRYP_KeyInitStruct.CRYP_Key1Left = *(uint32_t*)(key);
    CRYP_KeyInitStruct.CRYP_Key1Right = *(uint32_t*)(key + 4);
    CRYP_KeyInitStruct.CRYP_Key2Left = *(uint32_t*)(key + 8);
    CRYP_KeyInitStruct.CRYP_Key2Right = *(uint32_t*)(key + 12);
    CRYP_KeyStructInit(&CRYP_KeyInitStruct);
    CRYP_KeyInit(&CRYP_KeyInitStruct);

    /* Initialization for encryption */
    CRYP_InitTypeDef CRYP_InitStruct;
    CRYP_InitStruct.CRYP_AlgoDir = CRYP_AlgoDir_Encrypt;
    CRYP_InitStruct.CRYP_AlgoMode = CRYP_AlgoMode_AES_ECB;
    CRYP_InitStruct.CRYP_DataType = CRYP_DataType_8b;
    CRYP_InitStruct.CRYP_KeySize = CRYP_KeySize_128b;
    CRYP_Init(&CRYP_InitStruct);

    /* Start encryption process */
    CRYP_Cmd(ENABLE);
}

void AES_Encrypt(uint8_t *input, uint8_t *output)
{
    /* Write input data to the DIN register */
    CRYP_DataIn(*(uint32_t*)input);
    CRYP_DataIn(*((uint32_t*)(input + 4)));
    CRYP_DataIn(*((uint32_t*)(input + 8)));
    CRYP_DataIn(*((uint32_t*)(input + 12)));

    /* Wait until the complete message has been processed */
    while(CRYP_GetFlagStatus(CRYP_FLAG_BUSY) != RESET);

    /* Read the encrypted block from the DOUT register */
    *(uint32_t*)output = CRYP_DataOut();
    *((uint32_t*)(output + 4)) = CRYP_DataOut();
    *((uint32_t*)(output + 8)) = CRYP_DataOut();
    *((uint32_t*)(output + 12)) = CRYP_DataOut();

    /* Disable CRYP to save power */
    CRYP_Cmd(DISABLE);
}




void AES_Decrypt_Init(void)
{
    /* Enable CRYP clock */
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_CRYP, ENABLE);

    /* Key preparation for decryption */
    CRYP_KeyInitTypeDef CRYP_KeyInitStruct;
    CRYP_KeyInitStruct.CRYP_Key1Left = *(uint32_t*)(key);
    CRYP_KeyInitStruct.CRYP_Key1Right = *(uint32_t*)(key + 4);
    CRYP_KeyInitStruct.CRYP_Key2Left = *(uint32_t*)(key + 8);
    CRYP_KeyInitStruct.CRYP_Key2Right = *(uint32_t*)(key + 12);
    CRYP_KeyStructInit(&CRYP_KeyInitStruct);
    CRYP_KeyInit(&CRYP_KeyInitStruct);

    /* Initialization for decryption */
    CRYP_InitTypeDef CRYP_InitStruct;
    CRYP_InitStruct.CRYP_AlgoDir = CRYP_AlgoDir_Decrypt; // 设置为解密方向
    CRYP_InitStruct.CRYP_AlgoMode = CRYP_AlgoMode_AES_ECB;
    CRYP_InitStruct.CRYP_DataType = CRYP_DataType_8b;
    CRYP_InitStruct.CRYP_KeySize = CRYP_KeySize_128b;
    CRYP_Init(&CRYP_InitStruct);

    /* Start decryption process */
    CRYP_Cmd(ENABLE);
}

void AES_Decrypt(uint8_t *input, uint8_t *output)
{
    /* Write input data to the DIN register */
    CRYP_DataIn(*(uint32_t*)input);
    CRYP_DataIn(*((uint32_t*)(input + 4)));
    CRYP_DataIn(*((uint32_t*)(input + 8)));
    CRYP_DataIn(*((uint32_t*)(input + 12)));

    /* Wait until the complete message has been processed */
    while(CRYP_GetFlagStatus(CRYP_FLAG_BUSY) != RESET);

    /* Read the decrypted block from the DOUT register */
    *(uint32_t*)output = CRYP_DataOut();
    *((uint32_t*)(output + 4)) = CRYP_DataOut();
    *((uint32_t*)(output + 8)) = CRYP_DataOut();
    *((uint32_t*)(output + 12)) = CRYP_DataOut();

    /* Disable CRYP to save power */
    CRYP_Cmd(DISABLE);
}








void AES_test(void)
{



AES_Encrypt_Init();
AES_Encrypt(input_data, encrypted_data);
	
SYSTME_DEBUG_ARRAY_MESSAGE((uint8_t*)input_data,AES_BLOCK_SIZE," AES_Encrypt =  ");	
SYSTME_DEBUG_ARRAY_MESSAGE((uint8_t*)encrypted_data,AES_BLOCK_SIZE," AES_Encrypt =  ");
	
AES_Decrypt_Init();
AES_Decrypt(encrypted_data, decrypted_data);

SYSTME_DEBUG_ARRAY_MESSAGE((uint8_t*)encrypted_data,AES_BLOCK_SIZE," AES_Decrypt =  ");









}


/*********************************************END OF FILE**********************/

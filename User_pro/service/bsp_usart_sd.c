/**
  ******************************************************************************
  * @file    bsp_USART_485_B.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  

#include "./buffer/p_data_queue.h"
#include "./service/bsp_usart_sd.h"
#include "./sys/sysio.h"





/// 配置USART接收中断
//static void NVIC_485_B_Configuration(void)
//{
//    NVIC_InitTypeDef NVIC_InitStructure;
//    /* Configure the NVIC Preemption Priority Bits */
//    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

//    /* Enable the USARTy Interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = _485_B_INT_IRQ; 
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}
/*
 * 函数名：_485_Config
 * 描述  ：USART GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
//void USART_485_B_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;

//	/* config USART clock */
//	RCC_AHB1PeriphClockCmd(USART_485_B_RX_GPIO_CLK|USART_485_B_TX_GPIO_CLK|_485_B_RE_GPIO_CLK, ENABLE);
//	RCC_APB1PeriphClockCmd(USART_485_B_CLK, ENABLE); 

//	// USARTx_A_APBxClkCmd(USARTx_A_CLK,ENABLE);
//	  /* Connect PXx to USARTx_Tx*/
//  GPIO_PinAFConfig(USART_485_B_RX_GPIO_PORT,USART_485_B_RX_SOURCE, USART_485_B_RX_AF);

//  /* Connect PXx to USARTx_Rx*/
//  GPIO_PinAFConfig(USART_485_B_TX_GPIO_PORT,USART_485_B_TX_SOURCE,USART_485_B_TX_AF);

//	
//	/* USART GPIO config */
//   /* Configure USART Tx as alternate function push-pull */
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

//  GPIO_InitStructure.GPIO_Pin = USART_485_B_TX_PIN  ;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(USART_485_B_TX_GPIO_PORT, &GPIO_InitStructure);

//  /* Configure USART Rx as alternate function  */
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
//	GPIO_InitStructure.GPIO_Pin = USART_485_B_RX_PIN;
//  GPIO_Init(USART_485_B_RX_GPIO_PORT, &GPIO_InitStructure);

//  
//  /* 485收发控制管脚 */
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_Pin = _485_B_RE_PIN  ;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(_485_B_RE_GPIO_PORT, &GPIO_InitStructure);
//	  
//	/* USART mode config */
//	USART_InitStructure.USART_BaudRate = USART_485_B_BAUDRATE;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No ;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

//	USART_Init(USART_485_B, &USART_InitStructure); 

//	
//	NVIC_485_B_Configuration();
//	/* 使能串口接收中断 */
////	USART_ITConfig(USART_485_B, USART_IT_RXNE, ENABLE);
////	USART_ITConfig(USART_485_B, USART_IT_IDLE, ENABLE);	
//	USART_Cmd(USART_485_B, ENABLE);
//	p_rx_queue_init_all(USART_485_B);
//	
//	_485_B_RX_EN(); //默认进入接收模式
//}




//void _485_B_IRQHandler(void)
//{
//    bsp_485_B_IRQHandler();

//}



//extern QUEUE_DATA_BUFF GV_qdf_485_B;




//void bsp_485_B_IRQHandler(void)
//{

//		uint8_t ucCh;
//	Q_QUEUE_DATA_TYPE *data_p; 
//	
//	if(USART_GetITStatus(USART_485_B,USART_IT_RXNE)!=RESET)
//	{	
//		ucCh  = USART_ReceiveData( USART_485_B );
//		//Usart_SendByte( USARTx_A, ucCh);
//						/*获取写缓冲区指针，准备写入新数据*/
//		data_p = p_cbWrite(&GV_qdf_485_B.q_rx_queue); 
//		
//		if (data_p != NULL)	//若缓冲队列未满，开始传输
//		{		

//			//往缓冲区写入数据，如使用串口接收、dma写入等方式
//			*(data_p->q_head + data_p->q_len) = ucCh;
//				
//			if( ++data_p->q_len >= q_QUEUE_NODE_DATA_LEN)
//			{
//				p_cbWriteFinish(&GV_qdf_485_B.q_rx_queue);
//			}
//		}else return;	
//	}
//	
//	if ( USART_GetITStatus( USART_485_B, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
//	{
//			/*写入缓冲区完毕*/
//			p_cbWriteFinish(&GV_qdf_485_B.q_rx_queue);
//		ucCh = USART_ReceiveData( USART_485_B );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)

//	}	
//	
//	
//	
//}
#include "./pro_com/usart485_common_verify.h"



void sd_usart_cmd_Option(void){
	
//_485_B_TX_EN();
//Usart_SendString( USART_485_B,"<Option:>");
	const char *sendStr = "<Option:>";
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, strlen(sendStr));
//_485_B_RX_EN();
	
}

void sd_usart_cmd_Exit(void){
	
//_485_B_TX_EN();
//Usart_SendString( USART_485_B,"<Exit:>");
//_485_B_RX_EN();
		const char *sendStr = "<Exit:>";
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, strlen(sendStr));
}

void sd_usart_cmd_ReadData(void){
	
//_485_B_TX_EN();
//Usart_SendString( USART_485_B,"<Read:>");
//_485_B_RX_EN();
			const char *sendStr = "<Read:>";
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, strlen(sendStr));
}

void sd_usart_cmd_ReadTime(void){
	
//_485_B_TX_EN();
//Usart_SendString( USART_485_B,"<Time:>");
//_485_B_RX_EN();
				const char *sendStr = "<Time:>";
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, strlen(sendStr));
}







char* formatDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    static char formattedDateTime[30]; // 预分配足够的空间存储字符串
//	uint8_t a=sizeof("<SetTime:2023-06-25 15:15:15>");
//	SYSTEM_DEBUG("rSetTime  %d ******",a);
	snprintf(formattedDateTime, sizeof(formattedDateTime), "<SetTime:20%02d-%02d-%02d %02d:%02d:%02d>",
             year, month, day, hour, minute, second);
	
	SYSTEM_DEBUG("rSetTime ---:  %s ******",formattedDateTime);
    return formattedDateTime;
}



char* formatDateTitle(char* time,char* workmode) {
    static char formattedTitle[50]; // 预分配足够的空间存储字符串
	
    snprintf(formattedTitle, sizeof(formattedTitle), "<Title:%s,%s>", time, workmode);
    return formattedTitle;
    
}


//char* formatTitleDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
//    static char formattedDateTime[30]; // 预分配足够的空间存储字符串
////	uint8_t a=sizeof("<SetTime:2023-06-25 15:15:15>");
////	SYSTEM_DEBUG("rSetTime  %d ******",a);
//	snprintf(formattedDateTime, sizeof(formattedDateTime), "%04d-%02d-%02d %02d:%02d:%02d",
//             year, month, day, hour, minute, second);
//    return formattedDateTime;
//}


char* formatDateTimeToString(uint8_t hour, uint8_t minute, uint8_t second) {
    static char formattedDateTime[20]; // 预分配足够的空间存储字符串
//	uint8_t a=sizeof("<SetTime:2023-06-25 15:15:15>");
//	SYSTEM_DEBUG("rSetTime  %d ******",a);
	snprintf(formattedDateTime, sizeof(formattedDateTime), "%02d:%02d:%02d",hour, minute, second);
	
	
    return formattedDateTime;
}




char* formatTitle(char* workmode) {
    static char formattedTitle[30]; // 预分配足够的空间存储字符串
	
    snprintf(formattedTitle, sizeof(formattedTitle), "<Title:%s>", workmode);
    return formattedTitle;
    
}


//sd_usart_cmd_SetTime(2011,1,1,1,1,1);
void sd_usart_cmd_SetTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second){
	
//_485_B_TX_EN();
//Usart_SendString( USART_485_B,formatDateTime(year, month, day, hour, minute, second));
//	
//_485_B_RX_EN();
					const char *sendStr = formatDateTime(year, month, day, hour, minute, second);
	uint16_t sendLen = strlen(sendStr);
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, sendLen);
}

//sd_usart_cmd_SetTitle("2020-01-02 03:59:55","工作模式");


void sd_usart_cmd_SetTitle_bystr(char* workmode){

//_485_B_TX_EN();
//Usart_SendString( USART_485_B,formatTitle(workmode));
//_485_B_RX_EN();
						const char *sendStr = formatTitle(workmode);
	uint16_t sendLen = strlen(sendStr);
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, sendLen);
}


void sd_usart_cmd_SetTitle_bytimestr(char* time,char* workmode){

//_485_B_TX_EN();
//Usart_SendString( USART_485_B,formatDateTitle(time,workmode));
//_485_B_RX_EN();
							const char *sendStr = formatDateTitle(time,workmode);
	uint16_t sendLen = strlen(sendStr);
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, sendLen);
}


void sd_usart_cmd_SetTitle_bynum(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second,char* workmode){

//sd_usart_cmd_SetTitle_bystr( formatTitleDateTime(year,  month,  day,  hour,  minute,  second),workmode);
	
}






void sd_usart_cmd_SetItem1(void){
	
//_485_B_TX_EN();
//Usart_SendString( USART_485_B,"<Item:日期,时间,工作模式,工作电压(v),工作电流(A),容量(Ah)>");
//_485_B_RX_EN();
								const char *sendStr = "<Item:日期,时间,工作模式,工作电压(v),工作电流(A),容量(Ah)>";
	uint16_t sendLen = strlen(sendStr);
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, sendLen);
}

void sd_usart_cmd_SetItem(void){
	
//_485_B_TX_EN();
//Usart_SendString( USART_485_B,"<Item:日期,时间,运行状态,信息,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P"
//	"12>");
//_485_B_RX_EN();
									const char *sendStr = "<Item:日期,时间,运行状态,信息,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P"
	"12>";
	uint16_t sendLen = strlen(sendStr);
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, sendLen);
}



//<Data:23.5,70.3,12.4,22.9,1.23,有,东东南>
void sd_usart_cmd_SetData1(void){
	
//_485_B_TX_EN();
//Usart_SendString( USART_485_B,"<Data:2020,工作模式1,30(s),50(v),20(A),30(Ah)>");
//_485_B_RX_EN();
										const char *sendStr = "<Data:2020,工作模式1,30(s),50(v),20(A),30(Ah)>";
	uint16_t sendLen = strlen(sendStr);
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, sendLen);
}

//#include "./defines.h"
#include "./global/GV_enum.h" 
char* formatSD_Date(char* runtime,char* workmode,uint8_t batgroup,uint16_t* workdata,uint8_t showmode) {
    static char formattedDate[220]; // 预分配足够的空间存储字符串
	//char* formattedDate1="<Data:2020-01-02 03:59:55,串充,03:59:55,65535,65535,400>";
	memset(formattedDate, '\0', sizeof(formattedDate));
	
	if(EM_SHOW_VOL==showmode){
	    snprintf(formattedDate, sizeof(formattedDate), "<Data:%s,电池组%d电压(v),%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d>",  
			workmode,batgroup,
		workdata[0]/ 1000,workdata[0]%1000,workdata[1]/ 1000,workdata[1]%1000,workdata[2]/ 1000,workdata[2]%1000,workdata[3]/ 1000,workdata[3]%1000,workdata[4]/ 1000,workdata[4]%1000,workdata[5]/ 1000,workdata[5]%1000,workdata[6]/ 1000
		,workdata[6]%1000,workdata[7]/ 1000,workdata[7]%1000,workdata[8]/ 1000,workdata[8]%1000,workdata[9]/ 1000,workdata[9]%1000,workdata[10]/ 1000,workdata[10]%1000,workdata[11]/ 1000,workdata[11]%1000);//电压
	
	}else if(EM_SHOW_CURRENT==showmode){
		    snprintf(formattedDate, sizeof(formattedDate), "<Data:%s,电池组%d电流(A),%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d>",  
			workmode,batgroup,
		workdata[0]/ 100,workdata[0]%100,workdata[1]/ 100,workdata[1]%100,workdata[2]/ 100,workdata[2]%100,workdata[3]/ 100,workdata[3]%100,workdata[4]/ 100,workdata[4]%100,workdata[5]/ 100,workdata[5]%100,workdata[6]/ 100
		,workdata[6]%100,workdata[7]/ 100,workdata[7]%100,workdata[8]/ 100,workdata[8]%100,workdata[9]/ 100,workdata[9]%100,workdata[10]/ 100,workdata[10]%100,workdata[11]/ 100,workdata[11]%100);//电压
	
	
	}else if(EM_SHOW_CAPICITY==showmode){
//		workdata[0]=-3;workdata[1]=-23;workdata[2]=-33;workdata[3]=-43;
		    snprintf(formattedDate, sizeof(formattedDate), "<Data:%s,电池组%d容量(Ah),%d.%1d,%d.%1d,%d.%01d,%d.%1d,%d.%1d,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d>",  
			workmode,batgroup,
		((int16_t)workdata[0])/10,abs((int16_t)workdata[0])%10,
		((int16_t)workdata[1])/10,abs((int16_t)workdata[1])%10,
		((int16_t)workdata[2])/10,abs((int16_t)workdata[2])%10,
		((int16_t)workdata[3])/10,abs((int16_t)workdata[3])%10,
		((int16_t)workdata[4])/10,abs((int16_t)workdata[4])%10,
		((int16_t)workdata[5])/10,abs((int16_t)workdata[5])%10,
		((int16_t)workdata[6])/10,abs((int16_t)workdata[6])%10,
		((int16_t)workdata[7])/10,abs((int16_t)workdata[7])%10,
		((int16_t)workdata[8])/10,abs((int16_t)workdata[8])%10,
		((int16_t)workdata[9])/10,abs((int16_t)workdata[9])%10,
		((int16_t)workdata[10])/10,abs((int16_t)workdata[10])%10,
		((int16_t)workdata[11])/10,abs((int16_t)workdata[11])%10);//电压
	
	
	}else{
			    snprintf(formattedDate, sizeof(formattedDate), "<Data:%s,测试%d忽略,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d,%d.%01d>",  
			workmode,batgroup,
		0,0,0,0,0,0,0,0,0,0,0,0,0
		,0,0,0,0,0,0,0,0,0,0,0);//电压
	
	
	
	}

		
		
		
		
		
		
		
    return formattedDate;
    
}



void sd_usart_cmd_SetData(char* runtime,char* workmode,uint8_t batgroup,uint16_t* workdata,uint8_t showmode){
	
//_485_B_TX_EN();
//Usart_SendString( USART_485_B,formatSD_Date( runtime,workmode,batgroup, workdata, showmode));
////Usart_SendString( USART_485_B,formatSD_Date(  "串充","03:59:55",batgroup, workdata, showmode));

//_485_B_RX_EN();
											const char *sendStr = formatSD_Date( runtime,workmode,batgroup, workdata, showmode);
	uint16_t sendLen = strlen(sendStr);
	Usart485CommonComTask(&COM05_com485Inst, (uint8_t *)sendStr, sendLen);
}






/*********************************************END OF FILE**********************/

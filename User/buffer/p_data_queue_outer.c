/**
  ******************************************************************************
  * @file    p_data_queue.c
  * @author  wmap
  * @version V1.0
  * @date    2024-12-24
  * @brief   环形缓冲区，适用于接收外部数据时用作缓冲
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 IOT STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "./buffer/p_data_queue_outer.h"
#include "./sys/sysio.h"
#include "./usart/bsp_usart.h"
#include "./user_config.h"

//不要删，这是传统版本定义
#if 0
// 预先定义全局一维数组用于不同情况的 Q_QueueBuffer 结构体中的 elems 指针指向
Q_QUEUE_DATA_TYPE global_elems_1[q_QUEUE_NODE_NUM_1];
//Q_QUEUE_DATA_TYPE global_elems_2[q_QUEUE_NODE_NUM_2];

// 预先定义全局二维数组用于不同情况的 NODE_DATA_BUFF 结构体中的 node_buff 指针指向
char global_node_buff_1[q_QUEUE_NODE_NUM_1][q_QUEUE_NODE_DATA_LEN_1];
//char global_node_buff_2[q_QUEUE_NODE_NUM_2][q_QUEUE_NODE_DATA_LEN_2];

Q_QUEUE_DATA_TYPE  node_data_1[q_QUEUE_NODE_NUM_1];
//Q_QUEUE_DATA_TYPE  node_data_2[q_QUEUE_NODE_NUM_2];


Q_QueueBuffer q_tx_rx_queue_1;
NODE_DATA_BUFF GV_ndb_1;
#endif

//Q_QueueBuffer q_rx_queue_2;
//NODE_DATA_BUFF GV_ndb_2;

CREAT_Group_queue(2,q_QUEUE_NODE_NUM_2,q_QUEUE_NODE_DATA_LEN_2);

CREAT_Group_queue(UsartDMAsend,q_QUEUE_NODE_NUM_UsartDMAsend,q_QUEUE_NODE_DATA_LEN_UsartDMAsend);


CREAT_Group_queue(GROUP_1,q_QUEUE_NODE_NUM_GROUP_1,q_QUEUE_NODE_DATA_LEN_GROUP_1);
CREAT_Group_queue(GROUP_2,q_QUEUE_NODE_NUM_GROUP_2,q_QUEUE_NODE_DATA_LEN_GROUP_2);

//extern Q_QueueBuffer q_rx_queue_1;
//CREAT_GROUP_QUEUE_NEW(GROUP_3,q_QUEUE_NODE_NUM_GROUP_3,q_QUEUE_NODE_DATA_LEN_GROUP_3);



void p_tx_rx_groupedqueue_init_all(UART_HandleTypeDef* uartHandle){
	#if 0
if(uartHandle->Instance == USART_A)
{
	//SYSTEM_DEBUG(" ==xxxxx====1");
	p_tx_rx_groupedqueue_init(&q_rx_queue_1,&GV_ndb_1,q_QUEUE_NODE_NUM_1,q_QUEUE_NODE_DATA_LEN_1,global_elems_1,node_data_1,global_node_buff_1);

	//p_tx_rx_groupedqueue_init(QUEUE_DATA_BUFF *qdf, NODE_DATA_BUFF *ndb, int qnn_size,int qnd_length);
}
#else
if(uartHandle->Instance == USART_A)
{
	//SYSTEM_DEBUG(" ==xxxxx====1");
	p_tx_rx_groupedqueue_init(&q_tx_rx_queue_2,&GV_ndb_2,q_QUEUE_NODE_NUM_2,q_QUEUE_NODE_DATA_LEN_2,global_elems_2,node_data_2,global_node_buff_2);

	//p_tx_rx_groupedqueue_init(QUEUE_DATA_BUFF *qdf, NODE_DATA_BUFF *ndb, int qnn_size,int qnd_length);
}

#endif

//


}

//CREAT_Group_queue(UsartDMAsend,q_QUEUE_NODE_NUM_UsartDMAsend,q_QUEUE_NODE_DATA_LEN_UsartDMAsend);



void p_tx_rx_groupedqueue_init_dma(void){

p_tx_rx_groupedqueue_init(&q_tx_rx_queue_UsartDMAsend,&GV_ndb_UsartDMAsend,q_QUEUE_NODE_NUM_UsartDMAsend,
	q_QUEUE_NODE_DATA_LEN_UsartDMAsend,global_elems_UsartDMAsend,node_data_UsartDMAsend,global_node_buff_UsartDMAsend);

}
//#define CREAT_GROUP_QUEUE_NEW(que_name, node_num,node_len)	\
//Q_QUEUE_DATA_TYPE global_elems__##que_name[node_num];	\
//char global_node_buff__##que_name[node_num][node_len];	\
//Q_QUEUE_DATA_TYPE  node_data__##que_name[node_num];	\
//Q_QueueBuffer q_tx_rx_queue__##que_name;	\
//NODE_DATA_BUFF GV_ndb__##que__name;	

//CREAT_Group_queue(GROUP_1,q_QUEUE_NODE_NUM_GROUP_1,q_QUEUE_NODE_DATA_LEN_GROUP_1);
//CREAT_Group_queue(GROUP_2,q_QUEUE_NODE_NUM_GROUP_2,q_QUEUE_NODE_DATA_LEN_GROUP_2);



void P_queue_init_COMMON_TX_GROUPED_BUFF_dma(void){

p_tx_rx_groupedqueue_init(&q_GROUPqueue_1_UsartDMAsend,&GV_ndb_GROUP_1,q_QUEUE_NODE_NUM_GROUP_1,
	q_QUEUE_NODE_DATA_LEN_GROUP_1,global_elems_GROUP_1,node_data_GROUP_1,global_node_buff_GROUP_1);
	
	
p_tx_rx_groupedqueue_init(&q_GROUPqueue_2_UsartDMAsend,&GV_ndb_GROUP_2,q_QUEUE_NODE_NUM_GROUP_2,
	q_QUEUE_NODE_DATA_LEN_GROUP_2,global_elems_GROUP_2,node_data_GROUP_2,global_node_buff_GROUP_2);
	
	
//p_tx_rx_groupedqueue_init(&q_GROUPqueue_3_UsartDMAsend,&GV_ndb__GROUP_3,q_QUEUE_NODE_NUM__GROUP_3,
//	q_QUEUE_NODE_DATA_LEN_GROUP_2,global_elems_GROUP_2,node_data_GROUP_2,global_node_buff_GROUP_2);	
	
	
	
	
}






void Test_get_all_data_from_queue(UART_HandleTypeDef* uartHandle){


if(uartHandle->Instance == USART_A)
{
	p_pull_data_from_queue(&q_tx_rx_queue_2);

	//p_cbPrint_buff(GV_ndb_1.node_buff[0]);
	//SYSTEM_DEBUG(" 0: %d = %d = %d = %d ",GV_ndb_1.node_buff[0][0],GV_ndb_1.node_buff[0][1],GV_ndb_1.node_buff[0][2],GV_ndb_1.node_buff[0][3]);

}

}



void Test_all_queue(void){
//p_pull_data_from_queue(&q_rx_queue_1);
p_pull_data_from_queue(&q_tx_rx_queue_2);
//p_pull_data_from_queue(&q_tx_rx_queue_3);

	
}







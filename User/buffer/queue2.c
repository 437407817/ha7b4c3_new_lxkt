/**
  ******************************************************************************
  * @file    rx_data_queue.c
  * @author  fire
  * @version V1.0
  * @date    2015-01-xx
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

#include "./buffer/queue2.h"
#include "./sys/sysio.h"






#include <stdint.h>
#include <stdbool.h>






/**
 * @brief 初始化环形缓冲区
 * @param rb：环形缓冲区指针
 * @param buf：缓冲区数组
 * @param size：缓冲区大小
 */
// void QueueInit(QueueType_t *queue, uint8_t *buffer, uint32_t size)
 void RingBuffer_Init(RingBuffer_t *rb, uint8_t *buf, uint16_t size) {
    rb->buf = buf;
    rb->size = size;
    rb->read_idx = 0;
    rb->write_idx = 0;
}

/**
 * @brief 判断环形缓冲区是否为空
 * @param rb：环形缓冲区指针
 * @return 空返回true，否则返回false
 */
 bool RingBuffer_IsEmpty(RingBuffer_t *rb) {
    return (rb->read_idx == rb->write_idx);
}

/**
 * @brief 判断环形缓冲区是否为满
 * @param rb：环形缓冲区指针
 * @return 满返回true，否则返回false
 */
 bool RingBuffer_IsFull(RingBuffer_t *rb) {
    return ((rb->write_idx + 1) % rb->size) == rb->read_idx;
}

/**
 * @brief 向环形缓冲区写入一个字节
 * @param rb：环形缓冲区指针
 * @param data：待写入数据
 * @return 写入成功返回true，缓冲区满返回false
 */
//QueueStatus_t QueuePush(QueueType_t *queue, uint8_t data)
 bool RingBuffer_WriteByte(RingBuffer_t *rb, uint8_t data) {
    if (RingBuffer_IsFull(rb)) {
        return false; // 缓冲区满，写入失败
    }
    rb->buf[rb->write_idx] = data;
    rb->write_idx = (rb->write_idx + 1) % rb->size; // 写指针循环移动
    return true;
}

/**
 * @brief 从环形缓冲区读取一个字节
 * @param rb：环形缓冲区指针
 * @param data：读取到的数据（输出参数）
 * @return 读取成功返回true，缓冲区空返回false
 */
//QueueStatus_t QueuePop(QueueType_t *queue, uint8_t *pdata)
 bool RingBuffer_ReadByte(RingBuffer_t *rb, uint8_t *data) {
    if (RingBuffer_IsEmpty(rb)) {
        return false; // 缓冲区空，读取失败
    }
    *data = rb->buf[rb->read_idx];
    rb->read_idx = (rb->read_idx + 1) % rb->size; // 读指针循环移动
    return true;
}

/**
 * @brief 向环形缓冲区批量写入数据
 * @param rb：环形缓冲区指针
 * @param data：待写入数据数组
 * @param len：待写入数据长度
 */
//uint32_t QueuePushArray(QueueType_t *queue, uint8_t *pArray, uint32_t len)
 void RingBuffer_WriteMulti(RingBuffer_t *rb, const uint8_t *data, uint16_t len) {
//    __disable_irq(); // 禁止中断，防止读写冲突
    for (uint16_t i = 0; i < len; i++) {
        if (!RingBuffer_WriteByte(rb, data[i])) {
            break; // 缓冲区满，停止写入
        }
    }
//    __enable_irq(); // 恢复中断
}


/**
 * @brief 获取环形缓冲区中可用数据量（可读取的字节数）
 * @param rb：环形缓冲区指针
 * @return 可用数据量（字节数）
 */
//uint32_t QueueCount(QueueType_t *queue)
 uint16_t RingBuffer_GetAvailable(RingBuffer_t *rb) {
    if (rb == NULL) return 0;
    if (rb->write_idx >= rb->read_idx) {
        // 写指针在 read_idx 之后：可用数据 = write_idx - read_idx
        return rb->write_idx - rb->read_idx;
    } else {
        // 写指针在 read_idx 之前（绕圈）：可用数据 = (size - read_idx) + write_idx
        return (rb->size - rb->read_idx) + rb->write_idx;
    }
}

/**
 * @brief 从环形缓冲区批量读取数据（核心新增方法）
 * @param rb：环形缓冲区指针
 * @param buf：存储读取数据的缓冲区
 * @param len：期望读取的字节数
 * @return 实际读取的字节数（<= len）
 */
//uint32_t QueuePopArray(QueueType_t *queue, uint8_t *pArray, uint32_t len)
 uint16_t RingBuffer_ReadMulti(RingBuffer_t *rb, uint8_t *buf, uint16_t len) {
    if (rb == NULL || buf == NULL || len == 0) return 0;
    
    uint16_t available = RingBuffer_GetAvailable(rb);  // 获取可用数据量
    uint16_t read_len = (available < len) ? available : len;  // 实际读取长度（不超过可用量）
    if (read_len == 0) return 0;
    
//    __disable_irq();  // 禁止中断，防止读取过程中被写入干扰
    
    if (rb->read_idx + read_len <= rb->size) {
        // 情况1：读取区域不跨缓冲区边界（连续地址）
        memcpy(buf, &rb->buf[rb->read_idx], read_len);
        rb->read_idx += read_len;
    } else {
        // 情况2：读取区域跨缓冲区边界（需分两段复制）
        uint16_t first_part = rb->size - rb->read_idx;  // 第一段长度（从read_idx到缓冲区末尾）
        memcpy(buf, &rb->buf[rb->read_idx], first_part);
        
        uint16_t second_part = read_len - first_part;   // 第二段长度（从缓冲区开头到剩余长度）
        memcpy(&buf[first_part], rb->buf, second_part);
        
        rb->read_idx = second_part;  // 更新读指针（绕回开头）
    }
    
//    __enable_irq();   // 恢复中断
    return read_len;
}


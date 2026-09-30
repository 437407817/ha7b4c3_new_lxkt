#include "./service/Workled.h"

#include "./sys/bsp_systime.h"   


 #include "./sys/sysio.h"
  

#include "./io/bsp_io_output.h" 




#include <stdint.h>

// 定义最多支持的灯数量
#define MAX_LIGHTS 3

// 记录每个灯的打开次数（计数器）
static uint8_t lightCounters[MAX_LIGHTS] = {0};

/**
 * 打开指定编号的灯（支持计数）
 * @param lightNumber 灯的编号（1-5）
 * @return 操作结果：0-成功，非0-失败
 */
uint8_t turnOnLight(uint8_t lightNumber) {
    // 检查输入是否在有效范围内
    if (lightNumber < 1 || lightNumber > MAX_LIGHTS) {
        return 1; // 返回错误码
    }
    
    // 获取实际数组索引（灯1对应索引0）
    uint8_t index = lightNumber - 1;
    
    // 增加计数器（防止溢出）
    if (lightCounters[index] < 255) {
        lightCounters[index]++;
    }
    
    // 更新硬件状态（如果计数器大于0则点亮）
    updateHardwareLightState(index, lightCounters[index] > 0);
    
    return 0; // 操作成功
}

/**
 * 关闭指定编号的灯（支持计数）
 * @param lightNumber 灯的编号（1-5）
 * @return 操作结果：0-成功，非0-失败
 */
uint8_t turnOffLight(uint8_t lightNumber) {
    // 检查输入是否在有效范围内
    if (lightNumber < 1 || lightNumber > MAX_LIGHTS) {
        return 1; // 返回错误码
    }
    
    // 获取实际数组索引
    uint8_t index = lightNumber - 1;
    
    // 减少计数器（不能小于0）
    if (lightCounters[index] > 0) {
        lightCounters[index]--;
    }
    
    // 更新硬件状态（如果计数器为0则熄灭）
    updateHardwareLightState(index, lightCounters[index] > 0);
    
    return 0; // 操作成功
}

/**
 * 关闭所有灯（不考虑计数，强制关闭）
 * @return 操作结果：0-成功，非0-失败
 */
uint8_t turnOffAllLights(void) {
    // 重置所有计数器为0
    for (int i = 0; i < MAX_LIGHTS; i++) {
        lightCounters[i] = 0;
    }
//    SYSTEM_DEBUG("----turnOffAllLights----");
		
//		    // 获取调用者的返回地址
//    void* caller_addr = __builtin_return_address(0);
//    
//    // 打印调用者地址（需要addr2line工具解析）
//    SYSTEM_DEBUG("turnOffAllLights() called from address: %p\n", caller_addr);
    // 更新所有灯的硬件状态为关闭
    updateAllHardwareLightsState(0);
    
    return 0; // 操作成功
}



/**
 * 获取指定灯的当前状态
 * @param lightNumber 灯的编号（1-5）
 * @return 灯的状态：0-关闭，1-打开
 */
uint8_t getLightState(uint8_t lightNumber) {
    if (lightNumber < 1 || lightNumber > MAX_LIGHTS) {
        return 0; // 错误情况返回关闭状态
    }
    uint8_t index = lightNumber - 1;
    return lightCounters[index] > 0;
}






// 以下为硬件操作接口（需要根据实际硬件实现）
void updateHardwareLightState(uint8_t lightNumber, uint8_t state) {
    // 实际硬件控制代码，例如：
     if (state) {
			 switch(lightNumber) {
				 case 0:Turnled_ON(1);break;// 点亮指定灯
			 	case 1:Turnled_ON(2);break; // 点亮指定灯
			 
			 	case 2:Turnled_ON(3); break;// 点亮指定灯
				 default:break;
			 
			 } 
         
     } else {
         			 switch(lightNumber) {
				 case 0:Turnled_OFF(1);break;// 点亮指定灯
			 	case 1:Turnled_OFF(2);break; // 点亮指定灯
			 
			 	case 2:Turnled_OFF(3); break;// 点亮指定灯
				 default:break;
			 
			 }  // 熄灭指定灯
     }
}


void updateAllHardwareLightsState(uint8_t stateMask) {
    for (uint8_t i = 0; i < MAX_LIGHTS; i++) {
        uint8_t state = (stateMask & (1 << i)) != 0;
        updateHardwareLightState(i, state);
    }
}

//void updateAllHardwareLightsState1(uint8_t stateMask) {
//    // 实际硬件控制代码，例如：
//     for (int i = 0; i < MAX_LIGHTS; i++) {
//         if (stateMask & (1 << i)) {
//           switch(i) {
//				 case 0:Turnled_ON(1);break;// 点亮指定灯
//			 	case 1:Turnled_ON(2);break; // 点亮指定灯
//			 
//			 	case 2:Turnled_ON(3); break;// 点亮指定灯
//				 default:break;
//			 
//			 } 
//         } else {
//         switch(i) {
//				 case 0:Turnled_OFF(1);break;// 点亮指定灯
//			 	case 1:Turnled_OFF(2);break; // 点亮指定灯
//			 
//			 	case 2:Turnled_OFF(3); break;// 点亮指定灯
//				 default:break;
//			 
//			 }  // 熄灭指定灯
//         }
//     }
//}
/**
 * 获取所有灯的状态
 * @return 所有灯的状态（每一位代表一个灯：bit0-灯1，bit1-灯2...）
 */
uint8_t getAllLightsState(void) {
    uint8_t state = 0;
    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (lightCounters[i] > 0) {
            state |= (1 << i);
        }
    }
		
		SYSTEM_DEBUG_BINARY(state,"ledbit = \n");
    return state;
}


void SYSTEM_getAllLightsStateCountersfun(void) {

	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,lightCounters,3,"LED STATE pin=  %d - %d - %d --\n",GetLed_STATE(1),GetLed_STATE(2),GetLed_STATE(3));
	getAllLightsState();
    return ;
}



/*********************************************END OF FILE**********************/

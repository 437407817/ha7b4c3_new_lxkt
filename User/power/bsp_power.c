/**
  ******************************************************************************
  * @file    bsp_power.c
  * @author  STMicroelectronics / Adapted for STM32H7 HAL
  * @version V1.0
  * @date    2026-xx-xx
  * @brief   PVD 底层应用函数 (STM32H7 HAL库)
  ******************************************************************************
  */
  
#include "./power/bsp_power.h"
#include "./sys/sysio.h"
#include "./service/EepromFun.h"
#include "./Legacy/stm32_hal_legacy.h"
//#include "stm32h7xx_hal_rcc.h"
 str_PowerDownState GV_PowerDownState;

/**
  * @brief  配置PVD电压监控模式
  * @param  None
  * @retval None
  */
/**
  * @brief  配置PVD电压监控模式
  * @param  None
  * @retval None
  */
void Init_Voltage_Monitoring_Config(void)
{
  PWR_PVDTypeDef sConfigPVD = {0};

  /* STM32H7 的 PWR 时钟默认开启，无需调用 __HAL_RCC_PWR_CLK_ENABLE() */

  /* 1. 配置PVD级别5 (阈值约2.8V，具体请查阅STM32H7参考手册) */
  sConfigPVD.PVDLevel = PWR_PVDLEVEL_5;
  sConfigPVD.Mode     = PWR_PVD_MODE_IT_RISING_FALLING;
  HAL_PWR_ConfigPVD(&sConfigPVD);

  /* 2. 配置并使能 PVD/AVD 中断 (STM32H7中对应 PVD_AVD_IRQn) */
  HAL_NVIC_SetPriority(PVD_AVD_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(PVD_AVD_IRQn);
}

/**
  * @brief  使能PVD监控
  * @param  None
  * @retval None
  */
void enable_pwr(void)
{
  HAL_PWR_EnablePVD();
}

/**
  * @brief  禁能PVD监控
  * @param  None
  * @retval None
  */
void disable_pwr(void)
{
  SYSTEM_INFO(" ==disable_pwr== \n");
  HAL_PWR_DisablePVD();
}

/**
  * @brief  PVD中断服务函数
  * @param  None
  * @retval None
  */
void PVD_AVD_IRQHandler(void)
{
  HAL_PWR_PVD_IRQHandler();
}

/**
  * @brief  PVD中断回调函数（当电压跨越阈值时触发）
  * @param  None
  * @retval None
  */
void HAL_PWR_PVDCallback(void)
{
  /* 检测是否产生了PVD警告信号（VDD低于阈值） */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO) != RESET)
  {
    /* 进入紧急状态处理，保存掉电前的数据 */
    GV_PowerDownState.PowerDown_State = 1;
    SavePowerDownData();
  }
}

/*********************************************END OF FILE**********************/
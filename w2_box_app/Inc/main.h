/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MAIN_LED_Pin GPIO_PIN_13
#define MAIN_LED_GPIO_Port GPIOC
#define PTC_CTRL_Pin GPIO_PIN_1
#define PTC_CTRL_GPIO_Port GPIOA
#define ST25R_IRQ_Pin GPIO_PIN_3
#define ST25R_IRQ_GPIO_Port GPIOA
#define ST25R_IRQ_EXTI_IRQn EXTI3_IRQn
#define ST25R_CS_Pin GPIO_PIN_4
#define ST25R_CS_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_0
#define OLED_DC_GPIO_Port GPIOB
#define TMEP_Pin GPIO_PIN_1
#define TMEP_GPIO_Port GPIOB
#define OLED_RES1_Pin GPIO_PIN_10
#define OLED_RES1_GPIO_Port GPIOB
#define OELD_RES2_Pin GPIO_PIN_11
#define OELD_RES2_GPIO_Port GPIOB
#define OLED_CS2_Pin GPIO_PIN_12
#define OLED_CS2_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_15
#define BEEP_GPIO_Port GPIOA
#define MOTOR_POWER_Pin GPIO_PIN_3
#define MOTOR_POWER_GPIO_Port GPIOB
#define LED_PWM_CTRL_Pin GPIO_PIN_4
#define LED_PWM_CTRL_GPIO_Port GPIOB
#define CS_FLASH_Pin GPIO_PIN_5
#define CS_FLASH_GPIO_Port GPIOB
#define HALL_Pin GPIO_PIN_6
#define HALL_GPIO_Port GPIOB
#define OLED_CS1_Pin GPIO_PIN_7
#define OLED_CS1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

typedef struct sys_info {
  uint16_t sys_temp;        //当前系统温度
  uint16_t sys_set_temp;    //设置的系统温度
  uint16_t sys_ev;          //系统电流
  uint8_t heat_state;
}sys_info_t;
extern sys_info_t g_sys_info;


typedef struct {
  uint8_t motor_time;
}timer_t;

typedef enum
{
  BOX_SUCCESS     = 0,
  BOX_FAIL      = 0X02,
  BOX_UNKNOW_CMD, 
  BOX_UNKNOW_PARAM,

  Android_SUCCESS   = 0X80,
  Android_FAIL    = 0X82,
  Android_UNKNOW_CMD, 
  Android_UNKNOW_PARAM,
}ret_msg_t;

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

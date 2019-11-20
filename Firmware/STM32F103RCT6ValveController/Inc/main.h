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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define ZW_RST_Pin GPIO_PIN_13
#define ZW_RST_GPIO_Port GPIOC
#define MOTOR4_SA_Pin GPIO_PIN_0
#define MOTOR4_SA_GPIO_Port GPIOC
#define MOTOR4_SB_Pin GPIO_PIN_1
#define MOTOR4_SB_GPIO_Port GPIOC
#define MOTOR3_SA_Pin GPIO_PIN_2
#define MOTOR3_SA_GPIO_Port GPIOC
#define MOTOR3_SB_Pin GPIO_PIN_3
#define MOTOR3_SB_GPIO_Port GPIOC
#define MOTO1_SB_Pin GPIO_PIN_0
#define MOTO1_SB_GPIO_Port GPIOA
#define MOTOR1_SA_Pin GPIO_PIN_1
#define MOTOR1_SA_GPIO_Port GPIOA
#define GSM_UART2_TX_Pin GPIO_PIN_2
#define GSM_UART2_TX_GPIO_Port GPIOA
#define GSM_UART2_RX_Pin GPIO_PIN_3
#define GSM_UART2_RX_GPIO_Port GPIOA
#define BT_1_Pin GPIO_PIN_4
#define BT_1_GPIO_Port GPIOA
#define BT_2_Pin GPIO_PIN_5
#define BT_2_GPIO_Port GPIOA
#define BT_3_Pin GPIO_PIN_6
#define BT_3_GPIO_Port GPIOA
#define ZM_INT0_Pin GPIO_PIN_7
#define ZM_INT0_GPIO_Port GPIOA
#define MOTOR2_SA_Pin GPIO_PIN_4
#define MOTOR2_SA_GPIO_Port GPIOC
#define MOTOR2_SB_Pin GPIO_PIN_5
#define MOTOR2_SB_GPIO_Port GPIOC
#define MOTOR1_A_Pin GPIO_PIN_0
#define MOTOR1_A_GPIO_Port GPIOB
#define MOTOR1_B_Pin GPIO_PIN_1
#define MOTOR1_B_GPIO_Port GPIOB
#define ZM_INT1_Pin GPIO_PIN_2
#define ZM_INT1_GPIO_Port GPIOB
#define EPR_SCL_Pin GPIO_PIN_10
#define EPR_SCL_GPIO_Port GPIOB
#define EPR_SDA_Pin GPIO_PIN_11
#define EPR_SDA_GPIO_Port GPIOB
#define MOTOR2_ENC_Pin GPIO_PIN_12
#define MOTOR2_ENC_GPIO_Port GPIOB
#define MOTOR1_ENC_Pin GPIO_PIN_13
#define MOTOR1_ENC_GPIO_Port GPIOB
#define MOTOR4_ENC_Pin GPIO_PIN_14
#define MOTOR4_ENC_GPIO_Port GPIOB
#define MOTOR3_ENC_Pin GPIO_PIN_15
#define MOTOR3_ENC_GPIO_Port GPIOB
#define GSM_RST_Pin GPIO_PIN_6
#define GSM_RST_GPIO_Port GPIOC
#define LED_DBG2_Pin GPIO_PIN_7
#define LED_DBG2_GPIO_Port GPIOC
#define LED_DBG1_Pin GPIO_PIN_8
#define LED_DBG1_GPIO_Port GPIOC
#define RS485_RE_Pin GPIO_PIN_9
#define RS485_RE_GPIO_Port GPIOC
#define RS485_DE_Pin GPIO_PIN_8
#define RS485_DE_GPIO_Port GPIOA
#define RS485_DI_Pin GPIO_PIN_9
#define RS485_DI_GPIO_Port GPIOA
#define RS485_RO_Pin GPIO_PIN_10
#define RS485_RO_GPIO_Port GPIOA
#define ESP_RST_Pin GPIO_PIN_15
#define ESP_RST_GPIO_Port GPIOA
#define ESP_TX_Pin GPIO_PIN_10
#define ESP_TX_GPIO_Port GPIOC
#define ESP_RX_Pin GPIO_PIN_11
#define ESP_RX_GPIO_Port GPIOC
#define ZW_TX_Pin GPIO_PIN_12
#define ZW_TX_GPIO_Port GPIOC
#define ZW_RX_Pin GPIO_PIN_2
#define ZW_RX_GPIO_Port GPIOD
#define ZW_RSTB3_Pin GPIO_PIN_3
#define ZW_RSTB3_GPIO_Port GPIOB
#define MOTOR2_A_Pin GPIO_PIN_4
#define MOTOR2_A_GPIO_Port GPIOB
#define MOTOR2_B_Pin GPIO_PIN_5
#define MOTOR2_B_GPIO_Port GPIOB
#define MOTOR3_A_Pin GPIO_PIN_6
#define MOTOR3_A_GPIO_Port GPIOB
#define MOTOR3_B_Pin GPIO_PIN_7
#define MOTOR3_B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef enum {
	xHAL_OK=0,
	xHAL_ERR
}hal_status_t;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

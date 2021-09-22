/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32wlxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "tim.h"
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void SDTest(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USART_BAUDRATE 115200
#define RTC_PREDIV_A ((1<<(15-RTC_N_PREDIV_S))-1)
#define RTC_N_PREDIV_S 10
#define RTC_PREDIV_S ((1<<RTC_N_PREDIV_S)-1)
#define AFE_CH2_Pin GPIO_PIN_15
#define AFE_CH2_GPIO_Port GPIOA
#define AFE_CH1_Pin GPIO_PIN_4
#define AFE_CH1_GPIO_Port GPIOB
#define SD_CS_Pin GPIO_PIN_9
#define SD_CS_GPIO_Port GPIOB
#define BTN_Pin GPIO_PIN_0
#define BTN_GPIO_Port GPIOA
#define BTN_EXTI_IRQn EXTI0_IRQn
#define LED_R_Pin GPIO_PIN_3
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_2
#define LED_G_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_10
#define LED_B_GPIO_Port GPIOB
#define RF_SW_CTRL1_Pin GPIO_PIN_4
#define RF_SW_CTRL1_GPIO_Port GPIOA
#define RF_SW_CTRL2_Pin GPIO_PIN_5
#define RF_SW_CTRL2_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#define GPIO2_Pin GPIO_PIN_7
#define GPIO2_GPIO_Port GPIOB
#define BATT_CHRG_Pin GPIO_PIN_3
#define BATT_CHRG_GPIO_Port GPIOB
#define BATT_CONN_Pin GPIO_PIN_9
#define BATT_CONN_GPIO_Port GPIOA
#define SD_SPI_HANDLE hspi2
#define ADC_HANDLE hadc
#define ADC_TIM_HANDLE htim2
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

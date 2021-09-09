/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    lora_app.c
  * @author  MCD Application Team
  * @brief   Application of the LRWAN Middleware
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

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "Region.h" /* Needed for LORAWAN_DEFAULT_DATA_RATE */
#include "sys_app.h"
#include "lora_app.h"
#include "stm32_seq.h"
#include "stm32_timer.h"
#include "utilities_def.h"
#include "lora_app_version.h"
#include "lorawan_version.h"
#include "subghz_phy_version.h"
#include "lora_info.h"
#include "LmHandler.h"
#include "stm32_lpm.h"
#include "adc_if.h"
#include "sys_conf.h"
#include "CayenneLpp.h"
#include "sys_sensors.h"

/* USER CODE BEGIN Includes */
#include "datalog.h"
#include "app_fatfs.h"
#include "stm32_lpm.h"
/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */
extern uint16_t sdcard_file_counter;
extern ADC_HandleTypeDef ADC_HANDLE;
extern TIM_HandleTypeDef ADC_TIM_HANDLE;
/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief LoRa State Machine states
  */
typedef enum TxEventType_e
{
  /**
    * @brief Appdata Transmission issue based on timer every TxDutyCycleTime
    */
  TX_ON_TIMER,
  /**
    * @brief Appdata Transmission external event plugged on OnSendEvent( )
    */
  TX_ON_EVENT
  /* USER CODE BEGIN TxEventType_t */

  /* USER CODE END TxEventType_t */
} TxEventType_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  LoRa End Node send request
  */
static void SendTxData(void);

/**
  * @brief  TX timer callback function
  * @param  context ptr of timer context
  */
static void OnTxTimerEvent(void *context);

/**
  * @brief  join event callback function
  * @param  joinParams status of join
  */
static void OnJoinRequest(LmHandlerJoinParams_t *joinParams);

/**
  * @brief  tx event callback function
  * @param  params status of last Tx
  */
static void OnTxData(LmHandlerTxParams_t *params);

/**
  * @brief callback when LoRa application has received a frame
  * @param appData data received in the last Rx
  * @param params status of last Rx
  */
static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params);

/*!
 * Will be called each time a Radio IRQ is handled by the MAC layer
 *
 */
static void OnMacProcessNotify(void);

/* USER CODE BEGIN PFP */
/**
  * @brief  LED Tx timer callback function
  * @param  LED context
  * @retval none
  */
static void OnTxTimerLedEvent(void *context);

/**
  * @brief  LED Rx timer callback function
  * @param  LED context
  * @retval none
  */
static void OnRxTimerLedEvent(void *context);
/**
  * @brief  LED Join timer callback function
  * @param  LED context
  * @retval none
  */
static void OnJoinTimerLedEvent(void *context);

void MicProcess(void);
void EnableSDLog(void);
void DisableSDLog(void);
void WriteSD(void);
/* USER CODE END PFP */

/* Private variables ---------------------------------------------------------*/
static ActivationType_t ActivationType = LORAWAN_DEFAULT_ACTIVATION_TYPE;

/**
  * @brief LoRaWAN handler Callbacks
  */
static LmHandlerCallbacks_t LmHandlerCallbacks =
{
  .GetBatteryLevel =           GetBatteryLevel,
  .GetTemperature =            GetTemperatureLevel,
  .GetUniqueId =               GetUniqueId,
  .GetDevAddr =                GetDevAddr,
  .OnMacProcess =              OnMacProcessNotify,
  .OnJoinRequest =             OnJoinRequest,
  .OnTxData =                  OnTxData,
  .OnRxData =                  OnRxData
};

/**
  * @brief LoRaWAN handler parameters
  */
static LmHandlerParams_t LmHandlerParams =
{
  .ActiveRegion =             ACTIVE_REGION,
  .DefaultClass =             LORAWAN_DEFAULT_CLASS,
  .AdrEnable =                LORAWAN_ADR_STATE,
  .TxDatarate =               LORAWAN_DEFAULT_DATA_RATE,
  .PingPeriodicity =          LORAWAN_DEFAULT_PING_SLOT_PERIODICITY
};

/**
  * @brief Type of Event to generate application Tx
  */
static TxEventType_t EventType = TX_ON_TIMER;

/**
  * @brief Timer to handle the application Tx
  */
static UTIL_TIMER_Object_t TxTimer;

/* USER CODE BEGIN PV */
/**
  * @brief Timer to handle the application Tx Led to toggle
  */
static UTIL_TIMER_Object_t TxLedTimer;

/**
  * @brief Timer to handle the application Rx Led to toggle
  */
static UTIL_TIMER_Object_t RxLedTimer;

/**
  * @brief Timer to handle the application Join Led to toggle
  */
static UTIL_TIMER_Object_t JoinLedTimer;

static uint8_t AppDataBuffer[LORAWAN_APP_DATA_BUFFER_MAX_SIZE];

/**
  * @brief User application data structure
  */
static LmHandlerAppData_t AppData = { 0, 0, AppDataBuffer };

/**
  * @brief Specifies the state of the application LED
  */
static uint8_t AppLedStateOn = RESET;

volatile  int  index_buff=0;
volatile uint8_t write_flag=0;
volatile uint32_t MsInBuff=0;

uint16_t Audio_OUT_Buff[SIZE_BUFF];
volatile uint16_t cont=0;
uint16_t PCM_Buffer[AUDIO_CHANNELS * AUDIO_SAMPLING_FREQUENCY / 1000];

uint16_t AudioInBuff0[AUDIO_IN_BUFFER_SIZE] = {0};
uint16_t counter =0;
/* USER CODE END PV */

/* Exported functions ---------------------------------------------------------*/
/* USER CODE BEGIN EF */

/* USER CODE END EF */

void LoRaWAN_Init(void)
{
  /* USER CODE BEGIN LoRaWAN_Init_1 */
  /* Get LoRa APP version*/
  APP_LOG(TS_OFF, VLEVEL_M, "APP_VERSION:        V%X.%X.%X\r\n",
          (uint8_t)(__LORA_APP_VERSION >> __APP_VERSION_MAIN_SHIFT),
          (uint8_t)(__LORA_APP_VERSION >> __APP_VERSION_SUB1_SHIFT),
          (uint8_t)(__LORA_APP_VERSION >> __APP_VERSION_SUB2_SHIFT));

  /* Get MW LoraWAN info */
  APP_LOG(TS_OFF, VLEVEL_M, "MW_LORAWAN_VERSION: V%X.%X.%X\r\n",
          (uint8_t)(__LORAWAN_VERSION >> __APP_VERSION_MAIN_SHIFT),
          (uint8_t)(__LORAWAN_VERSION >> __APP_VERSION_SUB1_SHIFT),
          (uint8_t)(__LORAWAN_VERSION >> __APP_VERSION_SUB2_SHIFT));

  /* Get MW SubGhz_Phy info */
  APP_LOG(TS_OFF, VLEVEL_M, "MW_RADIO_VERSION:   V%X.%X.%X\r\n",
          (uint8_t)(__SUBGHZ_PHY_VERSION >> __APP_VERSION_MAIN_SHIFT),
          (uint8_t)(__SUBGHZ_PHY_VERSION >> __APP_VERSION_SUB1_SHIFT),
          (uint8_t)(__SUBGHZ_PHY_VERSION >> __APP_VERSION_SUB2_SHIFT));
  
  UTIL_TIMER_Create(&TxLedTimer, 0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnTxTimerLedEvent, NULL);
  UTIL_TIMER_Create(&RxLedTimer, 0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnRxTimerLedEvent, NULL);
  UTIL_TIMER_Create(&JoinLedTimer, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnJoinTimerLedEvent, NULL);
  UTIL_TIMER_SetPeriod(&TxLedTimer, 50);
  UTIL_TIMER_SetPeriod(&RxLedTimer, 100);
  UTIL_TIMER_SetPeriod(&JoinLedTimer, 200);
  
  /* USER CODE END LoRaWAN_Init_1 */

  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_LmHandlerProcess), UTIL_SEQ_RFU, LmHandlerProcess);
  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_LoRaSendOnTxTimerOrButtonEvent), UTIL_SEQ_RFU, SendTxData);
  /* Init Info table used by LmHandler*/
  LoraInfo_Init();

  /* Init the Lora Stack*/
  LmHandlerInit(&LmHandlerCallbacks);

  LmHandlerConfigure(&LmHandlerParams);

  /* USER CODE BEGIN LoRaWAN_Init_2 */
  UTIL_TIMER_Start(&JoinLedTimer);
  /* USER CODE END LoRaWAN_Init_2 */

  LmHandlerJoin(ActivationType);

  if (EventType == TX_ON_TIMER)
  {
    /* send every time timer elapses */
    UTIL_TIMER_Create(&TxTimer,  0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnTxTimerEvent, NULL);
    UTIL_TIMER_SetPeriod(&TxTimer,  APP_TX_DUTYCYCLE);
    UTIL_TIMER_Start(&TxTimer);
  }
  else
  {
    /* USER CODE BEGIN LoRaWAN_Init_3 */
    /* USER CODE END LoRaWAN_Init_3 */
  }

  /* USER CODE BEGIN LoRaWAN_Init_Last */
  APP_LOG(TS_OFF, VLEVEL_M, "got here");
  UTIL_SEQ_RegTask( 1 << CFG_EnableSDLog, UTIL_SEQ_RFU, EnableSDLog);
  UTIL_SEQ_RegTask( 1 << CFG_DisableSDLog, UTIL_SEQ_RFU, DisableSDLog);
  UTIL_SEQ_RegTask( 1 << CFG_WriteSD, UTIL_SEQ_RFU, WriteSD);
  /* USER CODE END LoRaWAN_Init_Last */
}

/* USER CODE BEGIN PB_Callbacks */

/* USER CODE END PB_Callbacks */

/* Private functions ---------------------------------------------------------*/
/* USER CODE BEGIN PrFD */
void EnableSDLog(void)
{
  // first turn off the pull downs on the spi bus
  if(SD_Log_Enabled == 0)
  {
    // Do not allow low power mode while logging
    APP_LOG(TS_OFF, VLEVEL_L, "LPM CONFIG\r\n");
    UTIL_LPM_SetOffMode(1 << CFG_LPM_SDLOG, UTIL_LPM_DISABLE);
    UTIL_LPM_SetStopMode(1 << CFG_LPM_SDLOG, UTIL_LPM_DISABLE);
    //DeConfigureSDPullDown();

    // Set up FatFS SD Card
    //MX_FATFS_Init();
    //DATALOG_SD_Init();
    
    if(DATALOG_SD_Log_Enable())
    {
      SD_Log_Enabled = 1; // tells the callback to do the logging
    }
    else
    {
      HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);
      HAL_Delay(500);
      HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 1);
      HAL_Delay(500);
      HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);
      HAL_Delay(500);
      HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 1);
      DATALOG_SD_Log_Disable();
      DATALOG_SD_DeInit();
      return;
    }
    APP_LOG(TS_OFF, VLEVEL_L, "LOG EN\r\n");

    // Start ADC and Timer
    //HAL_ADC_Start_DMA(&ADC_HANDLE, (uint32_t*)AudioInBuff0, AUDIO_IN_BUFFER_SIZE);
    MX_ADC_Init();
    MX_TIM2_Init();
  
    HAL_ADC_Start_DMA(&ADC_HANDLE, (uint32_t*)AudioInBuff0, AUDIO_IN_BUFFER_SIZE);
    HAL_TIM_Base_Start(&ADC_TIM_HANDLE);
    APP_LOG(TS_OFF, VLEVEL_L, "adc start \r\n");
  }
  //UTIL_SEQ_SetTask( 1<<CFG_TASK_SW1_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);
  return;
}

void DisableSDLog(void)
{
  if(SD_Log_Enabled == 1)
  {
    // Stop ADC and Timer
    //HAL_ADC_Stop_DMA(&ADC_HANDLE);
    HAL_TIM_Base_Stop(&ADC_TIM_HANDLE);

    // Clear MsInBuff  and SD_Log_Enabled
    MsInBuff = 0;
    SD_Log_Enabled = 0;

    // Close off the SD log file 
    DATALOG_SD_Log_Disable();

    // Unmount SD Card and unlink driver
    //DATALOG_SD_DeInit();

    // Set pull downs
    //ConfigureSDPullDown();

    // Allow low power mode
    UTIL_LPM_SetOffMode(1 << CFG_LPM_SDLOG, UTIL_LPM_ENABLE);
    UTIL_LPM_SetStopMode(1 << CFG_LPM_SDLOG, UTIL_LPM_ENABLE);

    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 1);
  }
  return;
}

void WriteSD(void)
{
  if(MsInBuff > 0)
  	{
  		write_ms_on_sd();
  		__disable_irq();
  		MsInBuff--;
  		__enable_irq();
  	}
}

void MicProcess(void)
{
	uint16_t index = 0;
	static uint16_t OUT_Buff_lvl = 0;

	if (SD_Log_Enabled)
	{
		for (index = 0; index < PCM_SAMPLES_X_MS ; index++)
		{
			Audio_OUT_Buff[OUT_Buff_lvl] = PCM_Buffer[index];
			OUT_Buff_lvl = (OUT_Buff_lvl + 1)%SIZE_BUFF;
		}
		MsInBuff++;
    UTIL_SEQ_SetTask(1 << CFG_WriteSD, CFG_SEQ_Prio_0);
		//HAL_GPIO_TogglePin(Mon_GPIO_Port, Mon_Pin);
  }
}

void ADCHalfCycle(ADC_HandleTypeDef* hadc)
{
	uint32_t i, j = 0;
	//HAL_GPIO_WritePin(Mon_GPIO_Port, Mon_Pin, 1);
  //APP_LOG(TS_OFF, VLEVEL_L, "H\r\n");
	for(i=0; i < AUDIO_IN_BUFFER_SIZE/2; i+=2)
	{
    /*
		PCM_Buffer[j++] = AudioInBuff0[i];
		PCM_Buffer[j++] = AudioInBuff0[i+1];
    */
    PCM_Buffer[j++] = counter;
    PCM_Buffer[j++] = counter++;
	}
	MicProcess();
	//HAL_GPIO_WritePin(Mon_GPIO_Port, Mon_Pin, 0);
}

void ADCCpltCycle(ADC_HandleTypeDef* hadc)
{
	uint32_t i, j=0;

	//HAL_GPIO_WritePin(Mon_GPIO_Port, Mon_Pin, 1);
	for(i = AUDIO_IN_BUFFER_SIZE/2; i< AUDIO_IN_BUFFER_SIZE; i+=2)
	{
    /*
		PCM_Buffer[j++] = AudioInBuff0[i];
		PCM_Buffer[j++] = AudioInBuff0[i+1];
    */
    PCM_Buffer[j++] = counter;
    PCM_Buffer[j++] = counter++;
	}
	MicProcess();
	//HAL_GPIO_WritePin(Mon_GPIO_Port, Mon_Pin, 0);
}
/* USER CODE END PrFD */

static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params)
{
  /* USER CODE BEGIN OnRxData_1 */
  if ((appData != NULL) && (params != NULL))
  {
  // add in rx led turning on here
  UTIL_TIMER_Start(&RxLedTimer);

    static const char *slotStrings[] = { "1", "2", "C", "C Multicast", "B Ping-Slot", "B Multicast Ping-Slot" };

    APP_LOG(TS_OFF, VLEVEL_M, "\r\n###### ========== MCPS-Indication ==========\r\n");
    APP_LOG(TS_OFF, VLEVEL_H, "###### D/L FRAME:%04d | SLOT:%s | PORT:%d | DR:%d | RSSI:%d | SNR:%d\r\n",
            params->DownlinkCounter, slotStrings[params->RxSlot], appData->Port, params->Datarate, params->Rssi, params->Snr);
    switch (appData->Port)
    {
      case LORAWAN_SWITCH_CLASS_PORT:
        /*this port switches the class*/
        if (appData->BufferSize == 1)
        {
          switch (appData->Buffer[0])
          {
            case 0:
            {
              LmHandlerRequestClass(CLASS_A);
              break;
            }
            case 1:
            {
              LmHandlerRequestClass(CLASS_B);
              break;
            }
            case 2:
            {
              LmHandlerRequestClass(CLASS_C);
              break;
            }
            default:
              break;
          }
        }
        break;
      case LORAWAN_USER_APP_PORT:
        if (appData->BufferSize == 1)
        {
          /*
          AppLedStateOn = appData->Buffer[0] & 0x01;
          if (AppLedStateOn == RESET)
          {
            APP_LOG(TS_OFF, VLEVEL_L,   "LED OFF\r\n");
            HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin,1);
            // use this to swtich led
          }
           else
          {
            APP_LOG(TS_OFF, VLEVEL_L, "LED ON\r\n");
            HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin,0);
            //toggle off led
          }
          */
         APP_LOG(TS_OFF, VLEVEL_L, "Message %d\r\n", appData->Buffer[0]);
         switch (appData->Buffer[0])
         {
         case 48:
           APP_LOG(TS_OFF, VLEVEL_L,   "LED OFF Log End\r\n");
           UTIL_SEQ_SetTask(1 << CFG_DisableSDLog, CFG_SEQ_PRIO_1);
           HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin,1);
           break;
         
         case 49:
           APP_LOG(TS_OFF, VLEVEL_L,   "LED ON Log Start\r\n");
           UTIL_SEQ_SetTask(1 << CFG_EnableSDLog, CFG_SEQ_PRIO_1);
           HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin,0);
           break; 
           
         case 50:
           APP_LOG(TS_OFF, VLEVEL_L, "Fast Mode on\r\n");
           UTIL_TIMER_Stop(&TxTimer);
           UTIL_TIMER_SetPeriod(&TxTimer,  3000);
           UTIL_TIMER_Start(&TxTimer);
           break;
         case 51: 
           APP_LOG(TS_OFF, VLEVEL_L, "Fast Mode off\r\n");
           UTIL_TIMER_Stop(&TxTimer);
           UTIL_TIMER_SetPeriod(&TxTimer,  APP_TX_DUTYCYCLE);
           UTIL_TIMER_Start(&TxTimer);
         default:
           break;
         }
        }
          break;
        default:
        break;
    }
  }
  /* USER CODE END OnRxData_1 */
}

static void SendTxData(void)
{
  /* USER CODE BEGIN SendTxData_1 */
  uint16_t pressure = 0;
  int16_t temperature = 0;
  sensor_t sensor_data;
  UTIL_TIMER_Time_t nextTxIn = 0;

  uint16_t humidity = 0;
  uint32_t i = 0;
  int32_t latitude = 0;
  int32_t longitude = 0;
  uint16_t altitudeGps = 0;

  EnvSensors_Read(&sensor_data);
  //temperature = (SYS_GetTemperatureLevel() >> 8);
  pressure    = (uint16_t)(sensor_data.pressure * 100 / 10);      /* in hPa / 10 */

  AppData.Port = LORAWAN_USER_APP_PORT;
  humidity    = (uint16_t)(sensor_data.humidity * 10);            /* in %*10     */

  AppData.Buffer[i++] = AppLedStateOn;
  AppData.Buffer[i++] = (uint8_t)((pressure >> 8) & 0xFF);
  AppData.Buffer[i++] = (uint8_t)(pressure & 0xFF);
  AppData.Buffer[i++] = (uint8_t)(temperature & 0xFF);
  AppData.Buffer[i++] = (uint8_t)((humidity >> 8) & 0xFF);
  AppData.Buffer[i++] = (uint8_t)(humidity & 0xFF);

  if ((LmHandlerParams.ActiveRegion == LORAMAC_REGION_US915) || (LmHandlerParams.ActiveRegion == LORAMAC_REGION_AU915)
      || (LmHandlerParams.ActiveRegion == LORAMAC_REGION_AS923))
  {
    AppData.Buffer[i++] = 0;
    AppData.Buffer[i++] = 0;
    AppData.Buffer[i++] = 0;
    AppData.Buffer[i++] = 0;
  }
  else
  {
    latitude = sensor_data.latitude;
    longitude = sensor_data.longitude;

    AppData.Buffer[i++] = GetBatteryLevel();        /* 1 (very low) to 254 (fully charged) */
    AppData.Buffer[i++] = (uint8_t)((latitude >> 16) & 0xFF);
    AppData.Buffer[i++] = (uint8_t)((latitude >> 8) & 0xFF);
    AppData.Buffer[i++] = (uint8_t)(latitude & 0xFF);
    AppData.Buffer[i++] = (uint8_t)((longitude >> 16) & 0xFF);
    AppData.Buffer[i++] = (uint8_t)((longitude >> 8) & 0xFF);
    AppData.Buffer[i++] = (uint8_t)(longitude & 0xFF);
    AppData.Buffer[i++] = (uint8_t)((altitudeGps >> 8) & 0xFF);
    AppData.Buffer[i++] = (uint8_t)(altitudeGps & 0xFF);
  }

  AppData.BufferSize = i;
  if (LmHandlerSetTxPower(TX_POWER_14) == LORAMAC_HANDLER_ERROR)
  {
	APP_LOG(TS_OFF, VLEVEL_M, "\r\n###### ========== Didn't set tx power ==========\r\n");
  }

  if (LORAMAC_HANDLER_SUCCESS == LmHandlerSend(&AppData, LORAWAN_DEFAULT_CONFIRMED_MSG_STATE, &nextTxIn, false))
  {
    APP_LOG(TS_ON, VLEVEL_L, "SEND REQUEST\r\n");
  }
  else if (nextTxIn > 0)
  {
    APP_LOG(TS_ON, VLEVEL_L, "Next Tx in  : ~%d second(s)\r\n", (nextTxIn / 1000));
  }
  /* USER CODE END SendTxData_1 */
}

static void OnTxTimerEvent(void *context)
{
  /* USER CODE BEGIN OnTxTimerEvent_1 */

  /* USER CODE END OnTxTimerEvent_1 */
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_LoRaSendOnTxTimerOrButtonEvent), CFG_SEQ_Prio_0);

  /*Wait for next tx slot*/
  UTIL_TIMER_Start(&TxTimer);
  /* USER CODE BEGIN OnTxTimerEvent_2 */

  /* USER CODE END OnTxTimerEvent_2 */
}

/* USER CODE BEGIN PrFD_LedEvents */
static void OnTxTimerLedEvent(void *context)
{
  HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
}

static void OnRxTimerLedEvent(void *context)
{
  //toggle another led on main board
}

static void OnJoinTimerLedEvent(void *context)
{
  HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
}
/* USER CODE END PrFD_LedEvents */

static void OnTxData(LmHandlerTxParams_t *params)
{
  /* USER CODE BEGIN OnTxData_1 */
  if ((params != NULL) && (params->IsMcpsConfirm != 0))
  {
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
    UTIL_TIMER_Start(&TxLedTimer);

    APP_LOG(TS_OFF, VLEVEL_M, "\r\n###### ========== MCPS-Confirm =============\r\n");
    APP_LOG(TS_OFF, VLEVEL_H, "###### U/L FRAME:%04d | PORT:%d | DR:%d | PWR:%d", params->UplinkCounter,
            params->AppData.Port, params->Datarate, params->TxPower);

    APP_LOG(TS_OFF, VLEVEL_H, " | MSG TYPE:");
    if (params->MsgType == LORAMAC_HANDLER_CONFIRMED_MSG)
    {
      APP_LOG(TS_OFF, VLEVEL_H, "CONFIRMED [%s]\r\n", (params->AckReceived != 0) ? "ACK" : "NACK");
    }
    else
    {
      APP_LOG(TS_OFF, VLEVEL_H, "UNCONFIRMED\r\n");
    }
  }
  /* USER CODE END OnTxData_1 */
}

static void OnJoinRequest(LmHandlerJoinParams_t *joinParams)
{
  /* USER CODE BEGIN OnJoinRequest_1 */
  if (joinParams != NULL)
  {
    if (joinParams->Status == LORAMAC_HANDLER_SUCCESS)
    {
      UTIL_TIMER_Stop(&JoinLedTimer);

      HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);

      APP_LOG(TS_OFF, VLEVEL_M, "\r\n###### = JOINED = ");
      if (joinParams->Mode == ACTIVATION_TYPE_ABP)
      {
        APP_LOG(TS_OFF, VLEVEL_M, "ABP ======================\r\n");
      }
      else
      {
        APP_LOG(TS_OFF, VLEVEL_M, "OTAA =====================\r\n");
      }
    }
    else
    {
      APP_LOG(TS_OFF, VLEVEL_M, "\r\n###### = JOIN FAILED\r\n");
    }
  }      
  /* USER CODE END OnJoinRequest_1 */
}

static void OnMacProcessNotify(void)
{
  /* USER CODE BEGIN OnMacProcessNotify_1 */

  /* USER CODE END OnMacProcessNotify_1 */
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_LmHandlerProcess), CFG_SEQ_Prio_0);

  /* USER CODE BEGIN OnMacProcessNotify_2 */

  /* USER CODE END OnMacProcessNotify_2 */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

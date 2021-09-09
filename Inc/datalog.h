/*
 * datalog.h
 *
 *  Created on: Jan 27, 2021
 *      Author: clock
 */

#ifndef INC_DATALOG_H_
#define INC_DATALOG_H_



#endif /* INC_DATALOG_H_ */


#include "main.h"
#include "app_fatfs.h"


extern volatile uint8_t SD_Log_Enabled;
extern uint16_t sdcard_file_counter;

#define AUDIO_CHANNELS 2
#define AUDIO_SAMPLING_FREQUENCY 4000

#define PCM_SAMPLES_X_MS              	(AUDIO_SAMPLING_FREQUENCY/1000*AUDIO_CHANNELS)
#define PCM_BYTES_X_MS                	(PCM_SAMPLES_X_MS)
#define SIZE_BUFF                     	(PCM_SAMPLES_X_MS*250)

#define AUDIO_IN_BUFFER_SIZE 			(AUDIO_SAMPLING_FREQUENCY/1000*AUDIO_CHANNELS)*2


void DATALOG_SD_Init(void);
uint8_t DATALOG_SD_Log_Enable(void);
void DATALOG_SD_Log_Disable(void);
void DATALOG_SD_DeInit(void);
void write_on_sd(void);
void write_ms_on_sd(void);

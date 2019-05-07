#include "bsp_rtc.h"
#include "rtc.h"
#include <stdio.h>




RTC_HandleTypeDef rtc;

void bsp_rtc_init(void)
{
	RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
	
	
	/** Initialize RTC Only 
  */
  rtc.Instance = RTC;
  rtc.Init.HourFormat = RTC_HOURFORMAT_24;
  rtc.Init.AsynchPrediv = 127;
  rtc.Init.SynchPrediv = 255;
  rtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  rtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  rtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&rtc) != HAL_OK)
  {
    Error_Handler();
  }
	
	
	if(HAL_RTCEx_BKUPRead(&rtc,RTC_BKP_DR0)!=0x1312)
	{
		printf("RTC is not initialiert\r\n");
		
		/** Initialize RTC and set the Time and Date 
		*/
		sTime.Hours = 0x12;
		sTime.Minutes = 0x35;
		sTime.Seconds = 0x40;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		if (HAL_RTC_SetTime(&rtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
		{
			Error_Handler();
		}
		sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
		sDate.Month = RTC_MONTH_MAY;
		sDate.Date = 0x07;
		sDate.Year = 19;

		if (HAL_RTC_SetDate(&rtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
		{
			Error_Handler();
		}
		HAL_RTCEx_BKUPWrite(&rtc,RTC_BKP_DR0,0x1312);
	}
	else
	{
		printf("RTC is initialiert\r\n");
	}

}


void BSP_RTC_GetDate(void)
{
	RTC_DateTypeDef sdatestructure;
	HAL_RTC_GetDate(&rtc, &sdatestructure, RTC_FORMAT_BIN);
	
	printf("%02d/%02d/%02d\r\n",2000 + sdatestructure.Year, sdatestructure.Month, sdatestructure.Date);
	
}



void BSP_RTC_GetTime(void)
{
	RTC_TimeTypeDef stimestructure;
	HAL_RTC_GetTime(&rtc, &stimestructure, RTC_FORMAT_BIN);
	
	printf("%02d:%02d:%02d\r\n",stimestructure.Hours, stimestructure.Minutes, stimestructure.Seconds);
	
}




void BSP_RTC_SetDate(uint16_t year, uint8_t month, uint8_t date,uint8_t weekday)
{
	RTC_DateTypeDef sdatestructure;
	HAL_RTC_GetDate(&rtc, &sdatestructure, RTC_FORMAT_BIN);
	
	sdatestructure.Year=year;
	sdatestructure.Month=month;
	sdatestructure.Date=date;
	sdatestructure.WeekDay=weekday;
	
	HAL_RTC_SetDate(&rtc,&sdatestructure, RTC_FORMAT_BIN);
	
}



void BSP_RTC_SetTime(uint8_t hous,uint8_t minutes,uint8_t seconds)
{
	RTC_TimeTypeDef stimestructure;
	HAL_RTC_GetTime(&rtc, &stimestructure, RTC_FORMAT_BIN);
	
	stimestructure.Hours=hous;
	stimestructure.Minutes=minutes;
	stimestructure.Seconds=seconds;
	stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
	
	HAL_RTC_SetTime(&rtc, &stimestructure, RTC_FORMAT_BIN);
	
}


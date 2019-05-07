#ifndef BSP_RTC_H
#define BSP_RTC_H


#include "main.h"
void bsp_rtc_init(void);

void BSP_RTC_GetDate(void);
void BSP_RTC_GetTime(void);

void BSP_RTC_SetDate(uint16_t year, uint8_t month, uint8_t date,uint8_t weekday);
void BSP_RTC_SetTime(uint8_t hous,uint8_t minutes,uint8_t seconds);







#endif

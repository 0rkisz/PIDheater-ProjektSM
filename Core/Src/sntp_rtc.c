/*
 * sntp_rtc.h
 *
 *  Created on: Dec 31, 2023
 *      Author: Admin
 */

#include "sntp_rtc.h"

#include "rtc.h"
#include "usart.h"
#include <time.h>
#include <stdint.h>
#include <stdio.h>

time_t sntp_time;

void SNTP_SecondsSetRTC(uint64_t seconds)
{
	RTC_TimeTypeDef rtc_time;
	RTC_DateTypeDef rtc_date;
	sntp_time = seconds;

	struct tm sntp_clock;
	localtime_r(&sntp_time, &sntp_clock);
	rtc_time.Hours = sntp_clock.tm_hour;
	rtc_time.Minutes = sntp_clock.tm_min;
	rtc_time.Seconds = sntp_clock.tm_sec;
	rtc_time.SubSeconds = 0;
	rtc_time.DayLightSaving = (sntp_clock.tm_isdst)? RTC_DAYLIGHTSAVING_ADD1H : RTC_DAYLIGHTSAVING_NONE;

	rtc_date.Date = sntp_clock.tm_mday;
	rtc_date.Month = sntp_clock.tm_mon + 1;
	rtc_date.Year = sntp_clock.tm_year % 100;
	rtc_date.WeekDay = sntp_clock.tm_wday + 1;

	HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char c_time_string[80], serial_buffer[150];
	ctime_r(&sntp_time, c_time_string);
	int length = snprintf((char*)serial_buffer, 149,"SNTP UTC time received %s\r\n", c_time_string);
	HAL_UART_Transmit(&huart3, (uint8_t*)serial_buffer, length, 1000);
}

/*
 * sntp_rtc.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Admin
 */

#ifndef INC_SNTP_RTC_H_
#define INC_SNTP_RTC_H_

#include <stdint.h>

void SNTP_SecondsSetRTC(uint64_t seconds);
void SNTP_SecondsGetRTC(uint64_t* seconds, uint64_t* ms);

#endif /* INC_SNTP_RTC_H_ */

#ifndef __USER_MICROSD_H
#define __USER_MICROSD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ff.h"

//używając karty SD korzystamy tyllko z funkcji tutaj, musimy tylko na początku zrobić funkcje mount

void transmit_uart(char *string);

FRESULT user_f_delete_line (FATFS* fat, FIL* fp, const TCHAR* path, INT l_nr);
FRESULT user_f_write (FATFS* fat, FIL* fp, const TCHAR* path, RTC_TimeTypeDef* time, RTC_TimeTypeDef* date);
FRESULT user_f_read (FATFS* fat, FIL* fp, const TCHAR* path);
FRESULT user_f_clear (FATFS* fat, FIL* fp, const TCHAR* path);






#ifdef __cplusplus
}
#endif

#endif

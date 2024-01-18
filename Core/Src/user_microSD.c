#include "user_microSD.h"
#include "usart.h"



// sending to UART
void transmit_uart(char *string) {
  uint8_t len = strlen(string);
  HAL_UART_Transmit(&huart3, (uint8_t*) string, len, 200);
}

FRESULT user_f_delete_line (FATFS* fat, FIL* fp, const TCHAR* path, INT l_nr)
{
	FRESULT res;
	FIL fil_sup; //support file
	const TCHAR* path_sup = "sup.txt"; //support file path
	char buffer[100];
	INT i_line = 1;

	res = f_mount(fat, "", 0);
//	  if (res == FR_OK) {
//		  transmit_uart("Micro SD card is mounted successfully!\n\r");
//	  } else if (res != FR_OK) {
//		  transmit_uart("Micro SD card's mount error!\n\r");
//	  }

	/* open files*/
	res = f_open(fp, path, FA_READ);
	res = f_open(&fil_sup, path_sup, FA_OPEN_APPEND | FA_WRITE | FA_READ);

	/* reading contents of fp file and writing it to support-file without line to delete*/
	while (f_gets(buffer, sizeof(buffer), fp)) {

	  char mRd[100];
	  sprintf(mRd, "%s", buffer);

	  if (i_line != l_nr)
		  f_puts(mRd, &fil_sup);

	  i_line++;
	}

	/* close files*/
	f_close(fp);
	f_close(&fil_sup);

	/* delete fp file*/
	res = f_unlink (path);
	/* change name of support file ("sup.txt") to fp file*/
	res = f_rename (path_sup, path);

	f_mount(NULL, "", 1);
//	  if (res == FR_OK) {
//		  transmit_uart("The Micro SD card is unmounted!\n\r");
//	  } else if (res != FR_OK) {
//	  	transmit_uart("The Micro SD was not unmounted!");
//	  }

	return res;
}

FRESULT user_f_write (FATFS* fat, FIL* fp, const TCHAR* path, RTC_TimeTypeDef* time, RTC_DateTypeDef* date, struct heater_data* heater_)
{
	FRESULT res;
	char buff[100];

	res = f_mount(fat, "", 0);
//	if (res == FR_OK) {
//		transmit_uart("Micro SD card is mounted successfully!\n\r");
//	} else if (res != FR_OK) {
//		transmit_uart("Micro SD card's mount error!\n\r");
//	}

	res = f_open(fp, path, FA_OPEN_APPEND | FA_WRITE);
//	if (res == FR_OK) {
//			transmit_uart("file is oppened succesfully!\n\r");
//		} else if (res != FR_OK) {
//			transmit_uart("file is not oppened succesfully!\n\r");
//		}

//	sprintf(buff, "%s %s %s %s\n", "rok:", "miesiac:", "dzien:", "czas:");
	snprintf(buff, 100,
				"{\"h\":%d,\"m\":%d,\"s\":%d, \"temp.\" %f}\r\n", time->Hours, time->Minutes, time->Seconds, heater_->temperature);

	f_puts(buff, fp);

	res = f_close(fp);

	f_mount(NULL, "", 1);
//		if (res == FR_OK) {
//			transmit_uart("The Micro SD card is unmounted!\n\r");
//		} else if (res != FR_OK) {
//		transmit_uart("The Micro SD was not unmounted!");
//	}

	return res;
}

FRESULT user_f_read (FATFS* fat, FIL* fp, const TCHAR* path)
{
	FRESULT res;
	char buffer[100];

	res = f_mount(fat, "", 0);
//	if (res == FR_OK) {
//		transmit_uart("Micro SD card is mounted successfully!\n\r");
//	} else if (res != FR_OK) {
//		transmit_uart("Micro SD card's mount error!\n\r");
//	}

	/* open file */
	res = f_open(fp, path, FA_READ);

	 while (f_gets(buffer, sizeof(buffer), fp)) {
		 char mRd[100];
		 sprintf(mRd, "%s\n\r", buffer);
	  	 transmit_uart(mRd);
	 }

	/* Close file */
	res = f_close(fp);

//	f_mount(NULL, "", 1);
//		if (res == FR_OK) {
//			transmit_uart("The Micro SD card is unmounted!\n\r");
//		} else if (res != FR_OK) {
//		transmit_uart("The Micro SD was not unmounted!");
//	}

	return res;
}

FRESULT user_f_clear (FATFS* fat, FIL* fp, const TCHAR* path)
{
	FRESULT res;

//	res = f_mount(fat, "", 0);
//	if (res == FR_OK) {
//		transmit_uart("Micro SD card is mounted successfully!\n\r");
//	} else if (res != FR_OK) {
//		transmit_uart("Micro SD card's mount error!\n\r");
//	}

	res = f_open(fp, path, FA_CREATE_ALWAYS);
	res = f_close(fp);

//	f_mount(NULL, "", 1);
//	if (res == FR_OK) {
//		transmit_uart("The Micro SD card is unmounted!\n\r");
//	} else if (res != FR_OK) {
//		transmit_uart("The Micro SD was not unmounted!");
//	}

	return res;
}

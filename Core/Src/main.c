/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "fatfs.h"
#include "lwip.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "grzalka_defs.h"
#include "base.h"
#include "web.h"
#include "memo.h"
#include "serial_communication.h"

/*microSD card*/
#include "fatfs_sd.h"
#include "string.h"
#include "stdio.h"
#include "user_microSD.h"
//#include "lwip/apps/sntp.h"
  /* klawiaturka*/
#include "klawiatur.h"

/*Wyswietlacz*/
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "snow_tiger.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
struct heater_data heat_d;
struct pid_config piajdi;
int Tem,IP1,IP2,IP3,IP4;

/*klawiaturka*/
//bufor przechowujący dwie cyfry zadanej temperatury do zapodania mieszkowi
int bufor_mieszka;
int idx_buf = 0;

char Rx_data[1], Rx_Buffer[50], Transfer_cplt;
uint16_t Rx_indx;
float idk = 69.9;
char message[50];

//RTC_TimeTypeDef czas;
//RTC_DateTypeDef date;
RTC_TimeTypeDef czas1;
RTC_DateTypeDef date1;

/* microSD card*/
FATFS fs;
FIL fil;
uint8_t kartasd;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if (huart == &huart3)
	{
		uint8_t i=0;
		//int j=0;
		if (Rx_indx==0) {for (i=0;i<22;i++) Rx_Buffer[i]=0;} //clear Rx_Buffer before receiving new data

		if (Rx_data[0]!=10)	//if received data different from ascii 13 (enter)
			{
			Rx_Buffer[Rx_indx++]=Rx_data[0];	//add data to Rx_Buffer
			}
		else			//if received data = 13
			{
			Rx_indx=0;
			Transfer_cplt=1;//transfer complete, data is ready to read
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			}

		if (Transfer_cplt == 1)
		{

            //for (j=0;j<3;j++) preVal[j]=&Rx_Buffer[j+1];
            //preVal=&Rx_Buffer[1];
        	uint32_t receive_crc;
            int IP11,IP21,IP31,IP41;
            float P1,I1,D1,Tem1;
            Tem1=P1=I1=D1=IP11=IP21=IP31=IP41=0;

            //char string[20];
            sscanf(Rx_Buffer,"Tem=%f;P=%f;I=%f;D=%f;IP=%d.%d.%d.%d;",&Tem1,&P1,&I1,&D1,&IP11,&IP21,&IP31,&IP41);
            receive_crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)Rx_Buffer, sizeof(Rx_Buffer));
            if (Tem1 == 1224)
            {

             }
             if(Tem1 != 1224)
             {
//               	Tem = Tem1;
            	 update_setpoint(&Tem1);
              }
              if (P1 == 1224)
              {
                 piajdi.P = piajdi.P;
               }
              if(P1 != 1224)
               {
            	  piajdi.P = P1;
               }
              if(I1 == 1224)
              {
            	  piajdi.I = piajdi.I;
              	  }
               if(I1 != 1224)
               {
            	   piajdi.I = I1;
                }
                if(D1 == 1224)
                 {
                	piajdi.D = piajdi.D;
            	 }
                 if(D1 != 1224)
                 {
                	 piajdi.D = D1;
                 }
                 if(IP11 == 1224)
                 {
                    IP1 = IP1;
                  }
                  if(IP11 != 1224)
                  {
                    IP1 = IP11;
                  }
                  if(IP21 == 1224)
                  {
                    IP2 = IP2;
                   }
                   if(IP21 != 1224)
                   {
                     IP2 = IP21;
                    }
                   if(IP31 == 1224)
                    {
                       IP3 = IP3;
                     }
                    if(IP31 != 1224)
                     {
                       IP3 = IP31;
                      }
                     if(IP41 == 1224)
                      {
                       IP4 = IP4;
                       }
                      if(IP41 != 1224)
                       {
                      	IP4 = IP41;
                       }

            //sscanf(Rx_Buffer,"%d",string,&a,&b,&c,&d);

////            if(strcmp(string,"ip")==0)1
////            {
////            	sntpreconfig(&a, &b, &c, &d);
////            }
//
//
//
            update_pid(&piajdi);
            sntpreconfig(&IP1, &IP2, &IP3, &IP4);
            Transfer_cplt = 0;
//

		}


		HAL_UART_Receive_IT(&huart3,(uint8_t*)&Rx_data, 1);	//activate UART receive interrupt every time
	}

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_CRC_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  MX_FATFS_Init();
  MX_LWIP_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  MX_TIM11_Init();
  MX_SPI5_Init();
  MX_TIM13_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim10);
  HAL_UART_Receive_IT(&huart3, (uint8_t*)&Rx_data, 1);
  HAL_TIM_Base_Start_IT(&htim3);  //timer SD card
  HAL_TIM_Base_Start_IT(&htim11); //przemiatanie klawiaturki
  /* Wyswietlacz*/
  ILI9341_Init();//initial driver setup to drive ili9341
  //HAL_TIM_Base_Start_IT(&htim13);

  internetprawdepowie();

  zegarmistrzswiatla();
  uint8_t k=100;

  user_f_clear (&fs, &fil, "log.txt");

  char full[120];
  uint16_t len;
  uint32_t crc8 = 0;
  mqtt_client_t* client;
  MX_LWIP_Process();
    client = mqtt_client_new();
    if(client != NULL)
    {
  	  mqtt_connect_to_broker(client);

    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  MX_LWIP_Process();

	  if (kartasd==1)
	  {
		  HAL_RTC_GetTime(&hrtc, &czas1, RTC_FORMAT_BIN);
		  HAL_RTC_GetDate(&hrtc, &date1, RTC_FORMAT_BIN);
		  get_sensor_data(&heat_d);
		  user_f_write (&fs, &fil, "log.txt", &czas1, &date1, &heat_d);
		  kartasd = 0;


		  len  = snprintf(full,sizeof(full),"Ob=%f;Tem=%f;Zad=%f;P=%f;I=%f;D=%f;IP=%d.%d.%d.%d;crc:%02X\r\n",heat_d.temperature,heat_d.xref,heat_d.u,piajdi.P,piajdi.I,piajdi.D,IP1,IP2,IP3,IP4,(uint8_t)crc8);
		  crc8 = HAL_CRC_Calculate(&hcrc, (uint32_t*)full, sizeof(full));
		  HAL_UART_Transmit_IT(&huart3, (uint8_t*)full, len);

		  if(mqtt_client_is_connected(client))
		  	  {
		  			  uint16_t message_length = 0;
		  			  message_length = snprintf(message,49,"\"Cebularz\":%0.3f",idk);

		  			  err_t err;
		  			  u8_t qos = 2;
		  			  u8_t retain = 0;
		  			  err = mqtt_publish(client, "stm/test", full, len, qos, retain, mqtt_pub_request_cb, 0);
		  			  if(err != ERR_OK)
		  			  {

		  			  }
		  			  err = mqtt_subscribe(client, "stm/set",qos,mqtt_sub_request_cb,0);
		  	  }


	  }
//	  HAL_Delay(10);
//	  k--;
//	  if(k==0)
//	  {
//		  k=100;
//		  //zegarmistrzswiatla();
//		  HAL_RTC_GetTime(&hrtc, &czas1, RTC_FORMAT_BIN);
//		  HAL_RTC_GetDate(&hrtc, &date1, RTC_FORMAT_BIN);
//		  get_sensor_data(&heat_d);
//		  user_f_write (&fs, &fil, "log.txt", &czas1, &date1, &heat_d);
//		  user_f_read(&fs, &fil, "log.txt");
//	  }
	  //cycle_heater(); // uruchomienie tego w taki sposób grozi poparzeniem lol


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
  if (htim->Instance == htim10.Instance)
  {
	  cycle_heater();
  }

//  if (htim->Instance == TIM3)
//  {
//	  HAL_RTC_GetTime(&hrtc, &czas1, RTC_FORMAT_BIN);
//	  HAL_RTC_GetDate(&hrtc, &date1, RTC_FORMAT_BIN);
//	  get_sensor_data(&heat_d);
//	  user_f_write (&fs, &fil, "log.txt", &czas1, &date1, &heat_d);
//	  //user_f_read(&fs, &fil, "log.txt");
//  }
  if (htim->Instance == TIM11)
  {
	  key = read_keypad();
	  if (key != none){bufor_mieszka = wpisywanie_klawiatur(key, &idx_buf);}
	  HAL_UART_Transmit(&huart3, &key, strlen(&key), 100);
  }
  if (htim->Instance == TIM13)
    {
	  int length = snprintf((char*)wyswietlacz,50,
	  			"%f", heat_d.temperature);

  		ILI9341_Fill_Screen(WHITE);
  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
  		ILI9341_Draw_Text("Slow draw by selecting", 10, 10, BLACK, 1, WHITE);
  		ILI9341_Draw_Text(wyswietlacz, 10, 20, BLACK, 1, WHITE);

//  		ILI9341_Fill_Screen(WHITE);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */



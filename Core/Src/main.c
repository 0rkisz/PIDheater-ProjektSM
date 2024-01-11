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
//#include "lwip/apps/sntp.h"


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

char Rx_data[2], Rx_Buffer[20], Transfer_cplt;
uint16_t Rx_indx;
char message[50];
float idk = 69.9;
uint8_t oneopportunity = 1;
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

		if (Rx_data[0]!=13)	//if received data different from ascii 13 (enter)
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
			int a,b,c,d;
			a=b=c=d=0;
			char string[2];
			sscanf(Rx_Buffer,"%2s=%d.%d.%d.%d",string,&a,&b,&c,&d);
			if(strcmp(string,"ip")==0)
			{
				sntpreconfig(&a, &b, &c, &d);
			}




			Transfer_cplt = 0;

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
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
  //HAL_TIM_Base_Start_IT(&htim10);
  HAL_UART_Receive_IT(&huart3, (uint8_t*)&Rx_data, 1);

  internetprawdepowie();

  zegarmistrzswiatla();
  uint8_t k=10;
  while(k>2)
  {
	  k--;
	  MX_LWIP_Process();
	  HAL_Delay(1000);
  }
  k=100;
  mqtt_client_t* client;
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
	  HAL_Delay(10);
	  k--;
	  if(k==0)
	  {
		  k=100;
		  zegarmistrzswiatla();

		  if(mqtt_client_is_connected(client))
		  {
			  uint16_t message_length = 0;
			  message_length = snprintf(message,49,"\"Cebularz\":%0.3f",idk);

			  err_t err;
			  u8_t qos = 2;
			  u8_t retain = 0;
			  err = mqtt_publish(client, "stm/test", message, message_length, qos, retain, mqtt_pub_request_cb, 0);
			  if(err != ERR_OK)
			  {

			  }		  }
	  }
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

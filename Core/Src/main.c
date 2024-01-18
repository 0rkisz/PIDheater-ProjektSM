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
#include "spi.h"
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
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int Tem,P,I,D,IP1,IP2,IP3,IP4;
int Rx_indx, Transfer_cplt;
char Rx_Buffer [50], Rx_data [1];
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if (huart == &huart3)
    {
        uint8_t i=0;
        //int j=0;
        if (Rx_indx==0) {for (i=0;i<49;i++) Rx_Buffer[i]=0;} //clear Rx_Buffer before receiving new data

        if (Rx_data[0]!=10)    //if received data different from ascii 13 (enter)
            {
            Rx_Buffer[Rx_indx++]=Rx_data[0];    //add data to Rx_Buffer
            }
        else            //if received data = 13
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
            int Tem1,P1,I1,D1,IP11,IP21,IP31,IP41;
            Tem1=P1=I1=D1=IP11=IP21=IP31=IP41=1;

            //char string[20];
            sscanf(Rx_Buffer,"Tem=%d;P=%d;I=%d;D=%d;IP=%d.%d.%d.%d;",&Tem1,&P1,&I1,&D1,&IP11,&IP21,&IP31,&IP41);
            receive_crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)Rx_Buffer, sizeof(Rx_Buffer));
            if (Tem1 == 1224)
            	{
            	Tem = Tem;
            	}
            else if(P1 == 1224)
            {
            	P = P;
            }
            else if(I1 == 1224)
            {
               	I = I;
            }
            else if(D1 == 1224)
            {
            	D = D;
            }
            else if(IP11 == 1224)
            {
            	IP1 = IP1;
            	IP2 = IP2;
            	IP3 = IP3;
            	IP4 = IP4;
	        }
            else
            {
            	Tem = Tem1;
            	P = P1;
            	I = I1;
            	D = D1;
            	IP1 = IP11;
            	IP2 = IP21;
            	IP3 = IP31;
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
//
            Transfer_cplt = 0;
//
        }
//
//
        HAL_UART_Receive_IT(&huart3,(uint8_t*)&Rx_data, 1);    //activate UART receive interrupt every time
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
  /* USER CODE BEGIN 2 */
  char full[100];
  uint16_t len;
  uint32_t crc8 = 0;
  HAL_UART_Receive_IT(&huart3,(uint8_t*)&Rx_data, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  len  = snprintf(full,sizeof(full),"Tem=%d;P=%d;I=%d;D=%d;IP=%d.%d.%d.%d;crc:%02X\r\n",Tem,P,I,D,IP1,IP2,IP3,IP4,(uint8_t)crc8);
	  crc8 = HAL_CRC_Calculate(&hcrc, (uint32_t*)full, sizeof(full));
	  HAL_UART_Transmit(&huart3, (uint8_t*)full, len, 1000);
  	  HAL_Delay(5000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
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

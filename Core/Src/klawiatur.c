#include <klawiatur.h>


uint32_t previousMillis = 0;
uint32_t currentMillis = 0;
uint32_t debounce_time = 150;

GPIO_PinState newState_R1C1 = GPIO_PIN_SET;
GPIO_PinState oldState_R1C1;
GPIO_PinState newState_R1C2 = GPIO_PIN_SET;
GPIO_PinState oldState_R1C2;
GPIO_PinState newState_R1C3 = GPIO_PIN_SET;
GPIO_PinState oldState_R1C3;
GPIO_PinState newState_R1C4 = GPIO_PIN_SET;
GPIO_PinState oldState_R1C4;

GPIO_PinState newState_R2C1 = GPIO_PIN_SET;
GPIO_PinState oldState_R2C1;
GPIO_PinState newState_R2C2 = GPIO_PIN_SET;
GPIO_PinState oldState_R2C2;
GPIO_PinState newState_R2C3 = GPIO_PIN_SET;
GPIO_PinState oldState_R2C3;
GPIO_PinState newState_R2C4 = GPIO_PIN_SET;
GPIO_PinState oldState_R2C4;

GPIO_PinState newState_R3C1 = GPIO_PIN_SET;
GPIO_PinState oldState_R3C1;
GPIO_PinState newState_R3C2 = GPIO_PIN_SET;
GPIO_PinState oldState_R3C2;
GPIO_PinState newState_R3C3 = GPIO_PIN_SET;
GPIO_PinState oldState_R3C3;
GPIO_PinState newState_R3C4 = GPIO_PIN_SET;
GPIO_PinState oldState_R3C4;

GPIO_PinState newState_R4C1 = GPIO_PIN_SET;
GPIO_PinState oldState_R4C1;
GPIO_PinState newState_R4C2 = GPIO_PIN_SET;
GPIO_PinState oldState_R4C2;
GPIO_PinState newState_R4C3 = GPIO_PIN_SET;
GPIO_PinState oldState_R4C3;
GPIO_PinState newState_R4C4 = GPIO_PIN_SET;
GPIO_PinState oldState_R4C4;

char buf_klawiatur[3] = {0, 0, 0};
char buf_klawiatur_pom[2] = {0, 0};
int bufor_wysylkowy;

uint8_t none = 'n';
uint8_t key = 'n';

int wpisywanie_klawiatur(char key, int* idx_buf)
{
	  switch (*idx_buf) {
	    case 0:
	    	buf_klawiatur[*idx_buf] = key;
	    	(*idx_buf)++;
	      break;
	    case 1:
	    	buf_klawiatur[*idx_buf] = key;
	    	if (buf_klawiatur[*idx_buf] != 'C') (*idx_buf)++;
	    	if (buf_klawiatur[*idx_buf] == 'C')
	    	{
//	    		char a = buf_klawiatur[0];
//	    		sscanf(buf_klawiatur[0], "%d", bufor_wysylkowy);
	    		char a = buf_klawiatur[0];
	    		bufor_wysylkowy = atoi(&a);
	    		buf_klawiatur[0] = 0;
	    		buf_klawiatur[1] = 0;
	    		*idx_buf = 0;
	    		return bufor_wysylkowy;
	    	}
	      break;
	    case 2:
	    	buf_klawiatur[*idx_buf] = key;

	    	buf_klawiatur_pom[0] = buf_klawiatur[0];
	    	buf_klawiatur_pom[1] = buf_klawiatur[1];
	    	bufor_wysylkowy = atoi(buf_klawiatur_pom);
		    if (buf_klawiatur[*idx_buf] == 'C')
		    {
		    	return bufor_wysylkowy;
		    }
		    buf_klawiatur[0] = 0;
		    buf_klawiatur[1] = 0;
		    *idx_buf = 0;
		break;

		    //uint16_t a = strlen(&key);
//		    HAL_UART_Transmit(&huart3, &key, strlen(&key), 100);
	  }

	  return 0;
}

//dobrze reaguje na zbocze narastające i debouncing, jednak gdy przycisk zostaje puszczony on w tym czasie jest
//w innym miejscu tej funkcji i wykrywa inne naciśnięcia
char read_keypad (void)
{
	currentMillis = HAL_GetTick();
	/* Make ROW 1 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_GPIO_Port, R1_Pin, GPIO_PIN_RESET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);  // Pull the R4 High

	//debouncing + zbocze
	oldState_R1C1 = newState_R1C1;
	newState_R1C1 = HAL_GPIO_ReadPin (C1_GPIO_Port, C1_Pin);
	oldState_R1C2 = newState_R1C2;
	newState_R1C2 = HAL_GPIO_ReadPin (C2_GPIO_Port, C2_Pin);
	oldState_R1C3 = newState_R1C3;
	newState_R1C3 = HAL_GPIO_ReadPin (C3_GPIO_Port, C3_Pin);
	oldState_R1C4 = newState_R1C4;
	newState_R1C4 = HAL_GPIO_ReadPin (C4_GPIO_Port, C4_Pin);

	int key_pressed = 0;

	if ((newState_R1C1 == GPIO_PIN_RESET) && oldState_R1C1 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))
	{
		key_pressed = 1;
		return '1';
	}

	else if ((newState_R1C2 == GPIO_PIN_RESET) && oldState_R1C2 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 2 is low
	{
		key_pressed = 1;
		return '2';
	}

	else if ((newState_R1C3 == GPIO_PIN_RESET) && oldState_R1C3 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 3 is low
	{
		key_pressed = 1;
		return '3';
	}

	else if ((newState_R1C3 == GPIO_PIN_RESET) && oldState_R1C3 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 4 is low
	{
		key_pressed = 1;
		return 'A';
	}

	/* Make ROW 2 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_GPIO_Port, R2_Pin, GPIO_PIN_RESET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);  // Pull the R4 High

	//debouncing + zbocze
	oldState_R2C1 = newState_R2C1;
	newState_R2C1 = HAL_GPIO_ReadPin (C1_GPIO_Port, C1_Pin);
	oldState_R2C2 = newState_R2C2;
	newState_R2C2 = HAL_GPIO_ReadPin (C2_GPIO_Port, C2_Pin);
	oldState_R2C3 = newState_R2C3;
	newState_R2C3 = HAL_GPIO_ReadPin (C3_GPIO_Port, C3_Pin);
	oldState_R2C4 = newState_R2C4;
	newState_R2C4 = HAL_GPIO_ReadPin (C4_GPIO_Port, C4_Pin);

		if ((newState_R2C1 == GPIO_PIN_RESET) && oldState_R2C1 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))
		{
			key_pressed = 1;
			return '4';
		}

	else if ((newState_R2C2 == GPIO_PIN_RESET) && oldState_R2C2 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 2 is low
	{
		key_pressed = 1;
		return '5';
	}

	else if ((newState_R2C3 == GPIO_PIN_RESET) && oldState_R2C3 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 3 is low
	{
		key_pressed = 1;
		return '6';
	}

	else if ((newState_R2C4 == GPIO_PIN_RESET) && oldState_R2C4 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 4 is low
	{
		key_pressed = 1;
		return 'B';
	}


	/* Make ROW 3 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_GPIO_Port, R3_Pin, GPIO_PIN_RESET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);  // Pull the R4 High

	//debouncing + zbocze
	oldState_R3C1 = newState_R3C1;
	newState_R3C1 = HAL_GPIO_ReadPin (C1_GPIO_Port, C1_Pin);
	oldState_R3C2 = newState_R3C2;
	newState_R3C2 = HAL_GPIO_ReadPin (C2_GPIO_Port, C2_Pin);
	oldState_R3C3 = newState_R3C3;
	newState_R3C3 = HAL_GPIO_ReadPin (C3_GPIO_Port, C3_Pin);
	oldState_R3C4 = newState_R3C4;
	newState_R3C4 = HAL_GPIO_ReadPin (C4_GPIO_Port, C4_Pin);

	if ((newState_R3C1 == GPIO_PIN_RESET) && oldState_R3C1 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 1 is low
	{
		key_pressed = 1;
		return '7';
	}

	else if ((newState_R3C2 == GPIO_PIN_RESET) && oldState_R3C2 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 2 is low
	{
		key_pressed = 1;
		return '8';
	}

	else if ((newState_R3C3 == GPIO_PIN_RESET) && oldState_R3C3 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 3 is low
	{
		key_pressed = 1;
		return '9';
	}

	else if ((newState_R3C4 == GPIO_PIN_RESET) && oldState_R3C4 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 4 is low
	{
		key_pressed = 1;
		return 'C';
	}


	/* Make ROW 4 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_GPIO_Port, R4_Pin, GPIO_PIN_RESET);  // Pull the R4 High

	//debouncing + zbocze
	oldState_R4C1 = newState_R4C1;
	newState_R4C1 = HAL_GPIO_ReadPin (C1_GPIO_Port, C1_Pin);
	oldState_R4C2 = newState_R4C2;
	newState_R4C2 = HAL_GPIO_ReadPin (C2_GPIO_Port, C2_Pin);
	oldState_R4C3 = newState_R4C3;
	newState_R4C3 = HAL_GPIO_ReadPin (C3_GPIO_Port, C3_Pin);
	oldState_R4C4 = newState_R4C4;
	newState_R4C4 = HAL_GPIO_ReadPin (C4_GPIO_Port, C4_Pin);

	if ((newState_R4C1 == GPIO_PIN_RESET) && oldState_R4C1 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 1 is low
	{
		key_pressed = 1;
		return '*';
	}

	else if ((newState_R4C2 == GPIO_PIN_RESET) && oldState_R4C2 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 2 is low
	{
		key_pressed = 1;
		return '0';
	}

	else if ((newState_R4C3 == GPIO_PIN_RESET) && oldState_R4C3 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 3 is low
	{
		key_pressed = 1;
		return '#';
	}

	else if ((newState_R4C4 == GPIO_PIN_RESET) && oldState_R4C4 == GPIO_PIN_SET && ((currentMillis - previousMillis) > debounce_time))   // if the Col 4 is low
	{
		key_pressed = 1;
		return 'D';
	}
	if (key_pressed == 1){
		previousMillis = currentMillis;
		key_pressed = 0;
	}
	else return none;
}

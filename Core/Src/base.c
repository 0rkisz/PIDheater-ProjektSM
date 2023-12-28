/*
 * base.c
 *
 *  Created on: Dec 27, 2023
 *      Author: Admin
 */
#include "base.h"
struct heater_data meas;
struct pid_config curr_pid;
void get_sensor_data(struct heater_data *h_data)
{
	/*
	 * funkcja zwracająca pomiary wskaźnikiem, okno na świat, uarty i ethernety będą się tutaj pasły
	 */
}

void cycle_heater()
{
	measure();
	calculatePID();
}

void calculatePID()
{
	/*
	 *  instert laby z PID hir
	 */
}

void measure()
{
	meas.temperature = HAL_ADC_PollForConversion(&hadc1, 1000); //makieta, of kors że będą tu dane z czapy
}
void update_pid(struct pid_config *newPID)
{
	/*
	 * 	wywołanie aktualizacji nastaw PID np przez ethernet lub UART
	 */
	curr_pid.P = newPID->P;
	curr_pid.I = newPID->I;
	curr_pid.D = newPID->D;
}

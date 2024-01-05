/*
 * base.c
 *
 *  Created on: Dec 27, 2023
 *      Author: Orkisz
 */

#include "base.h"


//PID variables

struct heater_data meas = {
		.pressure = 0,
		.temperature = 0,
		.xref = 55,
		.u = 0
};
struct pid_config curr_pid = {
		.P = 5,
		.I = 1.6,
		.D = 0.01
};
float P = 0; float I = 0; float D = 0; float temptemp=0; float filtertemp = 0;


//ADC variables

uint16_t raw_temp=0;
float ntc_voltage=0;
float R_ntc=0;

//filter parameters

//float32_t iir_coeffs[]={1.0000, 2.0000, 1.0000, -1.1615, 0.4211};
//float32_t iir_state[8] = {0};
//float32_t iir_gain = 0.0065; //0.4832
//arm_biquad_casd_df1_inst_f32 iir_filter = {.numStages = 1, .pState=iir_state, .pCoeffs=iir_coeffs};

//float32_t iir_coeffs[] = {1.0, 0.9927, 0, 0.7227, 0,
//		1.0, 2.0135, 1.0135, 1.4804, -0.5591,
//		1.0, 2.0031, 1.0031, 1.5882, -0.6727,
//		1.0, 1.9907, 0.9908, 1.7750, -0.8694};
//
//float32_t iir_state[16] = {0};
//float32_t iir_gain = 1.3622e-06;
//arm_biquad_casd_df1_inst_f32 iir_filter = {.numStages = 4, .pState = iir_state, .pCoeffs = iir_coeffs};

float32_t iir_coeffs[]={1.000000, 2.000000, 1.000000, 1.552622, -0.635816};

float32_t iir_state[16] = {0};
float32_t iir_gain = 0.0208;
arm_biquad_casd_df1_inst_f32 iir_filter = {.numStages = 1, .pState=iir_state, .pCoeffs=iir_coeffs};

void get_sensor_data(struct heater_data *h_data)
{
	h_data->pressure = meas.pressure;
	h_data->temperature = meas.temperature;
	h_data->u = meas.u;
	h_data->xref = meas.xref;
}

void update_setpoint(float *setpoint)
{
	meas.xref = *setpoint;
}

void cycle_heater()
{
	HAL_GPIO_WritePin(debug1_GPIO_Port, debug1_Pin, 1);
	measure();
	calculatePID(meas.xref,meas.temperature);
	//spread_misinformation();
	HAL_GPIO_WritePin(debug1_GPIO_Port, debug1_Pin, 0);
}

void calculatePID(float setpoint, float measured)
{

	if (setpoint > 90)
	{
		setpoint = 90;
	} else if (setpoint<0){setpoint=0;}

	static float previous_error = 0;
	static float previous_integral = 0;
	static float error = 0;
	static float integral = 0;
	static float derivative = 0;

	error = setpoint-measured;

	//proportional part
	P = curr_pid.P * error;

	//integral part
	integral = previous_integral + (error+previous_error); //numerical integrator without anti-windup

	if(integral > 10000)
	{
		integral = 10000;
	} else if ( integral < -10000){integral = -10000;}

	previous_integral = integral;
	I = curr_pid.I*integral*(dt/2.0);

	//derivative part
	derivative = (error - previous_error)/dt; //numerical derivative without filter
	previous_error = error;
	D = curr_pid.D*derivative;

	//sum of all parts
	meas.u = P  + I + D; //without saturation

	if (meas.u > 99)
	{
		meas.u = 99;
	} else if (meas.u<0){meas.u=0;}

	TIM9->CCR1=meas.u*10;
}

void measure()
{

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	raw_temp=HAL_ADC_GetValue(&hadc1);
	ntc_voltage = 3.3*raw_temp/4095;


	R_ntc = (R4*ntc_voltage)/(3.3-ntc_voltage);
	temptemp = 1/(log(R_ntc/ntcR0)*(1/ntcB)+(1/298.15));
	temptemp = temptemp - 273.15;

	HAL_GPIO_WritePin(debug2_GPIO_Port, debug2_Pin, 1);
	arm_biquad_cascade_df1_f32(&iir_filter, &temptemp, &filtertemp, 1); //process 1 sample
	HAL_GPIO_WritePin(debug2_GPIO_Port, debug2_Pin, 0);
    meas.temperature = filtertemp * iir_gain;

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

//void spread_misinformation()
//{
//	uint8_t len=0;
//	char msg[60];
//	memset(msg, 0, sizeof(msg));
//	len = sprintf(msg,"{\"Sygnal sterujacy\":%0.3f, \"Temp\":%0.3f}\r\n",meas.u,meas.temperature);
//	HAL_UART_Transmit(&huart3, (uint8_t*)msg, len, HAL_MAX_DELAY);
//}



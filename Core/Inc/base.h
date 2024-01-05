/*
 * base.h
 *
 *  Created on: Dec 27, 2023
 *      Author: Admin
 */

#ifndef INC_BASE_H_
#define INC_BASE_H_

#include "grzalka_defs.h"
#include "main.h"
#include "math.h"
#include "arm_math.h"
#include "adc.h"

#define R4 20000.0f
#define ntcR0 10000.0f
#define ntcB 3435.0f


void get_sensor_data(struct heater_data *h_data);
void update_setpoint(float *setpoint);
void cycle_heater();
void calculatePID(float setpoint, float measured);
void measure();
void update_pid(struct pid_config *newPID);



#endif /* INC_BASE_H_ */

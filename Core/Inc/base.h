/*
 * base.h
 *
 *  Created on: Dec 27, 2023
 *      Author: Admin
 */

#ifndef INC_BASE_H_
#define INC_BASE_H_

#include "grzalka_defs.h"
void get_sensor_data(struct heater_data *h_data);
void cycle_heater();
void calculatePID();
void measure();
void update_pid(struct pid_config *newPID);
#endif /* INC_BASE_H_ */

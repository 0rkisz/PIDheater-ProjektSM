/*
 * grzalka_defs.h
 *
 *  Created on: Dec 27, 2023
 *      Author: Admin
 */

#ifndef INC_GRZALKA_DEFS_H_
#define INC_GRZALKA_DEFS_H_
#include "main.h"

struct heater_data
{
    /*! Compensated pressure */
    float pressure;

    /*! Compensated temperature */
    float temperature;

    float u;

    float xref;
};
struct pid_config
{
	float P;
	float I;
	float D;
};

#define dt 0.005f
#endif /* INC_GRZALKA_DEFS_H_ */

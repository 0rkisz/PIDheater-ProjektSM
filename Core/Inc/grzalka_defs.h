/*
 * grzalka_defs.h
 *
 *  Created on: Dec 27, 2023
 *      Author: Admin
 */

#ifndef INC_GRZALKA_DEFS_H_
#define INC_GRZALKA_DEFS_H_
#include "main.h"
#include "adc.h"
struct heater_data
{
    /*! Compensated pressure */
    uint32_t pressure;

    /*! Compensated temperature */
    int32_t temperature;
};
struct pid_config
{
	uint8_t P;
	uint8_t I;
	uint8_t D;
};
#endif /* INC_GRZALKA_DEFS_H_ */

/*
 * web.h
 *
 *  Created on: Dec 27, 2023
 *      Author: Admin
 */

#ifndef INC_WEB_H_
#define INC_WEB_H_

#include "main.h"
#include "rtc.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "lwip/apps/sntp.h"
#include "lwip/raw.h"
#include "lwip/udp.h"


void zegarmistrzswiatla();
void internetprawdepowie();
void sntpreconfig(int *a,int *b,int *c,int *d);

#endif /* INC_WEB_H_ */

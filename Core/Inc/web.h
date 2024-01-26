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
#include "lwip/apps/mqtt.h"
//#include "lwip/api.h"


void zegarmistrzswiatla();
void internetprawdepowie();
void sntpreconfig(int *a,int *b,int *c,int *d);

void mqtt_connect_to_broker(mqtt_client_t *client);
void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);
void mqtt_sub_request_cb(void *arg, err_t result);
void mqtt_pub_request_cb(void *arg, err_t result);


#endif /* INC_WEB_H_ */

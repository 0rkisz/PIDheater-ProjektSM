/*
 * web.c
 *
 *  Created on: Dec 27, 2023
 *      Author: Orkisz
 */

#include "web.h"

RTC_TimeTypeDef czas;
RTC_DateTypeDef date;
uint8_t serial_buffer[101];

void zegarmistrzswiatla()
{

	memset(serial_buffer,0,sizeof(serial_buffer));
	int length;
	HAL_RTC_GetTime(&hrtc, &czas, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	length = snprintf((char*)serial_buffer, 100,
			"{\"h\":%d,\"m\":%d,\"s\":%d}\r\n", czas.Hours, czas.Minutes, czas.Seconds);
	HAL_UART_Transmit(&huart3, serial_buffer, length, 1000);



}
void internetprawdepowie()
{

	ip_addr_t sntp_server_ip;
	IP4_ADDR(&sntp_server_ip, 192, 168, 113, 4);

	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setserver(0, &sntp_server_ip);
	sntp_init();


	putenv("TZ=GMT-01:00");

//	struct udp_pcb *pUDP_pcb;
//	pUDP_pcb = udp_new();
//	udp_bind(pUDP_pcb, IP4_ADDR_ANY, 1000);
//#define MAX_SIZE 100
//	char message[MAX_SIZE]; uint16_t length;
//	//uint16_t count=0;
//	struct pbuf *p;
//	length = snprintf(message, MAX_SIZE, "DUPA 123 POWTARZAM DUPA 123");
//	p = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_RAM);
//	memcpy(p->payload, message, length);
//	udp_sendto(pUDP_pcb, p, &sntp_server_ip, 5000);
//	pbuf_free(p);
}
void sntpreconfig(int *a,int *b,int *c,int *d)
{
	sntp_stop();
	ip_addr_t sntp_server_ip;
	IP4_ADDR(&sntp_server_ip, *a, *b, *c, *d);

	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setserver(0, &sntp_server_ip);
	sntp_init();

}

void mqtt_connect_to_broker(mqtt_client_t *client)
{
	ip_addr_t mqtt_broker_addr;
	struct mqtt_connect_client_info_t ci;
	err_t err;
	ipaddr_aton("192.168.1.13", &mqtt_broker_addr);

	memset(&ci, 0, sizeof(ci));

	ci.client_id = "my_mqtt_id";
	ci.keep_alive = 50000;

	do{
		err = mqtt_client_connect(client, &mqtt_broker_addr, MQTT_PORT, mqtt_connection_cb, 0, &ci);
		HAL_Delay(100);
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	} while (err != ERR_OK || err != ERR_ISCONN );
}
void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
	err_t err;
	if(status == MQTT_CONNECT_ACCEPTED)
	{
		mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

		err = mqtt_subscribe(client, "stm/test", 1, mqtt_sub_request_cb,arg);

		if(err != ERR_OK)
		{

		} else {
			mqtt_connect_to_broker(client);
		}
	}
}
void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{

}
void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{

}
void mqtt_sub_request_cb(void *arg, err_t result)
{

}
void mqtt_pub_request_cb(void *arg, err_t result)
{

}


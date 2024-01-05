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

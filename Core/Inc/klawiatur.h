/*
 * klawiatur.h
 *
 *  Created on: Jan 26, 2024
 *      Author: julow
 */

#ifndef INC_KLAWIATUR_H_
#define INC_KLAWIATUR_H_

#include "main.h"
#include "usart.h"

//Debouncing
extern uint32_t previousMillis;
extern uint32_t currentMillis;
extern uint32_t debounce_time;

//Edge_detection()
extern GPIO_PinState newState_R1C1;
extern GPIO_PinState oldState_R1C1;
extern GPIO_PinState newState_R1C2;
extern GPIO_PinState oldState_R1C2;
extern GPIO_PinState newState_R1C3;
extern GPIO_PinState oldState_R1C3;
extern GPIO_PinState newState_R1C4;
extern GPIO_PinState oldState_R1C4;

extern GPIO_PinState newState_R2C1;
extern GPIO_PinState oldState_R2C1;
extern GPIO_PinState newState_R2C2;
extern GPIO_PinState oldState_R2C2;
extern GPIO_PinState newState_R2C3;
extern GPIO_PinState oldState_R2C3;
extern GPIO_PinState newState_R2C4;
extern GPIO_PinState oldState_R2C4;

extern GPIO_PinState newState_R3C1;
extern GPIO_PinState oldState_R3C1;
extern GPIO_PinState newState_R3C2;
extern GPIO_PinState oldState_R3C2;
extern GPIO_PinState newState_R3C3;
extern GPIO_PinState oldState_R3C3;
extern GPIO_PinState newState_R3C4;
extern GPIO_PinState oldState_R3C4;

extern GPIO_PinState newState_R4C1;
extern GPIO_PinState oldState_R4C1;
extern GPIO_PinState newState_R4C2;
extern GPIO_PinState oldState_R4C2;
extern GPIO_PinState newState_R4C3;
extern GPIO_PinState oldState_R4C3;
extern GPIO_PinState newState_R4C4;
extern GPIO_PinState oldState_R4C4;

extern char buf_klawiatur[3];
extern char buf_klawiatur_pom[2];
extern int bufor_wysylkowy;

extern uint8_t none;
extern uint8_t key;

int wpisywanie_klawiatur(char key, int* idx_buf);
char read_keypad (void);

#endif /* INC_KLAWIATUR_H_ */

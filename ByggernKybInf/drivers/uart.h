/*
 * uart.h
 *
 * Created: 06.09.2022 13:00:54
 *  Author: thomahl
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdio.h>

void UART_Transmit(unsigned char data);

unsigned char UART_Receive(void);

void UART_init(unsigned int ubrr);

static FILE UART_STREAM = FDEV_SETUP_STREAM(UART_Transmit, UART_Receive, _FDEV_SETUP_RW);

#endif /* UART_H_ */
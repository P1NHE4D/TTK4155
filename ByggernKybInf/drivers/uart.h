/*
 * uart.h
 *
 * Created: 06.09.2022 13:00:54
 *  Author: thomahl
 */ 


#ifndef UART_H_
#define UART_H_


void UART_init(unsigned int ubrr);

void UART_Transmit(unsigned char data);

unsigned char UART_Receive(void);



#endif /* UART_H_ */
/*
 * main.c
 *
 * Created: 06.09.2022 12:55:30
 *  Author: thomahl
 */ 

#include "drivers/uart.h"


#define FOSC 4915200
#define BAUD 9600
#define COMPUTED_UBRR FOSC/16/BAUD-1

#include <stdio.h>

int main(void) {
	UART_init(COMPUTED_UBRR);
	//fdevopen(UART_Transmit, UART_Receive);
	while (1) {
		UART_Transmit('0');
		/*
		UART_Transmit('B');
		UART_Transmit('\n');
		UART_Transmit('\0');
		*/
		
	}
}

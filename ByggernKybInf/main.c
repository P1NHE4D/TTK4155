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
	while (1) {
		int foo = 70;
		printf("foo %d\n\r", foo);
	}
}

/*
 * main.c
 *
 * Created: 06.09.2022 12:55:30
 *  Author: thomahl
 */ 

#include "avr/io.h"
#include "drivers/uart.h"
#include "drivers/xmem.h"


#define FOSC 4915200
#define BAUD 9600
#define COMPUTED_UBRR FOSC/16/BAUD-1

#include <stdio.h>
#include <stdlib.h>


int main(void) {
	UART_init(COMPUTED_UBRR);
	xmem_init();
		
	sram_test();   
}
/*
 * main.c
 *
 * Created: 06.09.2022 12:55:30
 *  Author: thomahl
 */ 





#include "defines.h"
#include "avr/io.h"
#include "drivers/uart.h"
#include "drivers/xmem.h"
#include "drivers/adc.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t adc_read(uint8_t channel);

int main(void) {
	UART_init(COMPUTED_UBRR);
	xmem_init();
	adc_init();

	
	/*
	while (1) {
		printf("Channel 0,1,2,3 reads %4d,%4d,%4d,%4d\n\r", adc_read(0), adc_read(1), adc_read(2), adc_read(3));
	}
	*/
	
	/*
	Exercise 3 task 5:
		Cutoff frequency: 795.8hz (using https://www.omnicalculator.com/physics/low-pass-filter, using R = 2000 Omh, C = 100 nF)
		Slope at cutoff frequency: ?
	*/
}
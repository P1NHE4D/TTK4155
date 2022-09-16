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

#define F_CPU FOSC

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>



int main(void) {
	UART_init(COMPUTED_UBRR);
	xmem_init();
	
	/*
	printf("foo\n\r");
	_delay_ms(10000);
	printf("bar\n\r");
	*/
	sram_test();


	
	DDRD |= (1 << DDD5);
	
	// table 56
	// yes to CTC mode
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS10);

	// table 53
	// toggle signal on each match
	TCCR1A |= (1 << COM1A0);

	// set OCR1A (it's divided into two registers)
	// ...let frequency equal CPU frequency?
	OCR1A = 0;
}

/*
const uint16_t ADC_BASE_ADDRESS = 0x1400;
const int ADC_NUM_CHANNELS = 4;

// channel must be one of 0,1,2,3
uint8_t adc_read(uint8_t channel) {
	if channel >= ADC_NUM_CHANNELS {
		printf("adc_read was passed argument channel out of bounds !(argument %d >= number of channels %d)", channel, ADC_NUM_CHANNELS)
	}
	
	volatile char *ext_mem = (char*) ADC_BASE_ADDRESS;
	
	// write to the ADC to initiate a conversion
	*ext_mem = 1;
	
	// wait t_conv time for the conversion to finish
	// TODO
	
	char channels[ADC_NUM_CHANNELS];
	
	for (int i = 0; i < ADC_NUM_CHANNELS; i++) {
		// read next channel from ADC (first channel read is channel 0)
		channels[i] = *ext_mem;
	}
	
	return channels[channel];	
}
*/
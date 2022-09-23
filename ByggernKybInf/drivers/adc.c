/*
 * adc.c
 *
 * Created: 16.09.2022 10:40:38
 *  Author: thomahl
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include "adc.h"
#include "../defines.h"

// needed for include of <util/delay.h>
#define F_CPU FOSC
#include <util/delay.h>

void adc_init(void) {
	// set PD5 as an output pin in the data direction register for port D
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

// channel must be one of 0,1,2,3
uint8_t adc_read(channel_t channel) {
	
	volatile char *ext_mem = (char*) ADC_BASE_ADDRESS;
	
	// write to the ADC to initiate a conversion
	ext_mem[0] = 0;
	
	// wait t_conv time for the conversion to finish
	_delay_ms(ADC_T_CONV);
	
	char channels[ADC_NUM_CHANNELS];
	
	for (int i = 0; i < ADC_NUM_CHANNELS; i++) {
		// read next channel from ADC (first channel read is channel 0)
		channels[i] = ext_mem[0];
	}
	
	return channels[channel];
}

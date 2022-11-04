
/*
 * adc.c
 *
 * Created: 04.11.2022 11:21:34
 *  Author: thomahl
 */ 

#include "adc.h"
#include <sam.h>
#include <stdbool.h>

void adc_init() {
	// enable the clock of the "ADC Controller" peripheral,
	// which has PID 37 (table 9-1)
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;
	
	// enable ADC channel 0
	ADC->ADC_CHER |= ADC_CHER_CH0;
	
	adc_read();

}

uint16_t adc_read() {
	ADC->ADC_CR |= ADC_CR_START;
	
	while (!(ADC->ADC_ISR & ADC_ISR_EOC0)) {}
	
	return ADC->ADC_CDR[0];
}
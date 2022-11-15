/*
 * dac.c
 *
 * Created: 11.11.2022 09:53:53
 *  Author: thomahl
 */ 

#include <sam.h>
#include "dac.h"

void dac_init() {
	// enable the clock of the "DAC Controller" peripheral,
	// which has PID 38 (table 9-1)
	PMC->PMC_PCER1 |= PMC_PCER1_PID38;

	// enable channel
	DACC->DACC_CHER |= DACC_CHER_CH1;
	
	if (!(DACC->DACC_CHSR & DACC_CHSR_CH1)) {
		printf("failed to enable ch1");
	}
	
	// always convert using channel 1
	// also note:
	//  WORD: default is half-world transfer mode. does this matter to us? (only the 16 LSB are used for conversion)
	//  TRGEN: default is free running mode, which is fine for now (conversion happens as soon as channel enabled and something is put onto DACC_CDR)
	DACC->DACC_MR |= DACC_MR_USER_SEL_CHANNEL1;	
}

void dac_convert(uint16_t value) {
	// block until DAC Controller is ready to accept conversions
	while (!(DACC->DACC_ISR & DACC_ISR_TXRDY)) {}

	// this starts a conversion since we are in free running mode	
	DACC->DACC_CDR = value;
	
	// block until conversion is complete
	while (!(DACC->DACC_ISR & DACC_ISR_EOC)) {}
}
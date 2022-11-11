/*
 * timer.c
 *
 * Created: 04.11.2022 14:12:47
 *  Author: thomahl
 */ 

#include "timer.h"
#include <sam.h>
#define FREQ 84000000

void timer_init() {
	// enable clock for TC0
	PMC->PMC_PCER0 |= PMC_PCER0_PID27;
	// enable timer
	REG_TC0_CCR0 |= (TC_CCR_CLKEN | TC_CCR_SWTRG);
	// set clock mode to MCK/2
	REG_TC0_CMR0 |= TC_CMR_TCCLKS_TIMER_CLOCK4;
	// check if clock is enabled
	if (!(REG_TC0_SR0 & TC_SR_CLKSTA)) {
		printf("Clock disabled!\n\r");
	}
}

uint32_t clock() {
	// read value
	uint32_t clock_read = REG_TC0_CV0;
	// return time in seconds
	return (clock_read) / (FREQ / 128);
}
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
	// ...
	//struct TcChannel->TC_CCR |= TC_CCR_CLKEN;
	//struct TcChannel->TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK1;
}

uint32_t clock() {
	//uint32_t clock_read = TcChannel->TC_CV[0];
	//return clock_read / (FREQ / 2);
	
	// read register
	// return (read value * 1000) / (FREQ / 2)
}
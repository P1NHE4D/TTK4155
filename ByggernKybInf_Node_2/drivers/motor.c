/*
 * motor.c
 *
 * Created: 15.11.2022 15:19:04
 *  Author: thomahl
 */ 

#include "motor.h"
#include <sam.h>
#include <stdint.h>
#include "dac.h"

int16_t encoder_max;
int16_t current_encoder_value;



int16_t calibrate_motor() {
	// enable motor
	PIOD->PIO_SODR |= PIO_SODR_P9;

	
	// move motor to right
	PIOD->PIO_SODR |= PIO_SODR_P10;
	dac_convert(pow(2, 12) - 1);
	
	int16_t prev;
	int same_count = 0;
	int same_thr = 100;
	int buffer = 50;
	
	while (1) {
		int16_t curr = read_encoder();
		if (abs(curr - prev) <= buffer) {
			same_count += 1;
		} else {
			same_count = 0;
		}
		if (same_count > same_thr) {
			// we found right!
			break;
		}
		prev = curr;
	}
	
	// restart encoder
	PIOD->PIO_CODR |= PIO_CODR_P1;
	for(int i = 0; i < 10000; i++) {}
	PIOD->PIO_SODR |= PIO_SODR_P1;
	
	// move motor to the left
	PIOD->PIO_CODR |= PIO_CODR_P10;
	dac_convert(pow(2, 12) - 1);
	
	same_count = 0;
	prev = 0;
	
	while (1) {
		int16_t curr = read_encoder();
		if (abs(curr - prev) <= buffer) {
			same_count += 1;
		} else {
			same_count = 0;
		}
		if (same_count > same_thr) {
			// we found left!
			break;
		}
		prev = curr;
	}

	// read encoder value
	encoder_max = read_encoder();
	
	// move to middle
	PIOD->PIO_SODR |= PIO_SODR_P9;
	PIOD->PIO_SODR |= PIO_SODR_P10;
	dac_convert(pow(2, 10) + pow(2, 11) / 4);


	while (1) {
		int16_t curr = read_encoder();
		//printf("encoder value: %d %d\n\r", curr, encoder_max);
		if (curr <= (encoder_max/2) + 800) {
			break;
		}
	}
	
	// stop moving
	dac_convert(0);
}

int16_t read_encoder() {
	// set OE to low
	PIOD->PIO_CODR |= PIO_CODR_P0;
	// set SEL to loooooow
	PIOD->PIO_CODR |= PIO_CODR_P2;
	// wait 20 microseconds
	for(int i = 0; i < 10000; i++) {}
	// read MJ2 high byte
	uint8_t encoder_high_byte = (PIOC->PIO_PDSR & (PIO_PDSR_P1 | PIO_PDSR_P2 | PIO_PDSR_P3 | PIO_PDSR_P4 | PIO_PDSR_P5 | PIO_PDSR_P6 | PIO_PDSR_P7 | PIO_PDSR_P8)) >> 1;
	// set SEL to high
	PIOD->PIO_SODR |= PIO_SODR_P2;
	// wait 20 microseconds
	for(int i = 0; i < 10000; i++) {}
	// read MJ2 low byte
	uint8_t encoder_low_byte = (PIOC->PIO_PDSR & (PIO_PDSR_P1 | PIO_PDSR_P2 | PIO_PDSR_P3 | PIO_PDSR_P4 | PIO_PDSR_P5 | PIO_PDSR_P6 | PIO_PDSR_P7 | PIO_PDSR_P8)) >> 1;
	// set to OE to HIGH
	PIOD->PIO_SODR |= PIO_SODR_P0;
	
	return (encoder_high_byte << 8) | encoder_low_byte;
}

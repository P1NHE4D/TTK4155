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
#include "drivers/user_controls.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t adc_read(uint8_t channel);

int main(void) {
	UART_init(COMPUTED_UBRR);
	xmem_init();
	adc_init();
			
	pos_t pos = read_joystick_position();
	printf("neutral before calibrate: pos x,y %d,%d\n\r", pos.x, pos.y);
	
	joystick_calibrate();
	
	pos = read_joystick_position();
	printf("neutral after calibrate:  pos x,y %d,%d\n\r", pos.x, pos.y);
//	_delay_ms(2000);
	
	
	while (1) {
		pos = read_joystick_position();
		direction_t dir = read_joystick_direction();
		//printf("pos x,y,direction %3d,%3d,%d\n\r", pos.x, pos.y, dir);
		//printf("left,right %3d,%3d\n\r", read_slider(LEFT_SLIDER), read_slider(RIGHT_SLIDER));
		printf("button 1,2,3, %3d,%3d,%3d\n\r", read_button(JOYSTICK), read_button(LEFT_BUTTON), read_button(RIGHT_BUTTON));
	}
	
	

	
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
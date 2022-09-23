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
#include "drivers/oled.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// needed for include of <util/delay.h>
#define F_CPU FOSC
#include <util/delay.h>

int main(void) {
	UART_init(COMPUTED_UBRR);
	stdout = &UART_STREAM;
	xmem_init();
	sram_test();
	adc_init();
	OLED_init();
	
	//read_user_controls();
	oled_test();
	
	/*
	Exercise 3 task 5:
		Cutoff frequency: 795.8hz (using https://www.omnicalculator.com/physics/low-pass-filter, using R = 2000 Omh, C = 100 nF)
		Slope at cutoff frequency: ?
	*/
}

void oled_test() {
	stdout = &OLED_STREAM;

	OLED_goto_line(1);
	printf("This is line 1");
	OLED_goto_line(2);
	printf("This is line 2");
	OLED_goto_line(3);
	printf("This is line 3");
	OLED_clear_line(1);
	printf("cleared line 1");
	OLED_cursor_pos(6, 63);
	printf("mid 6");
	
	while (1) {
		for (int i = 0; i < 256; i++) {
			OLED_set_brightness(i);
			_delay_ms(50);
		}
		
		for (int i = 255; i >= 0; i--) {
			OLED_set_brightness(i);
			_delay_ms(50);
		}
	}
}

void read_user_controls() {
	joystick_calibrate();
	while (1) {
		pos_t pos = read_joystick_position();
		printf("joystick x,y slider left,right %4d,%4d,%4d,%4d\n\r", pos.x, pos.y, read_slider(LEFT_SLIDER), read_slider(RIGHT_SLIDER));
	}
}
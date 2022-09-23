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

int main(void) {
	UART_init(COMPUTED_UBRR);
	xmem_init();
	sram_test();
	adc_init();
	OLED_init();
	
	volatile char *ext_mem = (char*) OLED_BASE_ADDRESS;
	ext_mem[(1 << OLED_COMMAND_DATA_CONTROL_ADDRESS_BIT)] = 0;
	
	OLED_write_command(0x20);
	OLED_write_command(0b00);
	
	OLED_write_command(0x22);
	OLED_write_command(0b000);
	OLED_write_command(0b111);

	OLED_write_command(0x21);
	OLED_write_command(0b0000000);
	OLED_write_command(0b1111111);

	for (int i = 0; i < 95; i++) {
		OLED_print_char(i+32);
	}
	
	exit(0);
	int i = 0;
	while(1) {
		OLED_write_data(0);
		/*
		if (i % 2 == 0) {
			OLED_write_data(0b10101010);
		} else {
			OLED_write_data(0b01010101);
		}
		*/
		i++;
	}
	
	
	
	/*
	Exercise 3 task 5:
		Cutoff frequency: 795.8hz (using https://www.omnicalculator.com/physics/low-pass-filter, using R = 2000 Omh, C = 100 nF)
		Slope at cutoff frequency: ?
	*/
}

void read_user_controls() {
	joystick_calibrate();
	while (1) {
		pos_t pos = read_joystick_position();
		printf("joystick x,y %4d,%4d,%4d,%4d\n\r", pos.x, pos.y, read_slider(LEFT_SLIDER), read_slider(RIGHT_SLIDER));
	}
}
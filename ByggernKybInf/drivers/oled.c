/*
 * oled.c
 *
 * Created: 22.09.2022 14:43:00
 *  Author: thomahl
 */ 

#include <avr/pgmspace.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "oled.h"
#include "../defines.h"
#include "../fonts.h"

void OLED_write(volatile char payload, WRITE_MODE mode) {
	volatile char *ext_mem = (char*) OLED_BASE_ADDRESS;

	// set D/C# pin depending on mode
	uint16_t offset = 0;
	if (mode == WRITE_MODE_DATA) {
		// indicate as data by setting high
		offset |= (1 << OLED_COMMAND_DATA_CONTROL_ADDRESS_BIT);
	} else {
		// indicate as command by leaving low
	}
	
	//printf("%x,%x,%x\n\r", OLED_BASE_ADDRESS, offset, OLED_BASE_ADDRESS + offset);

	// write to OLED with OLED_COMMAND_DATA_CONTROL_ADDRESS_BIT bit of the
	// address used as D/C# to indicate data or command. Our address
	// decoding scheme will chip-select OLED, and the MCU will set WR and
	// put "payload" onto the data bus.
	ext_mem[offset] = payload;
}

void OLED_write_data(volatile char data) {
	OLED_write(data, WRITE_MODE_DATA);
}

void OLED_write_command(volatile char command) {
	OLED_write(command, WRITE_MODE_COMMAND);
}

char noop_receive() { }

// PDF:"OLED LY190-128064" section 9.4
void OLED_init() { 
	OLED_write_command(0xae); // display off
	OLED_write_command(0xa1); //segment remap
	OLED_write_command(0xda); //common pads hardware: alternative
	OLED_write_command(0x12);
	OLED_write_command(0xc8); //common output scan direction:com63~com0
	OLED_write_command(0xa8); //multiplex ration mode:63
	OLED_write_command(0x3f);
	OLED_write_command(0xd5); //display divide ratio/osc. freq. mode
	OLED_write_command(0x80);
	OLED_write_command(0x81); //contrast control
	OLED_write_command(0x50);
	OLED_write_command(0xd9); //set pre-charge period
	OLED_write_command(0x21);
	OLED_write_command(0x20); //Set Memory Addressing Mode
	OLED_write_command(0x02);
	OLED_write_command(0xdb); //VCOM deselect level mode
	OLED_write_command(0x30);
	OLED_write_command(0xad); //master configuration
	OLED_write_command(0x00);
	OLED_write_command(0xa4); //out follows RAM content
	OLED_write_command(0xa6); //set normal display
	OLED_write_command(0xaf); // display on
	OLED_reset();
}

void OLED_reset() {
	OLED_home();
	for (int i = 0; i < 8; i++) {
		OLED_clear_line(i);
	}
	OLED_home();
}

void OLED_home() {
	OLED_cursor_pos(0, 0);
}

void OLED_goto_line(uint8_t line) {
	OLED_cursor_pos(line, 0);
}

// note: will result in goto line 'line'
void OLED_clear_line(uint8_t line) {
	OLED_goto_line(line);
	for (int i = 0; i < 128; i++) {
		OLED_write_data(0);
	}
	// cursor will wrap back to start at next write,
	// but goto line again just in case
	OLED_goto_line(line);
}

// note: end position is always max,max
void OLED_cursor_pos(uint8_t row, uint8_t column) {
	OLED_write_command(0x22);
	OLED_write_command(row);
	OLED_write_command(0b111);

	OLED_write_command(0x21);
	OLED_write_command(column);
	OLED_write_command(0b1111111);
}

void OLED_print_char(char c) {
	// get 8x8 version of character from fonts
	char font_char[8];
	for (int i = 0; i < 8; i++) {
		font_char[i] = pgm_read_byte(&(font8[c - 32][i]));
	}

	// draw font_char onto the oled segment by segment
	for (int i = 0; i < 8; i++) {
		OLED_write_data(font_char[i]);
	}
}

void OLED_print(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		OLED_print_char(str[i]);
	}
}

void OLED_set_brightness(uint8_t lvl) {
	OLED_write_command(0x81); // Set Contrast Control
	OLED_write_command(lvl);
}

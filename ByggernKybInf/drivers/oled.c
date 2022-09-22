/*
 * oled.c
 *
 * Created: 22.09.2022 14:43:00
 *  Author: thomahl
 */ 

#include <avr/pgmspace.h>
#include <string.h>
#include "oled.h"
#include "../defines.h"
#include "../fonts.h"

void OLED_write(volatile char word, WRITE_MODE mode) {
	volatile char *ext_mem = (char*) OLED_BASE_ADDRESS;

	// set D/C# pin depending on mode
	uint16_t offset = 0;
	if mode == WRITE_MODE_DATA {
		// indicate as data by setting high
		offset |= (1 << OLED_COMMAND_DATA_CONTROL_ADDRESS_BIT);
	} else {
		// indicate as command by leaving low
	}

	// write to OLED with OLED_COMMAND_DATA_CONTROL_ADDRESS_BIT bit of the
	// address used as D/C# to indicate data or command. Our address
	// decoding scheme will chip-select OLED, and the MCU will set WR and
	// put "word" onto the data bus.
	ext_mem[offset] = word;
}

void OLED_write_data(volatile char data) {
	OLED_write(command, WRITE_MODE_DATA);
}

void OLED_write_command(volatile char command) {
	OLED_write(command, WRITE_MODE_COMMAND);
}

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
}

void OLED_reset() {
	// TODO
	//  don't know what this is supposed to do...
	//  clear screen and go to home?
}

void OLED_home() {
	// TODO
}

void OLED_goto_line(int line) {
	// TODO
}

void OLED_clear_line(int line) {
	// TODO
}

void OLED_pos(int row, int column) {
	// TODO
}

void OLED_print_char(char c) {
	// get 8x8 version of character from fonts
	char font_char[8] = pgm_read_byte(&(font8[c - 32]));

	for (int i = 0; i < 8; i++) {
		OLED_write_data(font_char[i]);
	}
}

void OLED_print(char* str) {
	for (int i = 0; i < strlen(string); i++) {
		OLED_print_char(str[i]);
	}
}

void OLED_set_brightness(int lvl) {
	// TODO
}

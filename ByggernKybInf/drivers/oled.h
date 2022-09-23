/*
 * oled.h
 *
 * Created: 22.09.2022 14:43:00
 *  Author: thomahl
 */ 

#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>
#include <stdio.h>

typedef enum {
	WRITE_MODE_DATA,
	WRITE_MODE_COMMAND,
} WRITE_MODE;

void OLED_write(volatile char payload, WRITE_MODE mode);

void OLED_write_data(volatile char data);

void OLED_write_command(volatile char command);

void OLED_init(); // PDF:"OLED LY190-128064" section 9.4

void OLED_reset();

void OLED_home();

void OLED_goto_line(uint8_t line);

void OLED_clear_line(uint8_t line);

void OLED_cursor_pos(uint8_t row, uint8_t column);

void OLED_print_char(char c);

void OLED_print(char* str);

void OLED_set_brightness(uint8_t lvl);

char noop_receive(void);

static FILE OLED_STREAM = FDEV_SETUP_STREAM(OLED_print_char, noop_receive, _FDEV_SETUP_WRITE);

#endif /* OLED_H_ */

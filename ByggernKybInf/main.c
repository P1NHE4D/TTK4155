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
#include "drivers/menu.h"
#include "drivers/spi.h"
#include "drivers/mcp2515.h"
#include "drivers/can.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// needed for include of <util/delay.h>
#define F_CPU FOSC
#include <util/delay.h>


void init() {
	UART_init(COMPUTED_UBRR);
	stdout = &UART_STREAM;
	xmem_init();
	sram_test();
	adc_init();
	OLED_init();
	user_controls_init();
	/* MCP2515 calls this for us
	SPI_init();
	*/
	mcp2515_init();
	
	//read_user_controls();
}

void loop() {
	volatile menu_node_t *ext_mem = (char*) 0x1800;
	
	menu_node_t* main_node = malloc(sizeof(menu_node_t));
	strcpy(main_node->name, "Main menu");
	main_node->children_count = 3;
	main_node->parent = NULL;
	memcpy(ext_mem, main_node, sizeof(menu_node_t));
	free(main_node);
	
	menu_node_t* play_node = malloc(sizeof(menu_node_t));
	play_node->parent = ext_mem;
	strcpy(play_node->name, "Play");
	play_node->children_count = 0;
	memcpy(ext_mem + 1, play_node, sizeof(menu_node_t));
	free(play_node);
	
	menu_node_t* highscore_node = malloc(sizeof(menu_node_t));
	highscore_node->parent = ext_mem;
	strcpy(play_node->name, "High Score");
	highscore_node->children_count = 0;
	memcpy(ext_mem + 2, play_node, sizeof(menu_node_t));
	free(highscore_node);
	
	menu_node_t* character_node = malloc(sizeof(menu_node_t));
	character_node->parent = ext_mem;
	strcpy(play_node->name, "Characters");
	character_node->children_count = 3;
	memcpy(ext_mem + 3, play_node, sizeof(menu_node_t));
	free(character_node);
	
	ext_mem->children[0] = ext_mem + 1;
	ext_mem->children[1] = ext_mem + 2;
	ext_mem->children[2] = ext_mem + 3;
	
	menu_node_t* marte_node = malloc(sizeof(menu_node_t));
	marte_node->parent = ext_mem + 3;
	strcpy(play_node->name, "Marte");
	marte_node->children_count = 0;
	memcpy(ext_mem + 4, marte_node, sizeof(menu_node_t));
	free(marte_node);
	
	menu_node_t* alexander_node = malloc(sizeof(menu_node_t));
	alexander_node->parent = ext_mem + 3;
	strcpy(play_node->name, "Alexander");
	alexander_node->children_count = 0;
	memcpy(ext_mem + 5, alexander_node, sizeof(menu_node_t));
	free(alexander_node);
	
	menu_node_t* thomas_node = malloc(sizeof(menu_node_t));
	thomas_node->parent = ext_mem + 3;
	strcpy(play_node->name, "Thomas");
	thomas_node->children_count = 0;
	memcpy(ext_mem + 6, thomas_node, sizeof(menu_node_t));
	free(thomas_node);
	
	(ext_mem + 3)->children[0] = ext_mem + 4;
	(ext_mem + 3)->children[1] = ext_mem + 5;
	(ext_mem + 3)->children[2] = ext_mem + 6;
	
	main_node = ext_mem;
	uint8_t cursor_child = 0;
	
	stdout = &OLED_STREAM;
	uint8_t current_line = 0;
	OLED_goto_line(current_line);
	printf(main_node->name);
	for (uint8_t i = 0; i < main_node->children_count; i++) {
		OLED_goto_line(++current_line);
		if(cursor_child == i) {
			printf("> ");
			} else {
			printf("  ");
		}
		printf(main_node->children[i]->name);
	}
	
	menu_node_t* current_menu = main_node;
	
	bool engaged = false;
	bool button_clicked = false;
	
	while (1) {
		bool should_refresh = false;

		direction_t direction = read_joystick_direction();
		stdout = &UART_STREAM;
		bool is_button_pressed = read_button(JOYSTICK);
		if(is_button_pressed && !button_clicked) {
			button_clicked = true;
			if (cursor_child < current_menu->children_count) {
				current_menu = current_menu->children[cursor_child];
				} else {
				// go back
				current_menu = current_menu->parent;
			}
			should_refresh = true;
			cursor_child = 0;
			OLED_reset();
			} else if (!is_button_pressed) {
			button_clicked = false;
		}
		
		int lines_with_back = current_menu->parent != NULL ? current_menu->children_count + 1 : current_menu->children_count;

		if (direction == UP && !engaged) {
			cursor_child = 0 > cursor_child - 1 ? 0 : cursor_child - 1;
			engaged = true;
			should_refresh = true;
			} else if (direction == DOWN && !engaged) {
			cursor_child = lines_with_back - 1 < cursor_child + 1 ? lines_with_back - 1 : cursor_child + 1;
			engaged = true;
			should_refresh = true;
			} else if (direction == NEUTRAL){
			engaged = false;
		}
		
		if (!should_refresh) {
			continue;
		}
		
		
		stdout = &OLED_STREAM;
		uint8_t current_line = 0;
		OLED_goto_line(current_line);
		printf(current_menu->name);
		for (uint8_t i = 0; i < lines_with_back; i++) {
			OLED_goto_line(++current_line);
			if(cursor_child == i) {
				printf("> ");
				} else {
				printf("  ");
			}
			
			if (i < current_menu->children_count) {
				printf(current_menu->children[i]->name);
				} else {
				printf("Back");
			}
		}
	}
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

int main(void) {
	init();
	
	uint8_t status = mcp2515_rx_status();
	printf("status before doing anything is is %d\n\r", status);
	
	/*CAN_standard_message_t msg;
	msg.id = 3;
	msg.rtr = 0;
	msg.dlc = 8;
	msg.data[0] = 5;
	
	printf("I sent id: %d data: %d\n\r", msg.id, msg.data[0]);
	CAN_send(msg);*/
	while(1){
		CAN_standard_message_t msg;
		msg.id = 13;
		msg.rtr = 0;
		msg.dlc = 2;
		
		pos_t joystick_pos = read_joystick_position();
		printf("x: %d  y: %d\n\r", joystick_pos.x, joystick_pos.y);
		msg.data[0] = joystick_pos.x + 100;
		msg.data[1] = joystick_pos.y + 100;
		
		CAN_send(msg);
	}
	
	
	/*
	_delay_ms(200);
	
	status = mcp2515_rx_status();
	printf("status is %d\n\r", status);
	
	CAN_standard_message_t received_msg = CAN_receive();
	printf("I received id: %d data: %d\n\r", received_msg.id, received_msg.data[0]);
	*/
	loop();
	
	/*
	Exercise 3 task 5:
		Cutoff frequency: 795.8hz (using https://www.omnicalculator.com/physics/low-pass-filter, using R = 2000 Omh, C = 100 nF)
		Slope at cutoff frequency: ?
	*/
}
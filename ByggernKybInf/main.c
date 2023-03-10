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

void play_game();


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
	main_node->children_count = 1;
	main_node->parent = NULL;
	memcpy(ext_mem, main_node, sizeof(menu_node_t));
	free(main_node);
	
	menu_node_t* play_node = malloc(sizeof(menu_node_t));
	play_node->parent = ext_mem;
	strcpy(play_node->name, "Play");
	play_node->children_count = 0;
	play_node->on_click = play_game;
	memcpy(ext_mem + 1, play_node, sizeof(menu_node_t));
	free(play_node);
	ext_mem->children[0] = ext_mem + 1;
	

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
				OLED_reset();
				current_menu->on_click();
				current_menu = current_menu->parent;
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

void play_game(){

	// signal start of game
	CAN_standard_message_t msg;
	msg.id = 5;
	msg.dlc = 0;
	CAN_send(msg);
	
	while(1){
		CAN_standard_message_t msg;
		msg.id = 13;
		msg.rtr = 0;
		msg.dlc = 3;
		
		pos_t joystick_pos = read_joystick_position();
		bool button_pressed = read_button(JOYSTICK);
		printf("x: %d  y: %d button: %d\n\r", joystick_pos.x, joystick_pos.y, button_pressed);
		msg.data[0] = joystick_pos.x + 100;
		msg.data[1] = joystick_pos.y + 100;
		msg.data[2] = button_pressed;
		
		CAN_send(msg);
		
		CAN_standard_message_t recv_msg = CAN_receive();
		uint32_t score = (recv_msg.data[3] << 3 | recv_msg.data[2] << 2 | recv_msg.data[1] << 1 | recv_msg.data[0]);
		if (recv_msg.id == 1) {
			stdout = &OLED_STREAM;
			OLED_clear_line(0);
			printf("GAME OVER");
			OLED_goto_line(1);
			printf("FINAL SCORE: %d", score);
			stdout = &UART_STREAM;
			_delay_ms(5000);
			return;
		} else {
			printf("Score: %d\n\r", score);
			stdout = &OLED_STREAM;
			OLED_clear_line(0);
			printf("Score: %d", score);
			stdout = &UART_STREAM;
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
	
	
	loop();
}
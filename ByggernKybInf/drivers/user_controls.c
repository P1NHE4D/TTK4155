#include "avr/io.h"
#include "user_controls.h"
#include "adc.h"
#include <stdbool.h>
#include <stdint.h>

uint8_t joystick_neutral_x;
uint8_t joystick_neutral_y;

void user_controls_init() {
	joystick_calibrate();
	PORTB |= (1 << PB0);
}

void joystick_calibrate() {
	joystick_neutral_x = adc_read(JOYSTICK_X);
	joystick_neutral_y = adc_read(JOYSTICK_Y);
	printf("neutral is %d,%d\n\r", joystick_neutral_x, joystick_neutral_y);
}

pos_t read_joystick_position() {
	uint8_t raw_x = adc_read(JOYSTICK_X);
	uint8_t raw_y = adc_read(JOYSTICK_Y);
	
	int denom_x = joystick_neutral_x;
	if (raw_x > joystick_neutral_x) {
		denom_x = 255 - joystick_neutral_x;
	}
	
	int denom_y = joystick_neutral_y;
	if (raw_y > joystick_neutral_y) {
		denom_y = 255 - joystick_neutral_y;
	}

	pos_t pos;
	pos.x = (100 * (adc_read(JOYSTICK_X) - joystick_neutral_x)) / denom_x;
	pos.y = (100 * (adc_read(JOYSTICK_Y) - joystick_neutral_y)) / denom_y;
    return pos;
}

#define JOYSTICK_TRESHOLD 10

direction_t read_joystick_direction() {
    pos_t pos = read_joystick_position();

	bool within_threshold_x = -JOYSTICK_TRESHOLD < pos.x && pos.x < JOYSTICK_TRESHOLD;
	bool within_threshold_y = -JOYSTICK_TRESHOLD < pos.y && pos.y < JOYSTICK_TRESHOLD;
	if (within_threshold_x && within_threshold_y) {
			
		return NEUTRAL;
	}
	
	if (pos.y < 0 && pos.x > -50 && pos.x < 50) {
		return DOWN;
	} else if (pos.y > 0 && pos.x > -50 && pos.x < 50) {
		return UP;
	} else if (pos.x < 0 && pos.y > -50 && pos.y < 50) {
		return LEFT;
	} else if (pos.x > 0 && pos.y > -50 && pos.y < 50) {
		return RIGHT;
	}
	return NEUTRAL;
}

uint8_t read_slider(channel_t slider) {
   return (uint8_t) (100 * adc_read(slider) / 255);
}

bool read_button(button_t button) {
    switch (button) {
        case LEFT_BUTTON:
			return (PINB & (1 << PINB2));
        case RIGHT_BUTTON:
			return (PINB & (1 << PINB1));
        case JOYSTICK:
            return !(PINB & (1 << PINB0));
    }
}
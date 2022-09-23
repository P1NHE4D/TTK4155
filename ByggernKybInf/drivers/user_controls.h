#ifndef USER_CONTROL_H_
#define USER_CONTROL_H_

#include <stdbool.h>
#include "adc.h"

typedef struct pos_t {
	int8_t x, y;
} pos_t;
typedef enum { UP, RIGHT, DOWN, LEFT, NEUTRAL } direction_t;
typedef enum {JOYSTICK, LEFT_BUTTON, RIGHT_BUTTON} button_t;

void joystick_calibrate(void);
pos_t read_joystick_position(void);
direction_t read_joystick_direction(void);
uint8_t read_slider(channel_t slider);
bool read_button(button_t button);

#endif /* USER_CONTROL_H_ */
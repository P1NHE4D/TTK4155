/*
 * pid_interrupt.c
 *
 * Created: 11.11.2022 16:36:28
 *  Author: thomahl
 */ 

#include <sam.h>
#include "can_interrupt.h"
#include "motor.h"

const double kp = 0.75;
const double ki = 0.1;
const double kd = 0;
int32_t total_err = 0;
int32_t curr_err = 0;
int32_t prev_err = 0;
const double t = 0.5;
int32_t motor_output = 0;


void TC0_Handler (void) {
	// clear interrupt flag
	volatile uint32_t r = REG_TC0_SR0;
	
	// scale joystick position
	int16_t scaled_joystick_y = joystick_position_y * (encoder_max/200);

	int16_t desired = scaled_joystick_y;
	int16_t current = current_encoder_value;
	
	curr_err = desired - current;
	total_err += curr_err;
		
	// output signal to motor
	motor_output = kp * curr_err + t * ki * total_err + (kd / t) * (curr_err - prev_err);
	
	prev_err = curr_err;
}
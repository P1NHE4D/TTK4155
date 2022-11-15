/*
 * motor.h
 *
 * Created: 15.11.2022 15:20:05
 *  Author: thomahl
 */ 

#include <stdint.h>

#ifndef MOTOR_H_
#define MOTOR_H_

extern int16_t encoder_max;
extern int16_t current_encoder_value;

int16_t calibrate_motor();

int16_t read_encoder();


#endif /* MOTOR_H_ */
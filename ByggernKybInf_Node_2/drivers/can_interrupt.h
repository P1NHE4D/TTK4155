/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. Jølsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_

#include <stdint.h>
#include <stdbool.h>

extern uint8_t joystick_position_x;
extern uint8_t joystick_position_y;
extern bool button_pressed;
extern uint16_t can_msg_id;


void CAN0_Handler       ( void );






#endif /* CAN_INTERRUPT_H_ */
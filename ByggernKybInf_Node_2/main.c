/*
 * ByggernKybInf_Node_2.c
 *
 * Created: 14.10.2022 09:33:50
 * Author : thomahl
 */ 


#include <stdbool.h>
#include <math.h>
#include "sam.h"
#include "drivers/uart.h"
#include "drivers/can_controller.h"
#include "drivers/pwm.h"
#include "drivers/can_interrupt.h"
#include "drivers/dac.h"
#include "drivers/timer.h"
#include "drivers/motor.h"
#include "drivers/pid_interrupt.h"


uint16_t max_motor_actuation = pow(2, 12) - 1;
bool pid_motor_control_enabled = false;

int main(void)
{
    /* Initialize the SAM system */
	SystemInit();
	pwm_init();
	adc_init();

	
	// disable watchdog
	//PMC->PMC_PCER0 &= ~(1<<4);
	WDT->WDT_MR = WDT_MR_WDDIS;
		
	// clock signal enable for peripherals
	PMC->PMC_PCER0 |= PMC_PCER0_PID11;
	PMC->PMC_PCER0 |= PMC_PCER0_PID8;
	
	// Pin Enable Register
    PIOA->PIO_PER |= PIO_PER_P19 | PIO_PER_P20;
	
	// Output Enable Register
    PIOA->PIO_OER |= PIO_OER_P19 | PIO_OER_P20;
	
	// Clear Output Data Register
    PIOA->PIO_CODR |= PIO_CODR_P19 | PIO_CODR_P20;
	
	// Set Output Data Register
    PIOA->PIO_SODR |= PIO_SODR_P19 | PIO_SODR_P20;
	
	// Appears as com10 in putty, with baud rate 9600, stop bits 2, parity None
	
	// needs to happen after uart, since there is debug output
	dac_init();
	configure_uart();
	timer_init();

	printf("Starting...\n\r");
	
	/* configure enable pin of mj1 */
	
		
	// enable PIOD
	PMC->PMC_PCER0 |= PMC_PCER0_PID14;
	
	// Pin Enable Register
	PIOD->PIO_PER |= (PIO_PER_P9 | PIO_PER_P10 | PIO_PER_P3 | PIO_PER_P0 | PIO_PER_P2 | PIO_PER_P0 | PIO_PER_P1);
	
	// Output Enable Register
	PIOD->PIO_OER |= (PIO_OER_P9 | PIO_OER_P10 | PIO_OER_P3 | PIO_OER_P2 | PIO_OER_P0 | PIO_PER_P1);
	
	// Clear Output Data Register
	PIOD->PIO_CODR |= (PIO_CODR_P9 | PIO_CODR_P10 | PIO_CODR_P3);
	
	// reset encoder
	PIOD->PIO_CODR |= PIO_CODR_P1;
	for(int i = 0; i < 10000; i++) {}
	PIOD->PIO_SODR |= PIO_SODR_P1;
	
	
	
	/* enable PIOC */
	PMC->PMC_PCER0 |= PMC_PCER0_PID13;
	
	/*  PIN enable register */
	PIOC->PIO_PER |= (PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4 | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8);
	
	/*
	
	// Set Output Data Register
	PIOD->PIO_SODR |= (PIO_SODR_P9 | PIO_SODR_P10);
	
	uint16_t value_to_convert = pow(2, 12) - 1;
	
	PIOD->PIO_CODR |= PIO_CODR_P10;
	for (int i = 0; i < 100000; i++) {
		dac_convert(value_to_convert);
	}
	PIOD->PIO_CODR |= PIO_CODR_P9;
	//dac_convert(value_to_convert);
	//dac_convert(value_to_convert);
	*/
	
	/*
	 * Build CAN_BR
	 */
	// components, each starting at LSB (must be shifted into CAN_BR)
	uint32_t phase_2 = 3; // (TA: 5) phase 2 segment               phase_2 = (t_bit_time (8TQ) - T_prs - T_sync) / 2
	uint32_t phase_1 = 3; // (TA: 6) phase 1 segment               phase_1 = (t_bit_time (8TQ) - T_prs - T_sync) / 2
	uint32_t propag = 0;  // (TA: 1) programming time segment      250*10^-9 / ((1/500000)/8) - 1 = 1 (TQ), progag = 1 - 1 TQ
	uint32_t sjw = 0;     // re-synchronization jump width
	uint32_t brp = 20;    // (TA: 41) baudrate prescaler            ((((1/500000)/8)*84000000)-1)
	uint32_t smp = 0;     // sampling mode (just has to be the same as on node 1)
	
	// "can_br Value to be set in CAN0->CAN_BR register to match can bus bit timing"
	uint32_t can_br;
	
	// set bits of can_br following the pattern ((value & mask) << offset)
	can_br = 0
		| ((phase_2 & 0b111)     << 0)
		| ((phase_1 & 0b111)     << 4)
		| ((propag  & 0b111)     << 8)
		| ((sjw     & 0b11)      << 12)
		| ((brp     & 0b1111111) << 16)
		| ((smp     & 0b1)       << 24)
		;
		
	// can_br = 0x00290165;
	int status = can_init_def_tx_rx_mb(can_br);
	if (status != 0) {
		uart_putchar('b');
		uart_putchar('a');
		uart_putchar('d');
	}
	while (1) {
		if (can_msg_id == 5) {	
			play_game();
		}
	}
}

uint16_t min(uint16_t a, uint16_t b) {
	if (a < b) {
		return a;
	}
	return b;
}

void motor_control_bang_bang() {
	// hacky hacky hacky ho, control trakc thingy with joystick_y
	uint8_t middle = 100;
	uint16_t speed;
	if (joystick_position_y > middle) {
		// left
		speed = joystick_position_y - middle;
		PIOD->PIO_CODR |= PIO_CODR_P10;
	} else {
		// right
		speed = middle - joystick_position_y;
		PIOD->PIO_SODR |= PIO_SODR_P10;
	}
	if (speed < 20) {
		PIOD->PIO_CODR |= PIO_CODR_P9;
	} else {
		PIOD->PIO_SODR |= PIO_SODR_P9;
	}

	dac_convert((speed + 55) << 4);
}

void motor_control_pid() {
	/* 
	 set direction and speed from motor_speed (which is signed)
	 */
		
	uint16_t unclamped_motor_actuation = abs(motor_output); // todo: scale and whatever
	uint16_t clamped_motor_actuation = min(unclamped_motor_actuation, max_motor_actuation);
		
	if (motor_output > 0) {
		// left
		PIOD->PIO_CODR |= PIO_CODR_P10;
	} else {
		// right
		PIOD->PIO_SODR |= PIO_SODR_P10;
	}
		
	dac_convert(clamped_motor_actuation);
}

void play_game() {
	/* Replace with your application code */
	uint32_t score = 0;
	uint8_t thr = 2000;
	uint8_t cycle_thr = 10;
	uint16_t max_motor_actuation = pow(2, 12)-1;
	CAN_MESSAGE can_msg;
	
	uint32_t blocked_cycles = 0;
	
	if (pid_motor_control_enabled) {
		calibrate_motor();
	}
	
	for (int i = 0;; i++) {
		if (button_pressed) {
			PIOD->PIO_CODR |= PIO_CODR_P3;
			} else {
			PIOD->PIO_SODR |= PIO_SODR_P3;
		}
		current_encoder_value = read_encoder();
		
		// read joystick position from global
		// variable (set by interrupt) and modify
		// duty cycle
		uint8_t duty_cycle_e2 = ((200 - joystick_position_x) / (200/(21-9))) + 9;
		pwm_set_duty_cycle(duty_cycle_e2);
		
		if (pid_motor_control_enabled) {
			motor_control_pid();
		} else {
			motor_control_bang_bang();
		}
		
		uint16_t result = adc_read();
		
		if (result > thr) {
			blocked_cycles = 0;
			} else {
			blocked_cycles += 1;
		}
		
		can_msg.data[3] = score >> 3;
		can_msg.data[2] = score >> 2;
		can_msg.data[1] = score >> 1;
		can_msg.data[0] = score;
		can_msg.data_length = 4;
		if (blocked_cycles < cycle_thr) {
			if (i % 1000 == 0) {
				score += 1;
				printf("score: %d \n\r", score);
				printf("adc is %d\n\r", result);
			}
			} else {
			printf("adc is %d\n\r", result);
			printf("u suck, u got %d points \n\r", score);
			can_msg.id = 1;
			can_send(&can_msg, 0);
			return;
		}
		can_msg.id = 11;
		can_send(&can_msg, 0);
	}
}

/*
 * ByggernKybInf_Node_2.c
 *
 * Created: 14.10.2022 09:33:50
 * Author : thomahl
 */ 


#include <stdbool.h>
#include "sam.h"
#include "drivers/uart.h"
#include "drivers/can_controller.h"
#include "drivers/pwm.h"
#include "drivers/can_interrupt.h"


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
	configure_uart();
	

	printf("Starting...\n\r");
	
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
	} else {
		/* Replace with your application code */
		uint32_t score = 0;
		uint8_t thr = 2000;
		
		for (int i = 0;; i++) {
			// read joystick position from global
			// variable (set by interrupt) and modify
			// duty cycle
			uint8_t duty_cycle_e2 = ((200 - joystick_position_x) / (200/(21-9))) + 9;
			pwm_set_duty_cycle(duty_cycle_e2);
			
			uint16_t result = adc_read();

			if (result > thr) {
				if (i % 100000 == 0) {
					score += 1;
					printf("score: %d \n\r", score);	
				}
			} else {
				printf("u suck, u got %d points \n\r", score);
				break;
			}

		}
	}
}

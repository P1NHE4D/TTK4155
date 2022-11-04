/*
 * pwm.h
 *
 * Created: 28.10.2022 11:34:24
 *  Author: thomahl
 */ 


#ifndef PWM_H_
#define PWM_H_

uint32_t cprd;
uint32_t period_ms;

void pwm_init();

void pwm_set_duty_cycle(uint8_t duty_cycle_ms_e2);



#endif /* PWM_H_ */
/*
 * pwm.c
 *
 * Created: 28.10.2022 11:34:55
 *  Author: thomahl
 */

#include "sam.h"


void pwm_init() {
	// in general: see 38.6.5.1 for initialization sequence
	
	// TA said we need this...
	PMC->PMC_PCR |= PMC_PCR_EN | (ID_PWM << PMC_PCR_PID_Pos) | (0 << PMC_PCR_DIV_Pos);

	// enable the clock of the peripheral. We will use PIN44 on
	// the shield, which corresponds to PC19
	// (use PIN44 because the lab lecture recommends PIN44 or PIN45)
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;

	// we want to use peripheral function B, which is PWMH5 according
	// to table 9-4. Section 31.5.3 details how to select A or B:
	//   "The selection is performed by writing PIO_ABSR. [...] For
	//   each pin, the corresponding bit at level 0 means peripheral
	//   A is selected whereas the corresponding bit at level 1
	//   indicates that peripheral B is selected"
	// * also note that the default is 0 -> peripheral A, but that I/O
	//   line mode is default (so: we need to enable B, and we need to
	//   disable I/O)
	// * also also note that we have to BOTH configure PIO_ABSR AND
	//   PIO_PDR. PIO_ABSR does not automatically disable I/O.
	// * PWMH5 is, according to table 38-1, "PWM Waveform Output High
	//   for channel 5". Ergo: we are using channel 5
	PIOC->PIO_ABSR |= PIO_ABSR_P19;
	
	// disable PIO so we can use the desired peripheral function
	//
	// from the datasheet:
	//	"Disables the PIO from controlling the corresponding pin
	//	(enables peripheral control of the pin)"
	PIOC->PIO_PDR |= PIO_PDR_P19;

	
	// TODO:
	//   according to 38.6.5 we need to "Unlock User Interface by
	//   writing the WPCMD field in the PWM_WPCR Register" as a first
	// step in PWM initialization
	
	// PWM clock register. TA quote: "you set the clock prescaler
	// and divider to create a pwm clock (clock for the pwm
	// peripheral). I used a 2Mhz clock in my code"
	//
	// diva of 42 achieves a clka of 2Mhz exactly :)
	//
	// Only configure diva, leaving prea as 0, so we achieve clka
	// of mck/diva
	uint32_t diva = 42;
	PWM->PWM_CLK |= diva;
	
	// PWM Channel Mode Register. TA quote: "set clock source for
	// a specific PWM channel, and a mode. I used a dual slope pwm mode"
	// * CPRE: Use CLKA, which we configured to be exactly 2Mhz earlier
	//   (value from CPRE table under register 38.7.37)
	// * CALG - Channel Alignment: to match example in 38.6.2.2 we want
	//   left alignment, which is 0
	// * CPOL - Channel Polarity: we want to start at high, so set 1
	// * CES - Counter Event Selection: don't care
	// * DTE - Dead-Time Generator Enable: don't care
	// * DTHI - Dead-Time PWMHx Output Inverted: don't care
	// * DTLI - Dead-Time PWMLx Output Inverted: don't care
	uint32_t calg = 0b0;
	uint32_t cpre = 0b1011;
	uint32_t cpol = 0b1;
	
	REG_PWM_CMR5 |= 0
		| ((cpre & 0b1111) << 0)
		| ((calg & 0b1)    << 8)
		| ((cpol & 0b1)    << 9)
		;
	
	// PWM channel period register. TA quote: "set to desired value(s)"
	// from the datasheet:
	//   "By using the PWM master clock (MCK) divided by an X given
	//   prescaler value (with X being 1,2,4,8,16,32,64,128,256,512,1024),
	//   the resulting period formula will be ((X * CPRD) / MCK).
	// The period must be 20 ms so solve the formula
	// ((X * CPRD) / MCK) = 50ms for CPRD
	uint32_t cprd = 40000;
	REG_PWM_CPRD5 |= cprd;
	
	// PWM Channel Duty Cycle Register. TA quote: "set to desired value(s)"
	// from the datasheet:
	//   "This value must be defined between 0 and CPRD (PWM_CPRx)
	// Initialize to duty cycle of 1.5 ms
	// ((1.5 * 10^-3) / (20 * 10^-3)) * crpd
	uint32_t cdty = 3000;
	REG_PWM_CDTY5 |= cdty;
	
	// PWM Enable Register. TA quote: "enable the PWM channel in the PWM
	// Enable Register"
	PWM->PWM_ENA |= PWM_ENA_CHID5;
}
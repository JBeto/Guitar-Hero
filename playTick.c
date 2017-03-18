#ifndef PLAY_TICK
#define PLAY_TICK

#include "pwm.c"

// ----------------- Period -----------------
//50 ms
// ------------------------------------------

enum PWM_States {PWM_init, setFreq, PWM_reset};
	
int playTick(int PWM_state) {
	switch (PWM_state) {
		case PWM_init:
			if (PWM_freq != 0) {
				PWM_on();
				PWM_state = setFreq;
			}
			break;
		case setFreq:
			if (endGame) {
				PWM_state = PWM_reset;
			}
			break;
		case PWM_reset:
			PWM_off();
			PWM_state = PWM_init;
			break;
		default:
			break;
	}
	
	switch (PWM_state) {
		case PWM_init:
			break;
		case setFreq:
			set_PWM(PWM_freq);
			break;
		case PWM_reset:
			break;
		default:
			break;
	}
	
	return PWM_state;
}

#endif

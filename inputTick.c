#ifndef INPUT_TICK_C
#define INPUT_TICK_C

#include "inputs.c"

// ----------------------- Period ----------------------------
//unsigned char input_period = 250;
// -----------------------------------------------------------

// -------------------- Global variables ---------------------
unsigned char score = 0;
// -----------------------------------------------------------

// -------------------------Input SM -------------------------
enum Input_States {WaitScore, ScorePoints};
	
int inputTick(int input_state) {
	switch(input_state) {
		case WaitScore:
			if (notePos) {
				input_state = ScorePoints;
			}
			break;
		case ScorePoints:
			if (endGame) {
				input_state = WaitScore;
			}
			break;
		default:
			break;
	}
	
	switch(input_state) {
		case WaitScore:
			break;
		case ScorePoints:
			if ( (joystick_arrow() == currDir) && (button_arrow() == currDir) ) {
				++score;
			}
			break;
		default:
			break;
	}
	
	return input_state;
}
// -----------------------------------------------------------

#endif

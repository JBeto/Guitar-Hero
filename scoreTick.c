#ifndef SCORE_TICK_C
#define SCORE_TICK_C

#include "print.c"

// ------------------ Global Variables ------------------
//unsigned short displayPeriod = 250;
//unsigned char score = 0; //input
//unsigned char endGame = 0; //input
//unsigned char endPoints;
// ------------------------------------------------------

// ------------------- Score State SM -------------------
enum scoreStates {Wait, Display};

int scoreTick(int scoreState) {
	
	switch(scoreState) {
		case Wait:
			if (endGame) {
				scoreState = Display;
				printScore(score);
			}
			break;
		case Display:
			if (GetKeypadKey() == '1') {
				scoreState = Wait;
				endPoints = 1;
				clearDisplay();
			}
			break;
		default:
			break;
	}
	
	switch(scoreState) {
		case Wait:
			break;
		case Display:
			break;
		default:
			break;
	}
	
	return scoreState;
}
// ------------------------------------------------------

#endif

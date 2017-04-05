#ifndef SELECT_TICK_C
#define SELECT_TICK_C

#include "print.c"
#include "inputs.c"

// -------------------- Global variables ----------------
//unsigned char endDisplay = 0; 
//unsigned short update_period = 0;
unsigned char sel_period = 100;
unsigned char endPoints = 0; //bool for end display, return to menu
// ------------------------------------------------------

// ------------------------ Select Tick SM ------------------------
enum selStates {InitChoice, Change, Hold, Enter, ResetChoice};

int selectTick(int sel_state) {
	static unsigned char holdCount = 0;
	static unsigned char choice = 1;
	unsigned char j_stick = joystick_arrow();
	
	switch(sel_state) {
		case InitChoice:
			sel_state = Change;
			printChoice(choice);
			break;
		case Change:
	
			if (GetKeypadKey() == '1') {
				sel_state = Enter;
			}
			else {
				if (j_stick == LEFT) {
					if (choice > 1) {
						--choice;
					}
					printChoice(choice);
					sel_state = Hold;
				}
				else if (j_stick == RIGHT) {
					if (choice < 8) {
						++choice;
					}
					printChoice(choice);
					sel_state = Hold;
				}
			}
			break;
		case Hold:
			if (!j_stick) {
				sel_state = Change;
				holdCount = 0;
			}
			else if (holdCount >= 1000/sel_period) {
				sel_state = Change;
				holdCount = 0;
			}
			break;
		case Enter:
			sel_state = ResetChoice;
			break;
		case ResetChoice:
			if (endPoints) {
				endPoints = 0;
				choice = 1;
				sel_state = InitChoice;
			}
			break;
		default:
			break;
	}
	
	switch(sel_state) {
		case InitChoice:
			break;
		case Change:
			break;
		case Hold:
			++holdCount;
			break;
		case Enter:
			notePos = song1;
			endSong = end_song1;
			clearDisplay();
			break;
		case ResetChoice:
			break;
		default:
			break;
	}
	return sel_state;
}
// ----------------------------------------------------------------

#endif

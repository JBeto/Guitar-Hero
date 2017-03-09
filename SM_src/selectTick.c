#include "keypad.h"

//period = 250 ms

// ------------------ Global Variables ------------------
unsigned char songChoice = 0;
sound* notePos = 0; //note position
sound* endSong = 0; //index that song ends
// ------------------------------------------------------

// ---------------- Song Selection SM -------------------
enum songStates {Select, WaitSelect, EndSelect};

int selectTick(int selectState) {
	unsigned char press = GetKeypadKey();
	
	switch(selectState) {
		case Select:
			if (press == '*') { //TODO: pseudocode
				selectState = EndSelect;
			}
			else if (press == '1' || press == '2') { //increment song no.
				selectState = WaitSelect;
				if (songChoice < 2) {
					++songChoice;
				}
			}
			else if (press == 'A' || press == '3') { //decrement song no.
				selectState = WaitSelect;
				if (songChoice > 0) {
					--songChoice;
				}
			}
			break;
		case WaitSelect:
			if (press == '\0') { //button has been released
				selectState = Select;
			}
			break;
		case EndSelect:
			if (!notePos) { //song advancer has run out of notes, reset selection
				selectState = Select;
			}
			break;
		default:
			selectState = Select;
			break;
	}
	
	switch(selectState) {
		case Select:
			break;
		case WaitSelect:
			break;
		case EndSelect:
			switch(notePos) {
				case 0:
					notePos = song0; //TODO: change name of song
					endSong = &(song0 + endSong0);
					break;
				case 1:
					notePos = song1; //TODO: change name of song
					endSong = &(song1 + endSong1);
					break;
				case 2:
					notePos = song2; //TODO: change name of song
					endSong = &(song2 + endSong1);
					break;
				default:
					notePos = 0;
					endSong = 0;
					break;
			}
			break;
		default:
			notePos = 0;
			endSong = 0;
			break;
	}
	
	return selectState;
}
// ------------------------------------------------------
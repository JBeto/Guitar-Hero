#ifndef LED_TICK_C
#define LED_TICK_C

#include "Circular_Array.c"

// ----------- Dir Constants ---------------
const unsigned char LEFT = 0xFC;
const unsigned char RIGHT = 0x3F;
const unsigned char UP = 0xF3;
const unsigned char DOWN = 0xCF;
// ------------------------------------------

// --------------- Period -------------------
//unsigned char LED_period = 2;
// ------------------------------------------

// ------------- Global Variables ------------
unsigned char currDir = 0x00; //current direction of LED hitting floor
double PWM_freq = 0; //to be sent to playTick to play note
unsigned char endGame = 0; //bool for the end of the game
sound* prev = 0; //to keep track of when to add new notes
// ------------------------------------------

// --------------------- Helper Functions --------------------
void transmit_LED_row_data(unsigned char data) {
	int i;
	unsigned char temp_b;
	for (i = 0; i < 8 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTB = 0x08;
		// set SER = next bit of data to be sent.
		temp_b = 0x08 | ((data >> i) & 0x01);
		PORTB = temp_b;
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		temp_b |= 0x02;
		PORTB = temp_b;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	temp_b |= 0x04;
	PORTB = temp_b;
	// clears all lines in preparation of a new transmission
	PORTB = 0x00;
}

void transmit_LED_col_data(unsigned char data) {
	int i;
	unsigned char temp_d;
	for (i = 0; i < 8 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTD = 0x08;
		// set SER = next bit of data to be sent.
		temp_d = 0x08 |  ((data >> i) & 0x01);
		PORTD = temp_d;
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		temp_d |= 0x04;
		PORTD = temp_d;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	temp_d |= 0x02;
	PORTD = temp_d;
	// clears all lines in preparation of a new transmission
	PORTD = 0x00;
}

rectangle createRect() {
	double freq = notePos->note;
	unsigned char arrow = 0x00;
	unsigned char length = notePos->duration/50;
	
	if (prev) {
		if (prev->note > freq ) {
			if (notePos->duration < prev->duration) {
				arrow = RIGHT;
			}
			else {
				arrow = DOWN;
			}
		}
		else {
			if (notePos->duration < prev->duration) {
				arrow = LEFT;
			}
			else {
				arrow = UP;
			}
		}
	}
	else {
		arrow = DOWN;
	}
	
	return (rectangle){notePos->note, 0x00, arrow, length, 0};
}

void moveDown(rectangle* rect) {
	rect->row = rect->row << 1;
	if (rect->traveled < rect->length || rect->traveled == 0) {
		rect->row |= 0x01;
	}
	++(rect->traveled);
}

void updatePos(CircleArray* musicBlocks) {	
	for (rectangle* curr = musicBlocks->head; curr != musicBlocks->tail; curr = next(musicBlocks, curr)) {
		moveDown(curr);
	}
}
// -----------------------------------------------------------

// --------------------- LED Matrix ---------------------------

//0xF0 = bottom lit up
//0x0F = right side lit up

enum LED_States {WaitLED, UpdateLED, DrawNotes};

int LED_Tick(int note_light_state) {
	// ---------------- SM Variables --------------------
	static rectangle music[10];
	static CircleArray noteBlocks = {10, music, music + 10, music, music, 0};
	static rectangle* curr = 0;
	// --------------------------------------------------
	
	switch(note_light_state) {
		case WaitLED:
			if (notePos) {
				note_light_state = UpdateLED;
			}
			break;
		case UpdateLED:
			if (!notePos && isEmpty(&noteBlocks)) {
				endGame = 1;
				transmit_LED_col_data(0xFF);
				transmit_LED_row_data(0x00);
				PWM_freq = 0;
				note_light_state = WaitLED;
			}
			else {
				update = 0;
				curr = noteBlocks.head;
				note_light_state = DrawNotes;
			}
			break;
		case DrawNotes:
			if (update) {
				note_light_state = UpdateLED;
			}
			break;
		default:
			note_light_state = WaitLED;
			break;
	}
	
	switch(note_light_state) {
		case WaitLED:
			break;
		case UpdateLED:
			// Add notes
			if (prev != notePos) { //new note
				rectangle addedNote = createRect();
				push_back(&noteBlocks, &addedNote);
				prev = notePos;
			}
			//Delete note if it has disappeared
			if (noteBlocks.head->traveled >= 7 + noteBlocks.head->length) {
				pop_front(&noteBlocks);
			}
			// Check current note to see if it needs to set PWM
			if (noteBlocks.head->traveled >= 7) {
				currDir = noteBlocks.head->col;
				PWM_freq = noteBlocks.head->freq;
			}
			else {
				currDir = 0x00;
				PWM_freq = 0;
			}
		
			//Iterate through remaining notes
			updatePos(&noteBlocks);
			break;
		case DrawNotes:
			transmit_LED_col_data(curr->col);
			transmit_LED_row_data(curr->row);
			curr = next(&noteBlocks, curr);
			if (curr == noteBlocks.tail) {
				curr = noteBlocks.head;
			}
			break;
		default:
			break;
	}
	return note_light_state;
}

// -------------------------------------------------------------
#endif

//#include "PWM.h"
//period = 2 ms

// -------------------- Song Type ----------------------
//typedef struct sound {
//	double note; //frequency of note to be played
//	unsigned short duration; //duration for which note is held
//	unsigned short wait; //after duration ends, wait no. of ms before next note
//} sound;
// ------------------------------------------------------


// ------------------ Global Variables ------------------
//double PWM_freq = 0; //to determine which column in LED Matrix note is on
//sound* notePos = 0; //note position
//sound* endSong = 0; //index that song ends
// ------------------------------------------------------


// ------------------ Song Player SM --------------------
enum songStates {InitSong, PlayNote, HoldNote, WaitNote, ResetSong};

int songTick(int seqState) {
	static unsigned short durNote, gapNote;
	
	switch(seqState) {
		case InitSong:
			if (notePos) { //add another condition if synching block note fall
				seqState = PlayNote;
			}
			break;
		case PlayNote:
			if (notePos == endSong) {
				seqState = ResetSong;
			}
			else {
				seqState = HoldNote;
			}
			break;
		case HoldNote:
			if (durNote == notePos->duration) {
				durNote = 0; //reset durNote
				seqState = WaitNote;
			}
			break;
		case WaitNote:
			if (gapNote == notePos->wait) {
				seqState = PlayNote;
				gapNote = 0; //reset gapNote
				++notePos; //advance to next note
			}
			break;
		case ResetSong:
			seqState = InitSong;
			break;
		default:
			seqState = InitSong;
			break;
	}
	
	switch(seqState) {
		case InitSong: //PWM_freq should be 0 at this point already
			break;
		case PlayNote:
			PWM_freq = notePos->note;
			set_PWM(PWM_freq);
			break;
		case HoldNote:
			++durNote;
		case WaitNote:
			PWM_freq = 0;
			set_PWM(PWM_freq);
			++gapNote;
			break;
		case ResetSong:
			PWM_freq = 0; //may be unnecessary
			set_PWM(PWM_freq);
			notePos = 0;
			break;
		default:
			PWM_freq = 0;
			set_PWM(PWM_freq);
			notePos = 0;
			break;
	}
	
	return seqState;
}
// ------------------------------------------------------
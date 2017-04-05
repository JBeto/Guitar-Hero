#ifndef SONG_TICK_C
#define SONG_TICK_C

// -------------------- Song Type ----------------------
typedef struct sound {
	double note; //frequency of note to be played
	unsigned short duration; //duration for which note is held
	unsigned short wait; //after duration ends, wait no. of ms before next note
} sound;
// ------------------------------------------------------

// ------------------------ Period ---------------------
unsigned char song_period = 50;
// -----------------------------------------------------

// -------------------- Global Variables ---------------
sound* notePos = 0; //current note position, is 0 if song has ended or has not begun
sound* endSong = 0; //end of song
sound* song1 = 0;
sound* end_song1 = 0;
unsigned char update = 1; //tells LED_Tick to update display
// -----------------------------------------------------

// ------------------ Song Player SM --------------------
enum songStates {InitSong, PlayNote, HoldNote, WaitNote, ResetSong};

int songTick(int seqState) {
	static unsigned short durNote, gapNote;
	
	switch(seqState) {
		case InitSong:
			if (notePos) { //add another condition if syncing block note fall
				seqState = PlayNote;
			}
			break;
		case PlayNote:
			if (notePos == endSong) {
				seqState = ResetSong;
			}
			else {
				durNote = 0; //reset durNote
				seqState = HoldNote;
			}
			break;
		case HoldNote:
			if (durNote >= notePos->duration) {
				gapNote = 0; //reset gapNote
				seqState = WaitNote;
			}
			break;
		case WaitNote:
			if (gapNote >= notePos->wait) {
				seqState = PlayNote;
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
			update = 1;
			break;
		case HoldNote:
			update = 1;
			durNote += song_period;
			break;
		case WaitNote:
			update = 1;
			gapNote += song_period;
			break;
		case ResetSong:
			update = 1;
			notePos = 0;
			break;
		default:
			notePos = 0;
			break;
	}
	
	return seqState;
}

#endif

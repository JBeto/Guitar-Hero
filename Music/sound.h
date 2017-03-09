// -------------------- Song Type ----------------------
typedef struct sound {
	double note; //frequency of note to be played
	unsigned short duration; //duration for which note is held
	unsigned short wait; //after duration ends, wait no. of ms before next note
} sound;
// ------------------------------------------------------

//period = 100 ms

// ------------------ Global Variables ------------------
//bool endGame; //input
//unsigned char shiftMult //input
//signed short points; //input
//signed short score; //output
// ------------------------------------------------------


// ------------------- Score State SM -------------------
enum scoreStates {InitScore, ChangeScore, EndScore};

int scoreTick(int scoreState) {
	
	switch(scoreState) {
		case InitScore:
			score = 0;
			scoreState = ChangeScore;
			break;
		case ChangeScore:
			if (endGame) {
				scoreState = EndScore;
			}
			break:
		case EndScore:
			if (notePos) { //starts new song
				scoreState = InitScore;
			}
			break;
		default:
			scoreState = InitScore;
			break;
	}
	
	switch(scoreState) {
		case InitScore:
			break;
		case ChangeScore:
			score += points << shiftMult;
			break;
		case EndScore:
			break;
		default:
			score = 0;
			break;
	}
	
	return scoreState;
}
// ------------------------------------------------------
//period = 100 ms

// ------------------ Global Variables ------------------
//bool endGame; //input
//unsigned char shiftMult //input
//signed short points; //input
//signed short score; //output
// ------------------------------------------------------

// ------------------- Score State SM -------------------
enum LCD_Score_States {Init_LCD_Score, Display_Score, End_Score};

int LCD_Score_Tick(int LCD_Score_State) {
	
	switch(LCD_Score_State) {
        case Init_LCD_Score:
            break;
        case Display_Score:
            break;
        case End_Score:
            break;
        default:
            break;
	}
	
	switch(LCD_Score_State) {
	    case Init_LCD_Score:
            break;
        case Display_Score:
            break;
        case End_Score:
            break;
        default:
            break;
	}
	
	return LCD_Score_State;
}
// ------------------------------------------------------
//#include "keypad.h"
//#include <cstdlib>
//period = 250 ms

/*
LCD_ClearScreen()
LCD_init()
LCD_WriteCommand(unsigned char Command)
LCD_WriteData(unsigned char data)
LCD_DisplayString(unsigned char column, const unsigned char*)
LCD_Cursor(unsigned char column) */

// ------------------ Global Variables ------------------
//bool endGame; //input
//unsigned char shiftMult //input
//signed short points; //input
//signed short score; //output
// ------------------------------------------------------

// ------------------- Score State SM -------------------
enum LCD_Score_States {Init_LCD_Score, Display_Score, End_Score, End_Wait};

int LCD_Score_Tick(int LCD_Score_State) {
	static bool prevScore = false;
	
	switch(LCD_Score_State) {
        case Init_LCD_Score:
            if (endGame) {
                LCD_Score_State = Display_Score;
            }
            break;
        case Display_Score:
            if (GetKeypadKey() == '*') {
                LCD_Score_State = End_Score;
            }
            break;
        case End_Score:
            LCD_Score_State = End_Wait;
            break;
        case End_Wait:
            if (endGame) {
                LCD_Score_State = Init_LCD_Score
            }
            break;
        default:
            LCD_Score_State = Init_LCD_Score;
            break;
	}
	
	switch(LCD_Score_State) {
	    case Init_LCD_Score:
            break;
        case Display_Score:
            if (!prevScore) {
                //Prints string messages
                LCD_DisplayString(1, "Score: ");
                LCD_DisplayString(17, "Play: Press '*'");
                
                //converts score to a string and prints it
                char* sc = 0;
                itoa((int)score, sc, 10);
                unsigned char col = 8;
                while(*sc) {
                  LCD_Cursor(col++);
                  LCD_WriteData(*sc++);
                }
                
                //adds in 2 0s at the end to multiply score by 100
                LCD_WriteData('0');
                LCD_Cursor(++col);
                LCD_WriteData('0');
                
                //sets prevScore to true to avoid writing same message again
                prevScore = true;
            }
            break;
        case End_Score:
            endScore = true;
            break;
        case End_Wait:
            endScore = false;
            break;
        default:
            endScore = false;
            break;
	}
	
	return LCD_Score_State;
}
// ------------------------------------------------------
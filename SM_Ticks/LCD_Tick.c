//#include "io.c"
//period = 250 ms

/*
LCD_ClearScreen()
LCD_init()
LCD_WriteCommand(unsigned char Command)
LCD_WriteData(unsigned char data)
LCD_DisplayString(unsigned char column, const unsigned char*)
LCD_Cursor(unsigned char column) */

// ------------------ Global Variables ------------------
unsigned char songChoice = 1;
sound* notePos = 0;
bool endGame = false;
// ------------------------------------------------------

// ------------------ LCD Message SM --------------------
//period = 250 ms
enum LCD_States {InitLCD, PrintLCD, EndLCD};

int LCD_Tick(int LCD_State) {
	static unsigned char prevSong = 0;
	
	switch(LCD_State) {
		case InitLCD:
			LCD_State = PrintLCD;
			LCD_init();
			break;
		case PrintLCD:
			if (notePos) { //if song has been selected, go to EndLCD
				LCD_State = EndLCD;
				LCD_ClearScreen();
			}
			break;
		case EndLCD:
			if (endGame) {
				LCD_State = InitLCD;
			}
			break;
		default
			LCD_State = InitLCD;
			break;
	}
	
	switch(LCD_State) {
		case InitLCD:
			break;
		case PrintLCD:
			if (prevSong != songChoice) {
				switch(songChoice) {
					case 1:
						LCD_DisplayString(1, "Song 1") //TODO: Replace song with actual name
						break;
					case 2:
						LCD_DisplayString(1, "Song 2") //TODO: Replace song with actual name
						break;
					case 3:
						LCD_DisplayString(1, "Song 3") //TODO: Replace song with actual name
						break;
					default:
						LCD_DisplayString(1, "Song 1") //TODO: Replace song with actual name
						break;
				}
			}
			break;
		case EndLCD:
			break;
		default:
			break;
	}
	
	return LCD_State;
}
// ------------------------------------------------------
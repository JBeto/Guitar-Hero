#ifndef PRINT_C
#define PRINT_C

void clearDisplay() {
	PORTA &= 0x03;
	PORTD &= 0x9F;
}

void printChoice(unsigned char num) {
	switch(num) {
		case 1:
			PORTA &= 0x83;
			PORTA |= 0x80;
			break;
		case 2:
			PORTA &= 0xC3;
			PORTA |= 0xC0;
			break;
		case 3:
			PORTA &= 0xE3;
			PORTA |= 0xE0;
			break;
		case 4:
			PORTA &= 0xF3;
			PORTA |= 0xF0;
			break;
		case 5:
			//we technically don't need to consider PORTD
			PORTA &= 0xFB;
			PORTA |= 0xF8;
			break;
		case 6:
			PORTD &= 0x9F;
			PORTA |= 0xFC;
			break;
		case 7:
			PORTD &= 0xDF;
			PORTD |= 0x40;
			PORTA |= 0xFC;
			break;
			case 8:
			PORTD |= 0x60;
			PORTA |= 0xFC;
			break;
		default:
			break;
	}
}

void printScore(unsigned char points) {
	unsigned char lastTwo = (points & 0x03) << 5;
	unsigned char firstSix = points & 0xFC;
	PORTA |= firstSix;
	PORTD |= lastTwo;
}

#endif

#ifndef INPUTS_C
#define INPUTS_C

#include "bit.h"
#include "keypad.h"

// ------------------------------------------- ADC Joystick ------------------------------------
//I got the ADC code from: http://maxembedded.com/2011/06/the-adc-of-the-avr/

void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with ’7′ will always keep the value
	// of ‘ch’ between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	// write ’1′ to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0′ again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}
// ----------------------------------------------------------------------------------------------

unsigned char joystick_arrow() {
	uint16_t mid = 512;
	
	uint16_t vertArrow = adc_read(0);
	uint16_t horizArrow = adc_read(1);
	
	if (vertArrow < mid - 100) { //DOWN
		return DOWN;
	}
	else if (vertArrow > mid + 100){ //UP
		return UP;
	}
	else if (horizArrow < mid - 100) { //RIGHT
		return RIGHT;
	}
	else if (horizArrow > mid + 100) { //LEFT
		return LEFT;
	}
	else {
		return 0x00;
	}
}

unsigned char button_arrow() {
	unsigned char key = GetKeypadKey();
	
	switch(key) {
		case 'D':
			return LEFT;
			break;
		case '#':
			return UP;
			break;
		case '0':
			return DOWN;
			break;
		case '*':
			return RIGHT;
			break;
		default:
			return 0x00;
			break;
	}
}

#endif

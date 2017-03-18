/*
 * Beta_Guitar_Hero.c
 *
 * Created: 3/14/2017 11:04:58 PM
 *  Author: Joshua Beto
 */ 

#include <avr/io.h>
#include "timer.h"
#include "scheduler.h"

#include "songTick.c"
#include "LED_Tick.c"
#include "playTick.c"
#include "inputTick.c"
#include "selectTick.c"
#include "scoreTick.c"

int main(void)
{
	DDRA = 0xFC; PORTA = 0x03;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	
	// --------------------------------- SOS -----------------------------------
	//I got the Song of Storms melody from: https://forum.arduino.cc/index.php?topic=56637.0
	
	double NOTE_D5 = 587;
	double NOTE_F5 = 698;
	double NOTE_D6 = 1175;
	double NOTE_E6 = 1319;
	double NOTE_F6 = 1397;
	double NOTE_C6 = 1047;
	double NOTE_A5 = 880;
	double NOTE_G5 = 784;
	double NOTE_E5 = 659;
	
	sound song_of_storms[] = {	(sound){NOTE_D5, 150, 50}, (sound){NOTE_F5, 150, 50}, (sound){NOTE_D6, 300, 100}, //80
	(sound){NOTE_D5, 150, 50}, (sound){NOTE_F5, 150, 50}, (sound){NOTE_D6, 300, 100},

	(sound){NOTE_E6, 300, 100}, (sound){NOTE_F6, 100, 100}, (sound){NOTE_E6, 100, 100}, //100
	(sound){NOTE_F6, 100, 50}, (sound){NOTE_E6, 100, 100}, (sound){NOTE_C6, 100, 100},
	(sound){NOTE_A5, 200, 200},

	(sound){NOTE_A5, 200, 100}, (sound){NOTE_D5, 200, 100}, (sound){NOTE_F5, 100, 100}, //100
	(sound){NOTE_G5, 100, 100}, (sound){NOTE_A5, 350, 200},

	(sound){NOTE_A5, 300, 100}, (sound){NOTE_D5, 200, 100}, (sound){NOTE_F5, 100, 100},
	(sound){NOTE_G5, 100, 100}, (sound){NOTE_E5, 350, 200},

	(sound){NOTE_D5, 150, 50}, (sound){NOTE_F5, 150, 50}, (sound){NOTE_D6, 300, 100}, //80
	(sound){NOTE_D5, 150, 50}, (sound){NOTE_F5, 150, 50}, (sound){NOTE_D6, 300, 100},

	(sound){NOTE_E6, 300, 100}, (sound){NOTE_F6, 100, 100}, (sound){NOTE_E6, 100, 100}, //100
	(sound){NOTE_F6, 100, 50}, (sound){NOTE_E6, 100, 100}, (sound){NOTE_C6, 100, 100},
	(sound){NOTE_A5, 200, 200}, //300

	(sound){NOTE_A5, 200, 100}, (sound){NOTE_D5, 200, 100}, (sound){NOTE_F5, 100, 100},
	(sound){NOTE_G5, 100, 100}, (sound){NOTE_A5, 300, 100}, (sound){NOTE_A5, 200, 100},
	(sound){NOTE_D5, 300, 2000}
	};
	
	song1 = song_of_storms;
	end_song1 = song_of_storms + 43;
	// --------------------------------------------------------------------------------
	/*
	int state = WaitEnter;
	while(1) {
		unsigned char wow = GetKeypadKey();
		score = 50;
		printScore(3);
	}
	
	return 0; */
	
	// --------------------- Period for each tasks ----------------------------
	//unsigned short update_period = 50;
	//unsigned short displayPeriod = 100;
	//unsigned char sel_period = 100;
	//unsigned char LED_period = 2;
	//unsigned char input_period = 100;
	// ------------------------------------------------------------------------
	
	//Recalculate GCD periods for scheduler
	unsigned char SMTick1_period = 50;
	unsigned char SMTick2_period = 50;
	unsigned char SMTick3_period = 50;
	unsigned char SMTick4_period = 50;
	unsigned char SMTick5_period = 2;
	unsigned char SMTick6_period = 50;
	
	//Declare an array of tasks
	static task task1, task2, task3, task4, task5, task6;
	task *tasks[] = { &task1, &task2, &task3, &task4, &task5, &task6 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	// Task 1
	task1.state = InitSong;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &songTick;//Function pointer for the tick.

	// Task 2
	task2.state = PWM_init;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &playTick;//Function pointer for the tick.

	// Task 3
	task3.state = Wait;//Task initial state.
	task3.period = SMTick3_period;//Task Period.
	task3.elapsedTime = SMTick3_period; // Task current elapsed time.
	task3.TickFct = &scoreTick; // Function pointer for the tick.

	// Task 4
	task4.state = InitChoice;//Task initial state.
	task4.period = SMTick4_period;//Task Period.
	task4.elapsedTime = SMTick4_period; // Task current elapsed time.
	task4.TickFct = &selectTick; // Function pointer for the tick.
	
	// Task 5
	task5.state = WaitLED;//Task initial state.
	task5.period = SMTick5_period;//Task Period.
	task5.elapsedTime = SMTick5_period; // Task current elapsed time.
	task5.TickFct = &LED_Tick; // Function pointer for the tick.

	// Task 6
	task6.state = WaitScore;//Task initial state.
	task6.period = SMTick6_period;//Task Period.
	task6.elapsedTime = SMTick6_period; // Task current elapsed time.
	task6.TickFct = &inputTick; // Function pointer for the tick.

	// Set the timer and turn it on
	adc_init();
	TimerSet(1);
	TimerOn();
	
	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime >= tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	// Error: Program should not exit!
	return 0;
}

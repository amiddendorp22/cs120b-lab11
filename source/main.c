/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "avr/interrupt.h"
#include "timer.h"
#endif




//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------
enum Demo_States {shift};
int Demo_Tick(int state) {

	// Local Variables
	static unsigned char pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xFE;  	// Row(s) displaying pattern. 
							// 0: display pattern on row
							// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
		case shift:	
break;
		default:	
state = shift;
			break;
	}	
	// Actions
	switch (state) {
case shift:	
if (row == 0xEF && pattern == 0x01) { // Reset demo 
				pattern = 0x80;		    
				row = 0xFE;
			} else if (pattern == 0x01) { // Move LED to start of next row
				pattern = 0x80;
				row = (row << 1) | 0x01;
			} else { // Shift LED one spot to the right on current row
				pattern >>= 1;
			}
			break;
		default:
	break;
	}
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern	
	return state;
}

/*
unsigned char ballDirection = 0;  //0 is left, 1 is right
unsigned char ballPosition = 0x00;
enum Ball_States{ continueDirection, changeDirection};

int Ball_Tick(int state)
{
	switch(state)
	{
		case(continueDirection)
	}
}

*/

enum Player_States {moveLeft, moveRight, stay};

unsigned char paddleLocation = 0x1C;

int Player_Tick(int state)
{
	unsigned char tmpA = ~PINA & 0x03; //grabs first two pins of PINA for player input up/down
	
	switch(state)
	{
		case(moveLeft):
			if(tmpA == 0x00 || tmpA == 0x03)
			{
				state = stay;
			}
			else if(tmpA == 0x01)
			{
				state = moveRight;
			}
			else if(tmpA == 0x02)
			{
				state = moveLeft;
			}
			break;
		case(moveRight):
			if(tmpA == 0x00 || tmpA == 0x03)
                        {
                                state = stay;
                        }
			else if(tmpA == 0x01)
                        {
                                state = moveRight;
                        }
			else if(tmpA == 0x02)
                        {
                                state = moveLeft;
                        }
			break;
		case(stay):
			if(tmpA == 0x00 || tmpA == 0x03)
                        {
                                state = stay;
                        }
			else if(tmpA == 0x01)
                        {
                                state = moveRight;
                        }
			else if(tmpA == 0x02)
                        {
                                state = moveLeft;
                        }
			break;
		default:
			state = stay;
			break;
	}
	switch(state)
	{
		case(moveLeft):
			if(paddleLocation == 0xE0)
			{
				//Do nothing
			}
			else
			{
				paddleLocation = paddleLocation << 1;
			}
			break;
		case(moveRight):
			if(paddleLocation == 0x07)
			{
				//do nothing
			}
			else
			{
				paddleLocation = paddleLocation >> 1;
			}
			break;
		case(stay):
			break;
	}

	return state;
}


enum Display_States {display_display};



int Display_Tick(int state)
{
	switch(state)
	{
		case(display_display):
			state = display_display;
			break;
		default:
			state = display_display;
			break;
	}

	switch(state)
	{
		case(display_display):
			PORTC = paddleLocation;
			PORTD = ~(0x10);
			break;
	}

	return state;
}

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1)
	{
		c = a%b;
		if(c==0) {return b;}
		a = b;
		b = c;
	}

	return 0;
}


typedef struct _task
{
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
}task;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */

	static task task1, task2;
	task *tasks[] = {&task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	task1.state = start;
	task1.period = 200;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Player_Tick;

	task2.state = start;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.TickFct = &Display_Tick;

	unsigned long GCD = tasks[0]->period;

	unsigned short i;

	for(i = 0; i < numTasks; i++)
	{
		GCD = findGCD(GCD,tasks[i]->period);
	}

	TimerSet(GCD);
	TimerOn();

	unsigned char tmpA;

    while (1)
    {
	for(i = 0; i < numTasks; i++)
	{
		if(tasks[i]->elapsedTime == tasks[i]->period)
		{
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += GCD;
	}

	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}

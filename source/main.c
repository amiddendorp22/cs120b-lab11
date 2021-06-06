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


unsigned char ballXDirection = 0;  //0 is left, 1 is right
unsigned char ballYDirection = 0; //0 is decreasing, 1 is increasing
unsigned char ballXPosition = 0x10;
unsigned char ballYPosition = 0x00;
unsigned char ballNextPosition = 0x00;

/*
enum Ball_States{ continueDirection, changeDirection};

int Ball_Tick(int state)
{
	unsigned char tmpC = PORTC;
	switch(state)
	{
		case(continueDirection):
			if((tmpC & ballNextPosition) == ballNextPosition) //if the location where the ball should go next is lit up
			{
				state = changeDirection;
			}
			else
			{
				state = continueDirection;
			}
			break;
		case(changeDirection):
			state = continueDirection; //continues in the new direction
			break;
		default:
			state = continueDirection;
			break;
	}

	switch(state)
	{
		case(continueDirection):
			ballXPosition = ballNextPosition
			if(ballYPosition == 0)
			{
				ballYPosition--;
			}
			else
			{
				ballYPosition++;
			}
			if(ballDirection == 0)
			{
				ballNextPosition = ballNextPosition << 1;
			}
			else
			{
				ballNextPosition = ballNextPosition >> 1;
			}
			break;
		case(changeDirection):
			if(ballYPosition == 0)
			{
				ballYPosition = 1;
			}
			else
			{
				ballYPosition = 0;
			}
			if(ballDirection == 0)
			{
				ballDirection = 1;
			}
			else
			{
				ballDirection = 0;
			}
			break;

	}


	return state;
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


enum AI_States {AI_moveLeft, AI_moveRight, AI_stay};

unsigned char AI_Pos = 0x38; //same as player
unsigned char AI_Top_Pos = 0x08;
unsigned char AI_Bot_Pos = 0x20;
int AI_Tick(int state)
{
	switch(state)
	{
		case(AI_moveLeft):
			if(ballXPosition < AI_Top_Pos)
			{
				state = AI_moveRight;
			}
			else if (ballXPosition > AI_Top_Pos)
			{
				state = AI_moveLeft;
			}
			else
			{
				state = AI_stay;
			}
			break;
		case(AI_moveRight):
			if(ballXPosition < AI_Top_Pos)
                        {
                                state = AI_moveRight;
                        }
                        else if (ballXPosition > AI_Top_Pos)
                        {
                                state = AI_moveLeft;
                        }
                        else
                        {
                                state = AI_stay;
                        }
                        break;
		case(AI_stay):
			if(ballXPosition < AI_Top_Pos)
                        {
                                state = AI_moveRight;
                        }
                        else if (ballXPosition > AI_Top_Pos)
                        {
                                state = AI_moveLeft;
                        }
                        else
                        {
                                state = AI_stay;
                        }
                        break;
		default:
			state = AI_stay;
	}

	switch(state)
	{
		case(AI_moveLeft):
			if(AI_Pos == 0xE0)
			{
				//Do nothing
			}
			else
			{
				AI_Pos = AI_Pos << 1;
				AI_Top_Pos = AI_Top_Pos << 1;
				AI_Bot_Pos = AI_Bot_Pos << 1;
			}
			break;
		case(AI_moveRight):
			if(AI_Pos == 0x07)
			{
				//Do nothing
			}
			else
			{
				AI_Pos = AI_Pos >> 1;
				AI_Top_Pos = AI_Top_Pos >> 1;
				AI_Bot_Pos = AI_Bot_Pos >> 1;
			}
			break;
		case(AI_stay):
			break;
	}

	return state;
}

enum Display_States {display_player, display_opponent};



int Display_Tick(int state)
{
	switch(state)
	{
		case(display_player):
			state = display_opponent;
			break;
		case(display_opponent):
			state = display_player;
			break;
		default:
			state = display_player;
			break;
	}

	switch(state)
	{
		case(display_player):
			PORTC = paddleLocation;
			PORTD = ~(0x10);
			break;
		case(display_opponent):
			PORTC = AI_Pos;
			PORTD = ~(0x01);
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

	
	static task task1, task2, task3;
	task *tasks[] = {&task1, &task2, &task3};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	task1.state = start;
	task1.period = 200;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Player_Tick;

	task2.state = start;
	task2.period = 10;
	task2.elapsedTime = task2.period;
	task2.TickFct = &Display_Tick;

	task3.state = start;
	task3.period = 200;
	task3.elapsedTime = task3.period;
	task3.TickFct = &AI_Tick;

	unsigned long GCD = tasks[0]->period;

	unsigned short i;

	for(i = 0; i < numTasks; i++)
	{
		GCD = findGCD(GCD,tasks[i]->period);
	}

	TimerSet(GCD); //used to be GCD
	TimerOn();

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

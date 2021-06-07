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
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#endif

/////////////////////////////global variables

//ball variables
char ballXDirection = 1;  //-1 is left, 1 is right
unsigned char ballYDirection = 1; //0 is decreasing, 1 is increasing
unsigned char ballXPosition = 0x10;
unsigned char ballYPosition = 0x04;
unsigned char ballNextPosition = 0x10; //starts going down


//player variables
unsigned char paddleLocation = 0x1C;
unsigned char paddle_top = 0x04;
unsigned char paddle_bot = 0x10;
//AI variables
unsigned char AI_Pos = 0x38; //same as player
unsigned char AI_Top_Pos = 0x08;
unsigned char AI_Bot_Pos = 0x20;
//time_t t;
//srand(time(0));

//unsigned short x = ADC;

void ADC_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}
//helper function


int random_number(int min, int max)
{
	int difference = (max - min);
	return (((int)(difference+1)/RAND_MAX) * rand() + min);
}

enum Ball_States{ continueDirection, changeDirection, changeDirectionCorner, topBounce, botBounce, reset};

int Ball_Tick(int state)
{
	unsigned char resetButton = ~PINA & 0x08;
	switch(state)
	{
		case(continueDirection):
			if(resetButton == 0x08)
			{
				state = reset;
			}
			else if(ballYDirection == 1) //if the Y-coordinate of the ball is increasing
			{
				if((ballYPosition << 1) == 0x10) // if the ball is entering the row that contains a paddle (used to be opposite)
				{
					if((paddle_top & ballNextPosition) == ballNextPosition) //if the ball will collide with a paddle
					{
						state = changeDirectionCorner;
					}
					else if((paddle_bot & ballNextPosition) == ballNextPosition)
					{
						state = changeDirectionCorner;
					}
					else if((paddleLocation & ballNextPosition) == ballNextPosition)
					{
						state = changeDirection;
					}
					else
					{
						state = reset;
					}
				}
				else if(ballXPosition == 0x01) //if the ball is about to go off the top of the map
				{
					state = topBounce;
				}
				else if(ballXPosition == 0x80) //was ballNextPosition
				{
					state = botBounce;
				}
				else
				{
					state = continueDirection;
				}
			}
			else
			{
				if((ballYPosition >> 1) == 0x01) // if the ball is entering the row that contains a paddle (used to be opposite)
				{
					if((AI_Top_Pos & ballNextPosition) == ballNextPosition) //if the ball will collide with a paddle
					{
						state = changeDirectionCorner;
			//			AI_Pos = 0xE0;
					}
					else if((AI_Bot_Pos & ballNextPosition) == ballNextPosition)
					{
						state = changeDirectionCorner;
			//			AI_Pos = 0xE0;
					}
					else if((AI_Pos & ballNextPosition) == ballNextPosition)
					{
						state = changeDirection;
					}
					else
					{
						state = reset;
					}
				}
				else if(ballXPosition == 0x01)
				{
					state = topBounce;
				}
				else if(ballXPosition == 0x80)
				{
					state = botBounce;
				}
				else
				{
					state = continueDirection;
				}
			}
			break;
		case(changeDirection):
			if(resetButton == 0x08)
                        {
                                state = reset;
                        }

			if(ballXPosition == 0x01)
			{
				state = topBounce;
			}
			else if(ballXPosition == 0x80)
			{
				state = botBounce;
			}
			else
			{
				state = continueDirection;
			}
			break;
		case(reset):
			state = continueDirection;
			break;
		case(changeDirectionCorner):
			if(resetButton == 0x08)
                        {
                                state = reset;
                        }

			if(ballXPosition == 0x01)
                        {
                                state = topBounce;
                        }
                        else if(ballXPosition == 0x80)
                        {
                                state = botBounce;
                        }
                        else
                        {
                                state = continueDirection;
                        }

			break;
		case(botBounce):
			if(resetButton == 0x08)
                        {
                                state = reset;
                        }

			if(ballYDirection == 1) //if the Y-coordinate of the ball is increasing
			{
				if((ballYPosition << 1) == 0x10) // if the ball is entering the row that contains a paddle (used to be opposite)
				{
					if((paddle_top & ballNextPosition) == ballNextPosition) //if the ball will collide with a paddle
					{
						state = changeDirectionCorner;
					}
					else if((paddle_bot & ballNextPosition) == ballNextPosition)
					{
						state = changeDirectionCorner;
					}
					else if((paddleLocation & ballNextPosition) == ballNextPosition)
					{
						state = changeDirection;
					}
					else
					{
						state = reset;
					}
				}
				else if(ballXPosition == 0x01) //if the ball is about to go off the top of the map
				{
					state = topBounce;
				}
				else if(ballXPosition == 0x80) //was ballNextPosition
				{
					state = botBounce;
				}
				else
				{
					state = continueDirection;
				}
			}
			else
			{
				if((ballYPosition >> 1) == 0x01) // if the ball is entering the row that contains a paddle (used to be opposite)
				{
					if((AI_Top_Pos & ballNextPosition) == ballNextPosition) //if the ball will collide with a paddle
					{
						state = changeDirectionCorner;
					}
					else if((AI_Bot_Pos & ballNextPosition) == ballNextPosition)
					{
						state = changeDirectionCorner;
					}
					else if((AI_Pos & ballNextPosition) == ballNextPosition)
					{
						state = changeDirection;
					}
					else
					{
						state = reset;
					}
				}
				else if(ballXPosition == 0x01)
				{
					state = topBounce;
				}
				else if(ballXPosition == 0x80)
				{
					state = botBounce;
				}
				else
				{
					state = continueDirection;
				}
			}
			break;
		case(topBounce):
			if(resetButton == 0x08)
                        {
                                state = reset;
                        }

			if(ballYDirection == 1) //if the Y-coordinate of the ball is increasing
			{
				if((ballYPosition << 1) == 0x10) // if the ball is entering the row that contains a paddle (used to be opposite)
				{
					if((paddle_top & ballNextPosition) == ballNextPosition) //if the ball will collide with a paddle
					{
						state = changeDirectionCorner;
					//	AI_Pos = 0xE0;
					}
					else if((paddle_bot & ballNextPosition) == ballNextPosition)
					{
						state = changeDirectionCorner;
					//	AI_Pos = 0xE0;
					}
					else if((paddleLocation & ballNextPosition) == ballNextPosition)
					{
						state = changeDirection;
					//	AI_Pos = 0xE0;
					}
					else
					{
						state = reset;
					}
				}
				else if(ballXPosition == 0x01) //if the ball is about to go off the top of the map
				{
					state = topBounce;
				}
				else if(ballXPosition == 0x80) //was ballNextPosition
				{
					state = botBounce;
				}
				else
				{
					state = continueDirection;
				}
			}
			else
			{
				if((ballYPosition >> 1) == 0x01) // if the ball is entering the row that contains a paddle (used to be opposite)
				{
					if((AI_Top_Pos & ballNextPosition) == ballNextPosition) //if the ball will collide with a paddle
					{
						state = changeDirectionCorner;
					}
					else if((AI_Bot_Pos & ballNextPosition) == ballNextPosition)
					{
						state = changeDirectionCorner;
					}
					else if((AI_Pos & ballNextPosition) == ballNextPosition)
					{
						state = changeDirection;
					}
					else
					{
						state = reset;
					}
				}
				else if(ballXPosition == 0x01)
				{
					state = topBounce;
				}
				else if(ballXPosition == 0x80)
				{
					state = botBounce;
				}
				else
				{
					state = continueDirection;
				}
			}
			break;
		default:
			state = continueDirection;
			break;
	}

	switch(state)
	{
		case(continueDirection):
			//ballXPosition = ballNextPosition;
			//AI_Pos = 0xE0;
			if(ballYDirection == 0)
			{
				ballYPosition = ballYPosition >> 1; //used to be << 1
			}
			else
			{
				ballYPosition = ballYPosition << 1; //used to be >> 1
			}
			if(ballXDirection == -1)
			{
			//	AI_Pos = 0xE0;
				ballNextPosition = ballNextPosition << 1; //inverted this in last change
			}
			else
			{
			//	AI_Pos = 0xE0;
				ballNextPosition = ballNextPosition >> 1;
			}
			ballXPosition = ballNextPosition;
			break;
		case(changeDirection):
			if(ballYDirection == 0)
			{
				ballYDirection = 1;
				ballYPosition = ballYPosition << 1;
			}
			else
			{
				ballYDirection = 0;
				ballYPosition = ballYPosition >> 1;
			}
			if(ballXDirection == -1)
			{
				ballNextPosition = ballNextPosition << 1;
			}
			else
			{
				ballNextPosition = ballNextPosition >> 1;
			}
			ballXPosition = ballNextPosition;
			break;
		case(changeDirectionCorner):
			if(ballYDirection == 0)
			{
				ballYDirection = 1;
				ballYPosition = ballYPosition << 1;
			}
			else
			{
				ballYDirection = 0;
				ballYPosition =  ballYPosition >> 1;
			}
			if(ballXDirection == 1)
			{
				ballXDirection = -1; //does this change? does it change back somehow?
				//AI_Pos = 0xE0;
				ballNextPosition = ballNextPosition << 1;
			}
			else
			{
				ballXDirection = 1;
				ballNextPosition = ballNextPosition >> 1;
			}
			ballXPosition = ballNextPosition;
	//		AI_Pos = 0xE0;
			break;
		case(topBounce):
			ballXDirection = -1;
			ballNextPosition = ballNextPosition << 1;
			ballXPosition = ballNextPosition;
			if(ballYDirection == 0)
			{
				ballYPosition = ballYPosition >> 1;
			}
			else
			{
				ballYPosition = ballYPosition << 1;
			}
		//	AI_Pos = 0xE0;
			break;
		case(botBounce):
			ballXDirection = 1;
			ballNextPosition = ballNextPosition >> 1;
			ballXPosition = ballNextPosition;
			if(ballYDirection == 0)
			{
				ballYPosition = ballYPosition >> 1;
			}
			else
			{
				ballYPosition = ballYPosition << 1;
			}
			//AI_Pos = 0xE0;
			break;
		case(reset):
			ballXPosition = 0x10;
			ballYPosition = 0x04;
			ballNextPosition = 0x10; //resets back to going down
			ballXDirection = 1;
			ballYDirection = 1;

			paddleLocation = 0x1C;
			paddle_top = 0x04;
			paddle_bot = 0x10;

			AI_Pos = 0x38;
			AI_Top_Pos = 0x08;
			AI_Bot_Pos = 0x20;
			break;
	}


	return state;
}


enum Player_States {moveLeft, moveRight, stay};

int Player_Tick(int state)
{
	unsigned char tmpA = ~PINA & 0x06; 
	
	switch(state)
	{
		case(moveLeft):
			if(tmpA == 0x00 || tmpA == 0x06)
			{
				state = stay;
			}
			else if(tmpA == 0x02)
			{
				state = moveRight;
			}
			else if(tmpA == 0x04)
			{
				state = moveLeft;
			}
			break;
		case(moveRight):
			if(tmpA == 0x00 || tmpA == 0x06)
                        {
                                state = stay;
                        }
			else if(tmpA == 0x02)
                        {
                                state = moveRight;
                        }
			else if(tmpA == 0x04)
                        {
                                state = moveLeft;
                        }
			break;
		case(stay):
			if(tmpA == 0x00 || tmpA == 0x06)
                        {
                                state = stay;
                        }
			else if(tmpA == 0x02)
                        {
                                state = moveRight;
                        }
			else if(tmpA == 0x04)
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
				paddle_top = paddle_top << 1;
				paddle_bot = paddle_bot << 1;
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
				paddle_top = paddle_top >> 1;
				paddle_bot = paddle_bot >> 1;
			}
			break;
		case(stay):
			break;
	}

	return state;
}


enum AI_States {AI_moveLeft, AI_moveRight, AI_stay};

int AI_Tick(int state)
{
	int randomNumber = rand()%100; //generates a random number from 0 to 99
	int inaccuracy = 70;
	switch(state)
	{
		case(AI_moveLeft):
			if(ballXPosition < AI_Top_Pos)
			{
				if(randomNumber > inaccuracy)
				{
					state = AI_moveRight;
				}
				else
				{
					state = AI_stay;
				}
			}
			else if (ballXPosition > AI_Top_Pos)
			{
				if(randomNumber > inaccuracy)
				{
					state = AI_moveLeft;
				}
				else
				{
					state = AI_stay;
				}
			}
			else
			{
				state = AI_stay;
			}
			break;
		case(AI_moveRight):
			if(ballXPosition < AI_Top_Pos)
                        {
				if(randomNumber > inaccuracy)
				{
					state = AI_moveRight;
				}
				else
				{
					state = AI_stay;
				}
                        }
                        else if (ballXPosition > AI_Top_Pos)
                        {
				if(randomNumber > inaccuracy)
				{
					state = AI_moveLeft;
				}
				else
				{
					state = AI_stay;
				}
                        }
                        else
                        {
                                state = AI_stay;
                        }
                        break;
		case(AI_stay):
			if(ballXPosition < AI_Top_Pos)
                        {
				if(randomNumber > inaccuracy)
				{
					state = AI_moveRight;
				}
				else
				{
					state = AI_stay;
				}
                        }
                        else if (ballXPosition > AI_Top_Pos)
                        {
				if(randomNumber > inaccuracy)
				{
					state = AI_moveLeft;
				}
				else
				{
					state = AI_stay;
				}
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

enum Display_States {display_player, display_opponent, display_ball};



int Display_Tick(int state)
{
	switch(state)
	{
		case(display_player):
			state = display_opponent;
			break;
		case(display_opponent):
			state = display_ball;
			break;
		case(display_ball):
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
		case(display_ball):
			PORTC = ballXPosition;
			PORTD = ~(ballYPosition);
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
	static task task1, task2, task3, task4;
	task *tasks[] = {&task1, &task2, &task3, &task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	task1.state = start;
	task1.period = 200;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Player_Tick;

	task2.state = start;
	task2.period = 5;
	task2.elapsedTime = task2.period;
	task2.TickFct = &Display_Tick;

	task3.state = start;
	task3.period = 200;
	task3.elapsedTime = task3.period;
	task3.TickFct = &AI_Tick;

	task4.state = start;
	task4.period = 500;
	task4.elapsedTime = task4.period;
	task4.TickFct = &Ball_Tick;

	unsigned long GCD = tasks[0]->period;

	unsigned short i;

	for(i = 0; i < numTasks; i++)
	{
		GCD = findGCD(GCD,tasks[i]->period);
	}

	TimerSet(GCD); //used to be GCD
	TimerOn();

	uint16_t u_rand_val = 0;
	uint16_t u_seed_rand_val = 0;
	ADC_init();

	for(uint8_t i = 0; i < 16; i++)
	{
		u_seed_rand_val = u_seed_rand_val << 1 | (ADC & 0b1);
	}
	unsigned int seed = ADC;
	unsigned char pattern = ADC;
	srand(u_seed_rand_val);
    while (1)
    {
//	unsigned int randomNumber = rand()%100;
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

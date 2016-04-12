/*	jgome043_lab4_part3.c - 4/11/2016 2:44:41 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #4  Exercise #3
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

enum States { START, INIT, WAIT, INC_LEFT, INC_RIGHT } state;      //state vars
unsigned char tmpC;                     //temp var for PORTC manip
unsigned char cnt;                      //count var to keep track of button presses
unsigned char left;                     //keep track of left state
unsigned char inputA;                   //temp var to keep track of PINA

void Tick() {
    inputA = ~PINA & 0x01;              //get state of PINA bits
    //State Transitions
    switch (state) {
        case START:
            state = INIT; break;        //transition to INIT state
        case INIT:
            if (!inputA) {
                state = WAIT;        //transition to WAIT state
            }
            break;
        case WAIT:
            //check for left transition flag
            //less than 6 presses so far
            //and that PA0 is set
            if (left && (cnt < 6) && inputA) {
                state = INC_LEFT;   //move state to INC_LEFT state
                tmpC = tmpC  | (0x01  << cnt);  //set pin shifted left to total presses so far
                cnt++;                  //increment press counter
            }
            //check for cleared left transition flag
            //less than 6 presses so far
            //and that PA0 is set
            else if (!left && (cnt < 6) && inputA) {
                state = INC_RIGHT;  //move state to INC_RIGHT state
                tmpC = tmpC | (0x20  >> cnt);  //set pin shifted right to total presses so far
                cnt++;                  //increment press counter
            }
            //check for more than 6 presses so far
            //and that PA0 is set
            else if (inputA && (cnt >= 6)) {
                left = left ? 0 : 1;    //set or clear left flag to opposite of current state
                state = INIT;   //return to INIT state
            }
            break;
        case INC_LEFT:
            //check that PA0 is cleared
            if (!inputA) {
                state = WAIT;   //return to WAIT state
            }
            break;
        case INC_RIGHT:
            //check that PA0 is cleared
            if (!inputA) {
                state = WAIT;   //return to WAIT state
            }
            break;
        default:
            state = START;              //error likely happened, restart
            break;
    }

    PORTC = tmpC;                       //set PORTC

    //State Actions
    switch (state) {
        case INIT:
            tmpC = 0;                       //reset temp var
            cnt = 0;                        //reset cnt
            PORTC = tmpC; break;            //set PORTC
        default:
            break;
    }
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	//set PORTA to inputs
	DDRC = 0xFF; PORTC = 0x00;	//set PORTC to outputs

    tmpC = 0;                   //initialize temp var
    left = 0;                   //disable left incrementation
    state = START;              //initialize state

	while(1)
	{
		Tick();               //step through states
	}
}

/*	jgome043_lab5_part1.c - 4/12/2016 10:10:38 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #5  Exercise #1
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include <cs120b/timer.h>

enum States { START, INIT, WAIT } state;      // state vars
unsigned char tmpB;                     // temp var for PORTB manip

void Tick() {
    // State Transitions
    switch (state) {
        case START:
            state = INIT; break;        // transition to INIT state
        case INIT:
            state = WAIT; break;        // transition to WAIT state
        case WAIT:
            state = (tmpB == 0x04) ? INIT : WAIT; break;    // check if last LED lit
                                        // if lit, return to initial state
        default:
            state = START;              // error likely happened, restart
            break;
    }

    // State Actions
    switch (state) {
        case INIT:
            tmpB = (tmpB & 0x00) | 0x01; break;     // set tmpB to bit0=1
        case WAIT:
            tmpB = tmpB << 1; break;    // shift LED over
        default:
            break;
    }

    PORTB = tmpB;                       // set PORTB
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;	// set PORTB to outputs

    TimerSet(1000);             // set Timer period
    TimerOn();                  // enable timer

    tmpB |= 0x01;               // initialize temp var
    state = START;              // initialize state
    
	while(1)
	{
		Tick();                 // step through states
        while (!TimerFlag);     // Wait 1 sec
        TimerFlag = 0;          // reset timer flag
	}
}

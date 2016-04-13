/*	jgome043_lab5_part2.c - 4/12/2016 11:14:50 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #5  Exercise #2
 *	Exercise Description:
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include "timer.h"

enum States { START, INIT, PLAY, PAUSE } state;      // state vars
unsigned char tmpB;                     // temp var for PORTB manip
unsigned char rev;                      // flag to determine direction
unsigned char btn;                      // var to store button state
unsigned char wait;                     // flag to help handle pause
unsigned char cnt;                      // counter for LED timing

void Tick() {
    
    btn = ~PINA & 0x01;

    // State Transitions
    switch (state) {
        case START:
            state = INIT; break;        // transition to INIT state
        case INIT:
            rev = 0;                    // initialize rev flag
            wait = 0;                   // initialize wait flag
            state = PLAY; break;        // transition to PLAY state
        case PLAY:
            // check for btn press
            if (btn) {
                state = PAUSE;
                wait = 1;
                cnt = 0;                // reset counter
            }
            cnt++;                      // increment counter
            break; 
        case PAUSE:
            // check for button and wait flag
            if (!btn && wait) {
                wait = 0;   //clear wait flag
            }
            // check for button and wait flag not set
            else if (btn && !wait) {
                state = PLAY;           //transition to PLAY state
            }
            break;
        default:
            state = START;              // error likely happened, restart
            break;
    }

    // State Actions
    switch (state) {
        case INIT:
            tmpB = (tmpB & 0x00) | 0x01; break;    // initialize temp var
        case PLAY:
            // check that counter is on even number (2x period = 300ms)
            if (cnt % 2) {
                tmpB = (!rev) ? (tmpB << 1) : (tmpB >> 1);  // illum next LED
            }
            // check if LED is at low edge
            if (tmpB <= 0x01 && rev) {
                rev = 0;    // return to normal direction
            }
            // check if LED is at high edge
            else if (tmpB >= 0x04 && !rev) {
                rev = 1;    // reverse direction
            }
        default:
            break;
    }

    PORTB = tmpB;               // set PORTB
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// set PORTA to inputs and initialize
	DDRB = 0xFF; PORTB = 0x00;	// set PORTB to outputs and initialize

    TimerSet(150);             // set Timer period
    TimerOn();                  // enable Timer

    state = START;              // initialize state

	while(1)
	{
		Tick();                 // step through states
        while (!TimerFlag);     // Wait for a period
        TimerFlag = 0;          // reset the timer flag
	}
}

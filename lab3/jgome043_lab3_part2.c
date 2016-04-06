/*	jgome043_lab3_part2.c - 4/5/2016 10:44:30 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #2
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

enum States { START, INIT, WAIT, INCR, DECR } state;    //state vars
unsigned char tmpC;                                 //temp var for PORTC manip

void Tick() {
    //State Transitions
    switch (state) {
        case START:
            state = INIT; break;    //transition to INIT state
        case INIT:
            state = WAIT;           //transition to WAIT state
            tmpC = 0; 
            PORTC = tmpC; break;
        case WAIT:
            //both A0 and A1 are pressed
            if ((PINA & 0x01) && (PINA & 0x02)) {
                state = INIT;       //transition to INIT state
            }
            //A0 is pressed, A1 is not pressed
            else if ((PINA & 0x01) && !(PINA & 0x02)) {
                state = INCR;       //transition to INCR state
                tmpC = (tmpC < 9) ? tmpC + 1: tmpC;     //check if tmpC has reached bound
                                                        //if not, increment by 1
                PORTC = tmpC;                           //set PORTC
            }
            //A0 is not pressed, A1 is pressed
            else if (!(PINA & 0x01) && (PINA & 0x02)) {
                state = DECR;       //transition to DECR state
                tmpC = (tmpC > 0) ? tmpC - 1: tmpC;     //check if tmpC has reached bound
                                                        //if not, decrement by 1
                PORTC = tmpC;                           //set PORTC
            }
            break;
        case INCR:
            //both A0 and A1 are pressed
            if ((PINA & 0x01) && (PINA & 0x02)) {
                state = INIT;       //transition to INIT
            }
            //A0 is not pressed, A1 is pressed
            else if (!(PINA & 0x01) && (PINA & 0x02)) {
                state = DECR;       //transition to DECR
                tmpC = (tmpC > 0) ? tmpC - 1: tmpC;     //check if tmpC has reached bound
                                                        //if not, decrement by 1
                PORTC = tmpC;                           //set PORTC
            }
            //neither A0 nor A1 are pressed
            else if (!(PINA & 0x01) && !(PINA & 0x02)) {
                state = WAIT;       //transition to WAIT state
            }
            break;
        case DECR:
            //both A0 and A1 are pressed
            if ((PINA & 0x01) && (PINA & 0x02)) {
                state = INIT;       //transition to INIT state
            }
            //A0 is pressed, A1 is not pressed
            else if ((PINA & 0x01) && !(PINA & 0x02)) {
                state = INCR;       //transition to INCR state
                tmpC = (tmpC < 9) ? tmpC + 1: tmpC;     //check if tmpC has reached bound
                                                        //if not, increment by 1
                PORTC = tmpC;                           //set PORTC
            }
            //neither A0 nor A1 are pressed
            else if (!(PINA & 0x01) && !(PINA & 0x02)) {
                state = WAIT;       //transition to WAIT state
            }
            break;
        default: 
            state = START;                      //error likely happened, restart
            break;
    }

    //State Actions
    switch (state) {
        case INIT:
            tmpC = 0;                           //reset temp var
            PORTC = tmpC; break;                //set PORTC
        default:
            break;
    }
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	//set PORTA to inputs
	DDRC = 0xFF; PORTC = 0x00;	//set PORTC to outputs

    tmpC = 0;                               //initialize tmp var
    state = START;                          //set initial state

	while(1)
	{
		Tick();
	}
}

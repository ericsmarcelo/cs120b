/*	jgome043_lab3_part1.c - 4/4/2016 2:04:39 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #1
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
//initialize state vars
enum States { START, INIT, WAIT1, PRESSED1, WAIT2, PRESSED2 } state;
unsigned char tmpB;                 //temp var for PORTB assignment

void Tick() {

    //State Transitions
    switch (state) {
        case START:
            state = INIT; break;            //transition to INIT state
        case INIT:
            state = WAIT1; break;           //transition to WAIT1 state
        case WAIT1:
            //transition to PRESSED1 state if PINA0
            state = (PINA & 0x01) ? PRESSED1 : state; break;
        case PRESSED1:
            //transition to WAIT2 state if !PINA0
            state = (!(PINA & 0x01)) ? WAIT2 : state; break;
        case WAIT2:
            //transition to PRESSED2 state if PINA0
            state = (PINA & 0x01) ? PRESSED2 : state; break;
        case PRESSED2:
            //transition to WAIT1 state if !PINA0
            state = (!(PINA & 0x01)) ? WAIT1 : state; break;
        default:
            state = START;              //assume error, return to START state
            break;
    }

    //State Actions
    switch (state) {
        case START: break;
        case INIT:
            tmpB = (tmpB & 0x00) | 0x01;    //clear bits and set bit 0
            PORTB = tmpB;                   //assign to PORTB
            break;
        case WAIT1: break;
        case PRESSED1:
            tmpB = (tmpB & 0x00) | 0x02;    //clear bits and set bit 1
            PORTB = tmpB;                   //assign to PORTB
            break;
        case WAIT2: break;
        case PRESSED2:
            tmpB = (tmpB & 0x00) | 0x01;    //clear bits and set bit 0
            PORTB = tmpB;                   //assign to PORTB
        default: break;
    }

}


int main() {

    DDRA = 0x00; PORTA = 0xFF;	//set PORTA to inputs
    DDRB = 0xFF; PORTB = 0x00;	//set PORTB to outputs

    state = START;              //initialize state var
    tmpB = 0;                   //initialize tmpB var

    while (1) {
        Tick();                 //call tick()
    }

}

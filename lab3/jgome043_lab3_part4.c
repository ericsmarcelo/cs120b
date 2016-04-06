/*	jgome043_lab3_part4.c - 4/6/2016 1:17:16 AM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #4
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

enum States { START, INIT, DISARM, DISARM_OCT, ARM, ARM_OCT } state;      //state vars
unsigned char tmpB;                     //temp var for PORTB manip

void Tick() {
    //State Transitions
    switch (state) {
        case START:
        state = INIT; break;        //transition to INIT state
        case INIT:
        state = DISARM; break;      //transition to DISARM state
        case DISARM:
        //A0, A1, A2 not pressed, A7 pressed
        if (!(PINA & 0x01) && !(PINA & 0x02) && !(PINA & 0x04) && (PINA & 0x80)) {
            state = ARM;            //transition to ARM state
        }
        //A0, A1, A7 not pressed, A2 (#) pressed
        else if (!(PINA & 0x01) && !(PINA & 0x02) && (PINA & 0x04) && !(PINA & 0x80)) {
            state = DISARM_OCT;            //transition to DISARM_OCT state
        }
        break;
        case DISARM_OCT:
        //A0, A2, A7 not pressed, A1 (Y) pressed
        if (!(PINA & 0x01) && (PINA & 0x02) && !(PINA & 0x04) && !(PINA & 0x80)) {
            state = ARM;            //transition to ARM state
        }
        //A1, A2, A7 not pressed, A0 (X) pressed
        else if ((PINA & 0x01) && !(PINA & 0x02) && !(PINA & 0x04) && !(PINA & 0x80)) {
            state = DISARM;            //transition to DISARM state
        }
        //A0, A1, A2 not pressed, A7 pressed
        else if (!(PINA & 0x01) && !(PINA & 0x02) && !(PINA & 0x04) && (PINA & 0x80)) {
            state = ARM;            //transition to ARM state
        }
        break;
        case ARM:
        //A0, A1, A7 not pressed, A2 (#) pressed
        if (!(PINA & 0x01) && !(PINA & 0x02) && (PINA & 0x04) && !(PINA & 0x80)) {
            state = ARM_OCT;            //transition to ARM_OCT state
        }
        break;
        case ARM_OCT:
        //A0, A2, A7 not pressed, A1 (Y) pressed
        if (!(PINA & 0x01) && (PINA & 0x02) && !(PINA & 0x04) && !(PINA & 0x80)) {
            state = DISARM;            //transition to DISARM state
        }
        //A1, A2, A7 not pressed, A0 (X) pressed
        else if ((PINA & 0x01) && !(PINA & 0x02) && !(PINA & 0x04) && !(PINA & 0x80)) {
            state = ARM;            //transition to ARM state
        }
        //A0, A1, A2 not pressed, A7 pressed
        else if (!(PINA & 0x01) && !(PINA & 0x02) && !(PINA & 0x04) && (PINA & 0x80)) {
            state = ARM;            //transition to ARM state
        }
        break;
        default:
        state = START;              //error likely happened, restart
        break;
    }

    PORTC = state;                          //store current state to PORTC
    //State Actions
    switch (state) {
        case DISARM:
        tmpB |= 0x01;                   //set arm bit
        PORTB = tmpB;                   //set PORTB
        break;
        case ARM:
        tmpB &= 0xFE;                   //clear arm bit
        PORTB = tmpB;                   //set PORTB
        break;
        default:
        break;
    }
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	//set PORTA to inputs
    DDRB = 0xFF; PORTB = 0x00;	//set PORTB to outputs
    DDRC = 0xFF; PORTC = 0x00;	//set PORTC to outputs

    tmpB = 0;
    state = START;
    PORTC = state;                          //store current state to PORTC

    while(1)
    {
        Tick();                             //
    }
}

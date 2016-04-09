/*	jgome043_lab4_part1.c - 4/6/2016 3:17:25 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #4  Exercise #1
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

//function to set bit
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
//function to get bit value
unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}

enum States { START, INIT, WAIT, INPUT } state;
unsigned char fuel_level = 0;	//var to hold numerical fuel level
unsigned char tempC = 0;        //temp var to use as buffer for PORTC

void set_fuel() {
    fuel_level = (~PINA & 0x0F);	//get current fuel level

    //check if fuel level is near empty
    if (fuel_level <= 4) {      //illum low fuel lamp
        tempC = SetBit(tempC, 6, 1); //set temp var
    }
    else {	                    //clear low fuel lamp
        tempC = SetBit(tempC, 6, 0);    //set temp var
    }
    //check fuel level, illuminate/clear appropriate LED segments
    if (fuel_level >= 1) {
        tempC = SetBit(tempC, 5, 1);    //set temp var
    }
    else {
        tempC = SetBit(tempC, 5, 0);    //set temp var
    }
    if (fuel_level >= 3) {
        tempC = SetBit(tempC, 4, 1);    //set temp var
    }
    else {
        tempC = SetBit(tempC, 4, 0);    //set temp var
    }
    if (fuel_level >= 5) {
        tempC = SetBit(tempC, 3, 1);    //set temp var
    }
    else {
        tempC = SetBit(tempC, 3, 0);    //set temp var
    }
    if (fuel_level >= 7) {
        tempC = SetBit(tempC, 2, 1);    //set temp var
    }
    else {
        tempC = SetBit(tempC, 2, 0);    //set temp var
    }
    if (fuel_level >= 10) {
        tempC = SetBit(tempC, 1, 1);    //set temp var
    }
    else {
        tempC = SetBit(tempC, 1, 0);    //set temp var
    }
    if (fuel_level >= 13) {
        tempC = SetBit(tempC, 0, 1);    //set temp var
    }
    else {
        tempC = SetBit(tempC, 0, 0);    //set temp var
    }
    PORTC = tempC;              //set PORTC
}

void Tick() {
    switch(state) {
        case START:
            state = INIT; break;    // go to INIT state
        case INIT:
            state = WAIT; break;    // go to WAIT state
        case WAIT:
            // check if there is any input on PINA
            if ((PINA & 0x0F) > 0) {
                state = INPUT;      // go to INPUT state
            } break;
        case INPUT:
            // check if there is NO input on PINA
            if ((PINA & 0x0F) == 0) {
                state = INIT;       // return to WAIT state
            } break;
        default:
            state = START; break;   // something went wrong, reset to START state,
                                    // start over
    }

    switch(state) {
        case INIT:
            PORTC = PINC & 0xC0; break;     // initialize fuel gauge to 0
        case INPUT:
            set_fuel(); break;              // get fuel level and set fuel gauge 
        default: break;
    }
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	//set PORTA to inputs
    DDRC = 0xFF; PORTC = 0x00;	//set PORTC to outputs

    while(1)
    {
        Tick();
    }
}

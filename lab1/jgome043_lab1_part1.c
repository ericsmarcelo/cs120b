/*
 * jgome043_lab1_part1.c
 *
 * Created: 3/28/2016 2:55:31 PM
 * Author : Joel (jgome043@ucr.edu)
 * Lab Section: 021
 * Assignment: Lab 1 Exercise 1
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	//config port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00;	//config port B's 8 pins as outputs
                                //init output on PORTB to 0x00
    unsigned char tmpB = 0x00;	//temp var to hold value of B
    while (1) 
    {
        //if PA1PA0 is 01, set PB0=1, else =0
        if (PINA == 0x01) {	//true if PA0 is 1
            tmpB = (tmpB & 0xFE) | 0x01;	//sets tmpB to bbbbbbb1
                                        //(clear rightmost bit, then set to 1)
        }
        else {
            tmpB = (tmpB & 0xFE);
        }
        PORTB = tmpB;
    }
    return 0;
}


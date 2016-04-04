/*	jgome043_lab2_part2.c - 2016-03-30
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #2  Exercise #2
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

int main(void)
{
 	DDRA = 0x00; PORTA = 0xFF;	//set PORTA to inputs
 	DDRC = 0xFF; PORTC = 0x00;	//set PORTC to outputs

 	unsigned char fuel_level = 0;	//var to hold numerical fuel level
    unsigned char tempC = 0;        //temp var to use as buffer for PORTC

 	while(1)
 	{
 		fuel_level = (PINA & 0x0F);	//get current fuel level

        //check if fuel level is near empty
        if (fuel_level <= 4) {      //illum low fuel lamp
            tempC = SetBit(PORTC, 6, 1); //set temp var
        }
        else {	                    //clear low fuel lamp
            tempC = SetBit(PORTC, 6, 0);    //set temp var
        }
        //check fuel level, illuminate/clear appropriate LED segments
        if (fuel_level >= 1) {
            tempC = SetBit(PORTC, 5, 1);    //set temp var
        }
        else {
            tempC = SetBit(PORTC, 5, 0);    //set temp var
        }
        if (fuel_level >= 3) {
            tempC = SetBit(PORTC, 4, 1);    //set temp var
        }
        else {
            tempC = SetBit(PORTC, 4, 0);    //set temp var
        }
        if (fuel_level >= 5) {
            tempC = SetBit(PORTC, 3, 1);    //set temp var
        }
        else {
            tempC = SetBit(PORTC, 3, 0);    //set temp var
        }
        if (fuel_level >= 7) {
            tempC = SetBit(PORTC, 2, 1);    //set temp var
        }
        else {
            tempC = SetBit(PORTC, 2, 0);    //set temp var
        }
        if (fuel_level >= 10) {
            tempC = SetBit(PORTC, 1, 1);    //set temp var
        }
        else {
            tempC = SetBit(PORTC, 1, 0);    //set temp var
        }
        if (fuel_level >= 13) {
            tempC = SetBit(PORTC, 0, 1);    //set temp var
        }
        else {
            tempC = SetBit(PORTC, 0, 0);    //set temp var
        }
        PORTC = tempC;              //set PORTC
 	}
}

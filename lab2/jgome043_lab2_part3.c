/*	jgome043_lab2_part3.c - 3/30/2016 5:51:41 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #2  Exercise #3
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
    unsigned char tempC = 0;        //temp var to serve as buffer for PORTC

	while(1)
	{
		fuel_level = (PINA & 0x0F);	//get current fuel level

		//check if key is in ignition, driver is seated and seatbelt
		//not fastened
		if (GetBit(PINA, 4) && GetBit(PINA, 5) && !GetBit(PINA, 6)) {
			tempC = SetBit(tempC, 7, 1);	//illuminate seatbelt
											//warning lamp
		}
		else {
			tempC = SetBit(tempC, 7, 0);	//clear seatbelt
											//warning lamp
		}
		//check if fuel level is near empty
		if (fuel_level <= 4) {
			tempC = SetBit(tempC, 6, 1);	//illum low fuel lamp
		}
		else {
			tempC = SetBit(tempC, 6, 0);	//clear low fuel lamp
		}
		//check fuel level, illuminate/clear appropriate LED segments
        //set segment
		if (fuel_level >= 1) {
			tempC = SetBit(tempC, 5, 1);    //store changes to temp var
		}
        //clear segment
		else {
			tempC = SetBit(tempC, 5, 0);    //store changes to temp var
		}
        //set segment
		if (fuel_level >= 3) {
			tempC = SetBit(tempC, 4, 1);    //store changes to temp var
		}
        //clear segment
		else {
			tempC = SetBit(tempC, 4, 0);    //store changes to temp var
		}
        //set segment
		if (fuel_level >= 5) {
			tempC = SetBit(tempC, 3, 1);    //store changes to temp var
		}
        //clear segment
		else {
			tempC = SetBit(tempC, 3, 0);    //store changes to temp var
		}
        //set segment
		if (fuel_level >= 7) {
			tempC = SetBit(tempC, 2, 1);    //store changes to temp var
		}
        //clear segment
		else {
			tempC = SetBit(tempC, 2, 0);    //store changes to temp var
		}
        //set segment
		if (fuel_level >= 10) {
			tempC = SetBit(tempC, 1, 1);    //store changes to temp var
		}
        //clear segment
		else {
			tempC = SetBit(tempC, 1, 0);    //store changes to temp var
		}
        //set segment
		if (fuel_level >= 13) {
			tempC = SetBit(tempC, 0, 1);    //store changes to temp var
		}
        //clear segment
		else {
			tempC = SetBit(tempC, 0, 0);    //store changes to temp var
		}
        PORTC = tempC;                  //set PORTC
	}
}

/*
 *  jgome043_lab1_part4.c
 *
 *  Created: 3/29/2016 3:02:28 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #1  Exercise #4
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

#define max(x,y) ((x) > (y) ? (x) :(y))
#define min(x,y) ((x) < (y) ? (x) :(y))

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	//config port A to inputs, write to all 1
    DDRB = 0x00; PORTB = 0xFF;	//config port B to inputs, write to all 1
    DDRC = 0x00; PORTC = 0xFF;	//config port C to inputs, write to all 1
    DDRD = 0xFF; PORTD = 0x00;	//config port D to outputs, write to all 0

    unsigned short total_kg = 0;	//total passenger weight in kilograms
    unsigned char est_kg = 0;		//estimated passenger weight
    const unsigned char max_kg = 140;	//max weight allowed (in kg)
    const unsigned char diff_kg = 80;	//max difference allowed (in kg)
    unsigned short top_kg = 0;		//higher weight
    unsigned short bot_kg = 0;		//lower weight
    unsigned char tempD = 0;        //temp var for PORTD buffer

    while (1)
    {
		total_kg = PINA + PINB + PINC;
		top_kg = max(PINA, PINC);
		bot_kg = min(PINA, PINC);

		//check if total weight exceeds max (140kg)
		if (total_kg > max_kg) {
			tempD = (tempD & 0xFE) | 0x01;	//set PD0=1
		}
		else {
			tempD = tempD & 0xFE;	//clear PD0
		}
		//check if A and B weight difference exceeds max (80kg)
		if ((top_kg - bot_kg) > diff_kg) {
			tempD = (tempD & 0xFD) | 0x02;	//set PD1=1
		}
		else {
			tempD = tempD & 0xFD;	//clear PD1
		}

		//check if total weight/4 exceeds 6 bit representation
		//ie 252kg
		if ((total_kg >> 2) > 63) {
			tempD = (tempD & 0x03) | 0xFC;	//set bits D7-D2=1
		}
		//check if total weight is less than 4kg
		//(4kg is 6 bit representation minimum)
		else if (total_kg < 4) {
			tempD = tempD & 0x03;			//set bits D7-D2=0
		}
		else {	//show 6 bit representation of weight/4 on D7-D2
			est_kg = total_kg;					//initialize estimated weight
			est_kg = (est_kg >> 2) << 2;		//truncate estimated weight
			tempD = (tempD & 0x03) | est_kg;	//show estimated weight/4
												//on pins D7-D2
		}
        PORTD = tempD;                  //set PORTD
    }
	return 0;
}

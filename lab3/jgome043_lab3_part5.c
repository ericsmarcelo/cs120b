/*	jgome043_lab3_part5.c - 4/6/2016 1:56:52 AM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #5
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

const unsigned char key1 = 0x01;     //X
const unsigned char key2 = 0x02;     //Y
const unsigned char key3 = 0x04;     //#
const unsigned char CODE[] = { 0x04, 0x01, 0x02, 0x01 };        //unlock code
const unsigned char MAX_NUM = 4;                                //max number of keys for code

enum States { START, INIT, DISARM, ARM, KEY_PRS } state;      //state vars
unsigned char tmpB;                     //temp var for PORTB manip
unsigned char user_input[4];            //storage array for user input
unsigned char keys_entered;             //var to keep track of num of keys entered
unsigned char prev_state;               //temp var to store previous state since last key press

void check_code() {
    //user has finished inputting a code
    if (keys_entered >= MAX_NUM) {
        unsigned char invalid_keys = 0;         //count invalid keys
        for (unsigned char i = 0; i < MAX_NUM; i++) {
            //check for discrepancy in user input against valid code
            if (user_input[i] != CODE[i]) {
                invalid_keys++;         //increment invalid keys counter
            }
            user_input[i] = 0;          //clear user input array
        }
        //check for exactly no variance in user input vs valid code
        if (invalid_keys == 0) {
            state = (prev_state == ARM) ? DISARM : ARM;  //invert the current arm/disarm state
            prev_state = KEY_PRS;                   //store current state as key pressed
        }
        keys_entered = 0;                       //reset keys entered counter
    }
}

void get_code() {
    //check for last state being a key pressed
    if (prev_state == KEY_PRS) {
        prev_state = state;             //store current arm/disarm state
        state = KEY_PRS;                //return to key pressed state
    }
    //check for user input required
    else if (keys_entered < MAX_NUM) {
            //X pressed
            if ((PINA & key1) && !(PINA & key2) && !(PINA & key3)) {
                user_input[keys_entered] = key1;    //store user input
                keys_entered++;                     //increment keys entered counter
                prev_state = state;                 //store current arm/disarm state
                state = KEY_PRS;                    //change state to KEY_PRS (key pressed)
            }
            //Y pressed
            else if (!(PINA & key1) && (PINA & key2) && !(PINA & key3)) {
                user_input[keys_entered] = key2;    //store user input
                keys_entered++;                     //increment keys entered counter
                prev_state = state;                 //store current arm/disarm state
                state = KEY_PRS;                    //change state to KEY_PRS (key pressed)
            }
            //# pressed
            else if (!(PINA & key1) && !(PINA & key2) && (PINA & key3)) {
                user_input[keys_entered] = key3;    //store user input
                keys_entered++;                     //increment keys entered counter
                prev_state = state;                 //store current arm/disarm state
                state = KEY_PRS;                    //change state to KEY_PRS (key pressed)
            }
    }
    else {
        check_code();               //attempt to validate code
    }
}

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
            get_code();                 //get user input
            break;
        case ARM:
            get_code();                 //get user input
            break;
        case KEY_PRS:
            //check that all no keys are still pressed
            if (!(PINA & 0x01) && !(PINA & 0x02) && !(PINA & 0x04) && !(PINA & 0x80)) {
                state = prev_state;            //transition to last arm/disarm state
            }
            check_code();               //attempt to validate code
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
        Tick();
    }
}

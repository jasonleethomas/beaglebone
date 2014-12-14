/*
 * 	morse.h
 *	description:
 *		defines morse class converts characters
 *		to morse code. displays morse on beaglebone's
 *		usr led.
 *
 *  created on: Nov 20, 2014
 *      author: jason
 */

#ifndef MORSE_H_
#define MORSE_H_

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <led.h>

#define MORSE_DOT '.'
#define MORSE_DASH '-'
#define MORSE_SPACE '_'

#ifndef MORSE_DOT_GAP
	#define MORSE_DOT_GAP 250000 // milliseconds
#endif
#define MORSE_DASH_GAP (3 * MORSE_DOT_GAP)
#define MORSE_WORD_GAP (7 * MORSE_DOT_GAP)

char* morse_map[27] = {
".-", 	// a
"-...",	// b
"-.-.",	// c
"-..",	// d
".",	// e
"..-.",	// f
"--.",	// g
"....",	// h
"..",	// i
".---",	// j
"-.-",	// k
".-..",	// l
"--",	// m
"-.",	// n
"---",	// o
".--.",	// p
"--.-",	// q
".-.",	// r
"...",	// s
"-",	// t
"..-",	// u
"...-",	// v
".--",	// w
"-..-",	// x
"-.--",	// y
"--.."	// z
};

char* morse_space = "_";

typedef struct {
	char  morse_char;
	char* morse_code;
	char* morse_err;
} morse;

// initializes led:usr0 by default,
// generates morse code for char passed,
// 0 if successful, 1 otherwise
int morse_init(morse*, char);

// flashes morse code to led
// 0 if succesful, 1 otherwise
int morse_flash(morse*, led*);

char* char2morse(char);
void  morse_dot(led*);  	// function flashes morse dot to led
void  morse_dash(led*); 	// function flashes morse dash to led
void  morse_gap(int);	// sleep() wrapper function to create morse gap

int morse_init(morse* this, char c) {
	if(isalpha(c))  {
		this->morse_char = tolower(c);
		this->morse_code = char2morse(this->morse_char);
		if(this->morse_code == NULL) {
			this->morse_err = "Error: no morse code exists for character passed";
			return 1;
		}
	}
	else if(isspace(c)) {
		this->morse_char = c;
		this->morse_code = morse_space;
	}
	else {
		this->morse_err = "Error: unrecognized character";
		return 1;
	}

	return 0;
}

int morse_flash(morse* this, led* morse_led) {
	int i;
	for(i = 0; i < strlen(this->morse_code);  i++) {
		switch(this->morse_code[i]) {
		case MORSE_DOT:
			morse_dot(morse_led);
			break;
		case MORSE_DASH:
			morse_dash(morse_led);
			break;
		case MORSE_SPACE:
			morse_gap(MORSE_WORD_GAP);
			break;
		default:
			morse_gap(MORSE_DOT_GAP);
			break;
		}
	}

	return 0;
}

// internal worker functions

// if c is an ascii alphabetic char, function returns morse code
// returns null otherwise
char* char2morse(char c) {
	char ascii_offset = 97;
	int index = (int)(c - ascii_offset);	// direct hashing of ascii used to map chars to morse code
	if(index >= 0 && index <= 26)			// index within morse_map boundaries; valid alphabetic char
		return morse_map[index];
	else
		return NULL;
}


void morse_dot(led* morse_led) {

	led_toggle(morse_led);
	morse_gap(MORSE_DOT_GAP);

	if(!morse_led->isOn)
		morse_dot(morse_led);
	else
		led_toggle(morse_led);

	morse_gap(MORSE_DOT_GAP);
}

// function flashes morse dash to led
void morse_dash(led* morse_led) {

	led_toggle(morse_led);

	if(!morse_led->isOn) {
		morse_gap(MORSE_DOT_GAP);
		morse_dash(morse_led);
	}
	else {
		morse_gap(MORSE_DASH_GAP);
		led_toggle(morse_led);
	}

	morse_gap(MORSE_DOT_GAP);
}

void morse_gap(int gap) {
	usleep(gap);
}

#endif

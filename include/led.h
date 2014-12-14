/*
 *	led.h
 *	description:
 *		defines led class that controls beaglebone's
 *		usr leds.
 *		for now, only allows user to toggle leds on/off
 *		will be extended eventually.
 *
 *	created on: Nov 20, 2014
 *		author: jason
 */


#ifndef LED_H_
#define LED_H_

#include <stdio.h>

#ifndef NUMLED
#define NUMLED 4 	// number of LEDs supported
#endif

// trigger files
char* usr_trg[] = { 	"/sys/class/leds/beaglebone:green:usr0/trigger",
			"/sys/class/leds/beaglebone:green:usr1/trigger",
			"/sys/class/leds/beaglebone:green:usr2/trigger",
			"/sys/class/leds/beaglebone:green:usr3/trigger"};

// brightness files
char* usr_brt[] = { 	"/sys/class/leds/beaglebone:green:usr0/brightness",
			"/sys/class/leds/beaglebone:green:usr1/brightness",
			"/sys/class/leds/beaglebone:green:usr2/brightness",
			"/sys/class/leds/beaglebone:green:usr3/brightness"};

typedef struct {
	unsigned char isOn;

	FILE* 	led_ptr;	// file handler
	char* 	led_trg;	// trigger file
	char* 	led_brt;	// brightness file
	char* 	led_err;	// error logs for led
	int 	led_ndx;	// led index of usr array (0-3)
} led;

// led object methods
int led_init(led*, int);	// initialize led files. returns 0 if successful, 1 otherwise
int led_toggle(led*);		// toggles LEDs on/off. returns 0 if successful 1 otherwise
int led_reset(led*);		// resets LEDs to their predefined operation. returns 0 if successful, 1 otherwise

int led_init(led* this, int index) {

	this->isOn = 0;
	this->led_ptr = NULL;

	if(!(index >= 0 && index <= 3)) {
		this->led_err = "Error: led index out of bounds";
		return 1;
	}
	else
		this->led_ndx = index;

	this->led_trg = usr_trg[this->led_ndx];
	this->led_brt = usr_brt[this->led_ndx];

	this->led_ptr = fopen(this->led_trg, "w+");
	if(!this->led_ptr) {
		this->led_err = "Error: invalid trigger file";
		return 1;
	}
	else {
		fputs("none", this->led_ptr);
		fclose(this->led_ptr);
		this->led_ptr = NULL;
	}

	this->led_ptr = fopen(this->led_brt, "w+");
	if(!this->led_ptr) {
		this->led_err = "Error: invalid brightness file";
		return 1;
	}
	else {
		fputs("0",this->led_ptr);
		fclose(this->led_ptr);
		this->led_ptr = NULL;
	}

	return 0;
}

int led_toggle(led* this) {
	this->led_ptr = fopen(this->led_brt, "w+");
	if(this->isOn){
		fputs("0", this->led_ptr);
		this->isOn = 0;
	}
	else {
		fputs("1", this->led_ptr);
		this->isOn = 1;
	}

	fclose(this->led_ptr);
	this->led_ptr = NULL;

	return 0;
}

int led_reset(led* this) {

	this->led_ptr = fopen(this->led_trg, "w+");
	if(!this->led_ptr) {
		this->led_err = "Error: invalid trigger file";
		return 1;
	}
	else {
		switch(this->led_ndx) {
		case 0:
			fputs("heartbeat", this->led_ptr);
			break;
		case 1:
			fputs("mmc0", this->led_ptr);
			break;
		case 2:
			fputs("cpu0", this->led_ptr);
			break;
		case 3:
			fputs("mmc1", this->led_ptr);
			break;
		default:
			break;
		}
	}

	fclose(this->led_ptr);
	this->led_ptr = NULL;
	return 0;
}

#endif

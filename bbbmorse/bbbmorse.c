/*
 * 	morse_flasher.c
 *	description:
 *		flashes characters in textfile, uses beaglebone's
 *		usr leds. each character is wrapped in morse object
 *		which converts char it's to morse code. each morse
 *		object is added to message queue, and is the displayed
 *		in order.
 *	inputs:
 *		filename(optional): looks for message.txt in
 *			current folder if not specified.
 *		character limit (optional):
 *			set to 512 if not specified.
 *		led index(optional): chooses beaglebone's usr(0-4)
 *			set to 0 if not specified
 *	error handling:
 *		in case of critical errors, heap vars are
 *		freed, global error string is written to
 *		error stream; program then makes a hard exit.
 *
 *  created on: Nov 20, 2014
 *      author: jason
 */

#include <stdio.h>
#include <stdlib.h>
#include <led.h>	// beaglebone black's usr leds
#include <genque.h> // generic queue and methods
#include "morse.h"	// converts char to morse object

char* error_message; 	// passed around by functions to store error messages
int error = 0;			// used to communicate errors between functions

/*	desc: 6pReads file, converts each character
 * 		to morse object, then adds it to que.
 * 	return: 0 if successful, 1 otherwise
 */
int read_message(queue* que, char* file, int);

/*	desc: Flashes each morse code on flash led
 * 		then removes it from que.
 * 	return: 0 if successful, 1 otherwise
 */
int disp_message(queue* que, led* flash);

int main(int argvc, char* argv[]) {
	char* filename;

	// maximum number of characters supported,
	// using max word count would be better though
	int maxchar;

	// index to choose led: usr(0-3)
	int ledindex;

	if(argvc < 4) {
		ledindex = 0;
		printf("Warning: led index not specified; using usr%d \n", ledindex);
	}
	else {
		ledindex = atoi(argv[3]); // convert command-line string to int
		if(ledindex < 0 || ledindex > 3) {
			printf("Warning: led index usr%d not valid; using usr0", ledindex);
			ledindex = 0;
		}
	}

	if(argvc < 3) {
		maxchar = 512;
		printf("Warning: character limit not specified; set to %d\n", maxchar);
	}
	else {
		maxchar = atoi(argv[2]);
		if(maxchar > 2056)
			printf("Warning: high character limit; ensure enough memory is available");
	}
	if(argvc < 2) {
		filename = "./message.txt";
		printf("Warning: No filename passed, attempting to read from %s\n", filename);
	}
	else
		filename = argv[1]; // filename passed from command-line

	queue message;
	queue_init(&message);

	error = read_message(&message, filename, maxchar);
	if(error) {
		perror(error_message);
		return 1;
	}

	led* morse_led = malloc(sizeof(*morse_led));
	if(morse_led == NULL) {
		perror("Error: cannot initialize led");
		return 1;
	}
	else {
		error = led_init(morse_led, ledindex);
		if(error) {
			perror(error_message);
			free(morse_led);
			return 1;
		}
	}

	error = disp_message(&message, morse_led);
	if(error) {
		perror(error_message);
		free(morse_led);
		return 1;
	}

	// cleanup
	led_reset(morse_led);
	free(morse_led);
	queue_destroy(&message);

	return 0;
}

int read_message(queue* message, char* filename, int maxchar) {
	FILE* file;
	file = fopen(filename,"r+");
	if(file == NULL) {
		error_message = "Error: cannot open file";
		return 1;
	}
	int i;
	for(i = 0; i < maxchar; i++) { // to avoid memory overflow
		if(feof(file)) // if file ends before we hit limit
			break;
		else {
			int buffer = fgetc(file);
			if(!(isalpha(buffer) || isspace(buffer)))
				continue;	// ignore if not an alphabet or space
			else {
				morse* code = malloc(sizeof(*code));
				if(code == NULL) {
					error_message = "Error: cannot assign memory to new morse object";
					return 1;
				}
				error = morse_init(code, buffer);
				if(error) {
					error_message = code->morse_err;
					free(code);
					return 1;
				}
				error = queue_enq(message, code);
				if(error) {
					error_message = message->err_msg;
					free(code);
					return 1;
				}
			}
		}
	}

	fclose(file);

	return 0;
}

int disp_message(queue* message, led* morse_led) {
	morse* buffer = malloc(sizeof(*buffer));
	if(buffer == NULL) {
		error_message = "Error: cannot assign memory to new morse object";
		return 1;
	}
	else {
		while(!queue_isempty(message)) { // while message isn't empty
			buffer = queue_front(message);
			morse_flash(buffer, morse_led);
			queue_deq(message);
		}

		free(buffer);
	}

	return 0;
}

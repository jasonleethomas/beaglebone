#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <led.h>

#define NUMCYC 10

int main() {
	srand(time(NULL));
	int rnd = 0; // starts cycle at usr0

	led usr[NUMLED];
	int errFlg = 0, i;
	for(i = 0; i < NUMLED; i++) {
		errFlg = led_init( &usr[i], i);
		if(errFlg) {
			printf("%s\n", usr[i].led_err);
			return 1;
		}
	}

	for(i = 0; i < NUMCYC; i++) {
		led_toggle(&usr[rnd]);		// toggle on
		sleep(TIMDLY);				// wait TIMDLY seconds
		led_toggle(&usr[rnd]); 		// toggle off
		do {
			rnd = rand() % NUMLED;	// select random LED
		} while(usr[rnd].isOn);		// avoid selecting an active led	p.s: hangs state machine, use fixed algorithm.
	}

	for(i = 0; i < NUMCYC; i++) {
		led_reset(&usr[i]);
	}

	return 0;
}

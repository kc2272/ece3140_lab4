/* mytest_1.c
 *
 * Joshua Kwakye Minott
 * Cornell University
 * All right reserved
 *
 * Oct 2026
 * Ithaca NY
 *
 * This file is part of the ECE3140/CS3420 offering for Fall 2026. If you
 * are not part of this class you should not have access to this file. Do not
 * post, share, or otherwise distribute this file. We will consider it an AI
 * violation if you do. If you somehow get this code and you are NOT enrolled
 * the Fall 2026 version of ECE3140 please contact the course staff
 * immediately and describe how you found it.
 *
 * mytest_1.c
 * The point of this test is to ensure that mutual exclusivity is preserved under a case where many
 * processes are actively trying to:
 * 		1. Access the same lock
 * 		2. Access the same hardware
 *
 * 	4 processes, concurrently running and making progress at different points try to access the same LED
 * 	to send their morse code number. The processes should all act separately and should not interrupt each other
 * 	After every number has been morseed, the red frdm led should flash to indicate it's completion
 */

#include "3140_concur.h"
#include "led.h"
#include "lock.h"

#define UNIT_MS 200 // Defining the length of a dot, which is 200 ms here

lock_t l; // The only lock we are using

// Lab 1 morse_digit. 1 = dot, 0 = dash. Same process used on last lab. The differing calculations will also
// Add some variability in when the process is ready to use the critical section
static unsigned int morse_pattern(unsigned int digit) {
    unsigned int pattern = 0;
    unsigned int bit = 16;
    unsigned int set = 1;

    if (digit > 9) {
        return 0xFFFFFFFF;
    }

    while (digit != 0) {
        if (bit == 0) {
            set = 0;
            bit = 16;
            continue;
        }
        digit--;
        if (set) {
            pattern |= bit;
        } else {
            pattern &= ~bit;
        }
        bit >>= 1;
    }

    return pattern;
}

// Morse out via the pattern we calculate
static void morse(unsigned int digit) {
    unsigned int pattern = morse_pattern(digit);
    l_lock(&l); // Critical section starts
    for (unsigned int mask = 16; mask != 0; mask >>= 1) {
    	green_toggle_frdm();
        delay( (pattern & mask) ? UNIT_MS : 3 * UNIT_MS);
        green_toggle_frdm();
        delay(UNIT_MS);
    }
    red_toggle_frdm(); // Note that the more has finished
    delay(500);
    red_toggle_frdm();
	l_unlock(&l); // Critical section ends
}

void p1(void) { morse(3); }

void p2(void) { morse(1); }

void p3(void) {	morse(4); }

void p4(void) { morse(0); }

int main(void) {
	led_init();
	l_init(&l); // Initialize the lock

	if (process_create(p1, 48) < 0) {
		return -1;
	}

	if (process_create(p2, 48) < 0) {
		return -1;
	}

	if (process_create(p3, 48) < 0) {
		return -1;
	}

	if (process_create(p4, 48) < 0) {
		return -1;
	}

	process_start();

	blue_on(); // Just turn on blue after the whole process is done.

	while(1);
}


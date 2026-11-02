/* mytest_2.c
 *
 * Tests to make sure that a blocked process isn't using CPU cycles (spinlock does use cycles)
 *
 * p1: Takes lock first and holds it for a long critical section (red LED continually on) before releasing the lock
 * p2: Tries to take the lock from p1 (releases lock immediately after acquiring it)
 * p3: Runs the whole time (no lock required) so the CPU is running multiple processes simultaneously
 *
 * Expected behavior:
 * Spinlock: p2 spins while waiting for p1 to release the lock so p1, p2, and p3 all get 1/3 of the CPU time
 * 			(p1 takes ~9.5 seconds to complete when I used a stopwatch)
 * Blocking lock: p2 is blocked while waiting so it uses no CPU cycles; p1 and p3 use 1/2 of the CPU time
 * 			(p1 takes ~6.3 seconds to complete when I used a stopwatch)
 */

#include "3140_concur.h"
#include "led.h"
#include "lock.h"

lock_t l;

void p1(void) {
	int i;
	l_lock(&l);

	// critical section: keeps red LED on and releases lock after LED turns off
	red_toggle_frdm();
	for (i = 0; i < 10; i++) {
		delay(300);
	}
	red_toggle_frdm();

	l_unlock(&l);
}

void p2(void) {
	// immediately releases lock after acquiring it
	// spins while p1 is locked for spinlock, blocked for blocking lock
	l_lock(&l);
	l_unlock(&l);
}

void p3(void) {
	int i;
	// doesn't need lock; takes longer than p1 critical section
	for (i = 0; i < 30; i++) {
		delay(300);
		green_toggle_frdm();
	}
}

int main(void) {
	led_init();
	l_init(&l);

	// locks acquired in order of creation (p1 -> p2)
	if (process_create(p1, 20) < 0) {
		return -1;
	}
	if (process_create(p2, 20) < 0) {
		return -1;
	}
	if (process_create(p3, 20) < 0) {
		return -1;
	}

	process_start();

	green_on_frdm(); 						// everything finished

	while (1); 								// loop forever
	return 0;
}


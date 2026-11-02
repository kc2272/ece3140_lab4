
/* lock_spin.c
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
 */

#include "lock.h"
#include <stdlib.h>
#include "3140_concur.h"

/**
 * Initialises the lock structure
 *
 * @param l pointer to lock to be initialised
 */
void l_init(lock_t* l) {
	lock_t lock; // Create lock object
	lock.locked = 0; // Initially unlocked
	lock.head = NULL; // Points to nothing given spinlock
	*l = lock; // Point l at lock object
}

/**
 * Grab the lock or block the process until lock is available
 *
 * @param l pointer to lock to be grabbed
 */

// Must do this function with interrupts given that we have no atomic functions we can use
void l_lock(lock_t* l) {
	while (1) {
		uint32_t prevState = __get_PRIMASK(); // Get the current state of interrupts
		__disable_irq(); // Turn off interrupts whilst we're checking
		if (l->locked == 0) { // If lock is free
			l->locked = 1; // lock the lock
			__set_PRIMASK(prevState); // Return the state of interrupts to what it was before
			return; // Break out of function as we have locked the lock
		} else {
			__set_PRIMASK(prevState); // Return the state of interrupts to what it was before 
		}
	}
}

/**
 * Release the lock along with the first process that may be waiting on
 * the lock. This ensures fairness wrt lock acquisition.
 *
 * @param l pointer to lock to be unlocked
 */
void l_unlock(lock_t* l) {
	l->locked = 0;
}

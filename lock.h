/* lock.h
 *
 * Nils Napp
 * Cornell University
 * All right reserved
 *
 * Jan 2024
 * Ithaca NY
 *
 * This file is part of the ECE3140/CS3420 offering for Fall 2026. If you
 * are not part of this class you should not have access to this file. Do not
 * post, share, or otherwise distribute this file. We will consider it an AI
 * violation if you do. If you somehow get this code and you are NOT enrolled
 * the Fall 2026 version of ECE3140 please contact the course staff
 * immediately and describe how you found it.
 */

#ifndef __LOCK_H_INCLUDED__
#define __LOCK_H_INCLUDED__

#include "3140_concur.h"

typedef struct lock_ {
 /*
  * YOU NEED TO PUT SOMETHING HERE
  */
} lock_t;

/**
 * Initialises the lock structure
 *
 * @param l pointer to lock to be initialised
 */
void l_init(lock_t* l);

/**
 * Grab the lock or block the process until lock is available
 *
 * @param l pointer to lock to be grabbed
 */
void l_lock(lock_t* l);

/**
 * Release the lock along with the first process that may be waiting on
 * the lock. This ensures fairness wrt lock acquisition.
 *
 * @param l pointer to lock to be unlocked
 */
void l_unlock(lock_t* l);

#endif /* __LOCK_H_INCLUDED */

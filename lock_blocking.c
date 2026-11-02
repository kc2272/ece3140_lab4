/*
 * lock_blocking.c
 */

#include "lock.h"
#include "3140_concur.h"

void l_init(lock_t *l) {
    l->locked = 0; 						// not locked
    l->head = NULL; 					// no blocked queue
}

void l_lock(lock_t *l) {
    __disable_irq();
    if (l->locked == 0) {
        l->locked = 1; 					// acquired lock successfully
        __enable_irq();
        return;
    }
    // failed to acquire lock (add to blocked queue)
    current_process_p->blocked = 1;
    current_process_p->next = NULL; 	// tail end of blocked queue

    if (l->head == NULL) {
        l->head = current_process_p;    // queue was empty
    } else {
        process_t *p = l->head;
        while (p->next) p = p->next;    // move through queue until end is reached
        p->next = current_process_p;    // add current process to end of blocked queue
    }

    process_blocked();                  // context switch
}

void l_unlock(lock_t *l) {
    __disable_irq();
    if (l->head) {
        process_t *p = l->head;			// remove from blocked queue and add to normal process queue
        l->head = p->next;
        p->next = NULL;
        p->blocked = 0;
        enqueue(p, &process_queue);     // runnable again
        // l->locked stays 1
    } else {
        l->locked = 0;
    }
    __enable_irq();
}

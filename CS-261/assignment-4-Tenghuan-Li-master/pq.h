/*
 * This file contains the definition of the interface for the priority queue
 * you'll implement.  You can find descriptions of the priority queue functions,
 * including their parameters and their return values, in pq.c.
 *
 * You should not modify this file.
 */

#ifndef __PQ_H
#define __PQ_H

/*
 * Structure used to represent a priority queue.
 */
struct pq;

/*
 * Priority queue interface function prototypes.  Refer to pq.c for
 * documentation about each of these functions.
 */
struct pq* pq_create();
void pq_free(struct pq* pq);
int pq_isempty(struct pq* pq);
void pq_insert(struct pq* pq, void* value, int priority);
void* pq_first(struct pq* pq);
int pq_first_priority(struct pq* pq);
void* pq_remove_first(struct pq* pq);

void prio_swap(struct pq* pq, int num1, int num2);
int priority_value_compare(struct pq* pq, int num1, int num2);
void priority_queue_head(struct pq* pq);
void priority_queue_tail(struct pq* pq);

#endif

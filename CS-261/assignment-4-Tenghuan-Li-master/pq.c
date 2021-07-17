/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:Tenghuan Li
 * Email:liten@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>
#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq{
  struct dynarray* dy_pq;
};

struct priority_value{
  int prio;
  void* value;
};

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */

struct pq* pq_create() {

  struct pq* new_pq = malloc( sizeof( struct pq ) );
  assert(new_pq);
  new_pq->dy_pq = dynarray_create();
  return new_pq;
}

//define a swap function
void prio_swap(struct pq* pq, int num1, int num2){
  //swap two node
  struct priority_value* node1 =dynarray_get(pq->dy_pq,num1);
  struct priority_value* node2 = dynarray_get(pq->dy_pq,num2);
  //swap them two node
  dynarray_set(pq->dy_pq,num1, node2);
  dynarray_set(pq->dy_pq,num2, node1);
  return;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  assert(pq);
  int i=0;
  while(i<dynarray_size(pq->dy_pq)){
    struct priority_value* the_node = dynarray_get(pq->dy_pq,i);
    free(the_node);
    i++;
  }

  dynarray_free(pq->dy_pq);
  free(pq);
  return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  assert(pq);
  if(dynarray_size(pq->dy_pq)==0){
    return 1;
  }else{
    return 0;
  }
}

//define a compare function
int priority_value_compare(struct pq* pq, int num1, int num2){
  struct priority_value* node1 =dynarray_get(pq->dy_pq,num1);
  struct priority_value* node2 = dynarray_get(pq->dy_pq,num2);
  int key;
  if(dynarray_size(pq->dy_pq)){
    key = node1->prio - node2->prio;
    return key;
  }
  return -1;
}

//define function for the queue head
void priority_queue_head(struct pq* pq){
  assert(pq);
  int i,j;
  i=j=0;
  int pare = 0;
  int left = 0;
  int right = 0;
  int size= dynarray_size(pq->dy_pq);

  while(i< (size-1)/2){
    pare = i;
    left = 2*i +1;
    right = left+1;
    j = left;
    if(priority_value_compare(pq,left,right) >0 ){
      j++;
    }
    if(priority_value_compare(pq,pare,j)>0){
      prio_swap(pq,i,j);
      i=j;
    }else{
      break;
    }
  }
  return;
}

//define a function for tail
void priority_queue_tail(struct pq* pq){
  assert(pq);
  int i;
  int pare;
  int child;
  i = dynarray_size(pq->dy_pq)-2;
  while(i>0){
    child = i;
    pare = (child -1)/2;

    if(priority_value_compare(pq, pare, child) > 0 ){
      prio_swap(pq, child, pare);
      i= pare;
    }else{
      break;
    }
  }
  return;
}
/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  assert(pq);
  struct priority_value* node = malloc(sizeof(struct priority_value));
  node->prio = priority;
  node->value = value;

  dynarray_insert(pq->dy_pq, node);
  priority_queue_tail(pq);
  return;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  assert(pq);
  struct priority_value* new = NULL;
  if(dynarray_size(pq->dy_pq)){
    new = dynarray_get(pq->dy_pq,0);
    return new->value;
  }
  return NULL;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  assert(pq);

  if(dynarray_size(pq->dy_pq)){
    struct priority_value* new = dynarray_get(pq->dy_pq,0);
    return new->prio;
  }
  return -1;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  assert(pq);
  struct priority_value* new_node = NULL;
  int size;
  int* value;
  size = dynarray_size(pq->dy_pq);

  if(size){
    new_node = dynarray_get(pq->dy_pq,0);
    prio_swap(pq, 0 ,size -1);
    dynarray_remove(pq->dy_pq, -1);
    value = new_node->value;
    free(new_node);
    priority_queue_head(pq);
    return value;
  }
  return NULL;
}

/*
 * This file is where you should implement your queue.  It already contains
 * skeletons of the functions you need to implement (along with documentation
 * for each function).  Feel free to implement any additional functions you
 * might need.  Also, don't forget to include your name and @oregonstate.edu
 * email address below.
 *
 * Name:Tenghuan Li
 * Email:liten@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "dynarray.h"

//int had = 0;// defi a logical 1 where it physcial address point for dynarray
//int end = 0;//defi a end point for dynarray
/*
 * This is the structure that will be used to represent a queue.  This
 * structure specifically contains a single field representing a dynamic array
 * that should be used as the underlying data storage for the queue.
 *
 * You should not modify this structure.
 */
struct queue {
  struct dynarray* array;
};

/*
 * This function should allocate and initialize a new, empty queue and return
 * a pointer to it.
 */
struct queue* queue_create() {
  struct queue* new_que = malloc(sizeof(struct queue));
  new_que->array= dynarray_create();
  return new_que;
}

/*
 * This function should free the memory associated with a queue.  While this
 * function should up all memory used in the queue itself, it should not free
 * any memory allocated to the pointer values stored in the queue.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   queue - the queue to be destroyed.  May not be NULL.
 */
void queue_free(struct queue* queue) {
  assert(queue);
  dynarray_free(queue->array);
  free(queue);
  return;
}

/*
 * This function should indicate whether a given queue is currently empty.
 * Specifically, it should return 1 if the specified queue is empty (i.e.
 * contains no elements) and 0 otherwise.
 *
 * Params:
 *   queue - the queue whose emptiness is being questioned.  May not be NULL.
 */
int queue_isempty(struct queue* queue) {
  assert(queue);
  return dynarray_isempty(queue->array);
}

/*
 * This function should enqueue a new value into a given queue.  The value to
 * be enqueued is specified as a void pointer.  This function must have O(1)
 * average runtime complexity.
 *
 * Params:
 *   queue - the queue into which a value is to be enqueued.  May not be NULL.
 *   val - the value to be enqueued.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
 //因为时间复杂度要求O(1),所以需要进行取余计算，因而需要得到login_start,size,capacity,
 //这三个元素来进行操作，所以在此处新定义函数，在dynarray.c中操作更简单
void queue_enqueue(struct queue* queue, void* val) {
  assert(queue);
  // int siz = dynarray_get_size(queue->array);
  // int capa = dynarray_get_capa(queue->array);
  //
  // if(dynarray_capa_check(queue->array)){
  //   struct dynarray* new_dy= dynarray_create();
  //   for(int i=0;i<capa;i++){
  //     int phy = (had+i)%capa;
  //     void* value = dynarray_get(queue->array,phy);
  //     dynarray_insert(queue->array,i,value);
  //   }
  //   had=0;
  //   _dynarray_resize(new_dy,2 * capa);
  // }
  // int new_address=(had+siz)%capa;
  // dynarray_insert(queue->array,new_address,val);
  dynarray_enqueue(queue->array, val);
  return;
}

/*
 * This function should return the value stored at the front of a given queue
 * *without* removing that value.  This function must have O(1) average runtime
 * complexity.
 *
 * Params:
 *   queue - the queue from which to query the front value.  May not be NULL.
 */
void* queue_front(struct queue* queue) {
  assert(queue);
  return dynarray_front(queue->array);
}

/*
 * This function should dequeue a value from a given queue and return the
 * dequeued value.  This function must have O(1) average runtime complexity.
 *
 * Params:
 *   queue - the queue from which a value is to be dequeued.  May not be NULL.
 *
 * Return:
 *   This function should return the value that was dequeued.
 */
void* queue_dequeue(struct queue* queue) {
//  void* final = dynarray_get(queue->array,had);
//  dynarray_remove(queue->array,had);
//  had++;
  assert(queue);
  return dynarray_dequeue(queue->array);
}

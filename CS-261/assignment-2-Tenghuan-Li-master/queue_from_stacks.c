/*
 * This file is where you should implement your queue-from-stacks.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name:Tenghuan Li
 * Email:liten@oregonsatae.edu
 */

#include <stdlib.h>
#include<assert.h>
#include "stack.h"
#include "queue_from_stacks.h"

/*
 * This is the structure that will be used to represent a queue using two
 * stacks.  It specifically contains two fields, each representing a stack.
 * These two stacks should be used together as the underlying data storage for
 * the queue.
 *
 * You should not modify this structure.
 */
struct queue_from_stacks {
  struct stack* s1;
  struct stack* s2;
};

/*
 * This function should allocate and initialize a new, empty queue-from-stacks
 * and return a pointer to it.
 */
struct queue_from_stacks* queue_from_stacks_create() {
  struct queue_from_stacks* new_que = malloc(sizeof(struct queue_from_stacks));
  assert(new_que);
  new_que->s1 = stack_create();
  new_que->s2 = stack_create();
  return new_que;
}

/*
 * This function should free the memory associated with a queue-from-stacks.
 * While this function should up all memory used in the queue-from-stacks
 * itself, it should not free any memory allocated to the pointer values stored
 * in the queue.  This is the responsibility of the caller.
 *
 * Params:
 *   qfs - the queue-from-stacks to be destroyed.  May not be NULL.
 */
void queue_from_stacks_free(struct queue_from_stacks* qfs) {
  assert(qfs);
  stack_free(qfs->s1);
  stack_free(qfs->s2);
  free(qfs);
  return;
}

/*
 * This function should indicate whether a given queue-from-stacks is currently
 * empty.  Specifically, it should return 1 if the specified queue-from-stacks
 * is empty (i.e. contains no elements) and 0 otherwise.
 *
 * Params:
 *   qfs - the queue-from-stacks whose emptiness is being questioned.  May not
 *     be NULL.
 */
int queue_from_stacks_isempty(struct queue_from_stacks* qfs) {
  assert(qfs);
  if(stack_isempty(qfs->s1)&&stack_isempty(qfs->s2)){
    return 1;
  }
  return 0;
}

/*
 * This function should enqueue a new value into a given queue-from-stacks.
 * The value to be enqueued is specified as a void pointer.
 *
 * Params:
 *   qfs - the queue-from-stacks into which a value is to be enqueued.  May not
 *     be NULL.
 *   val - the value to be enqueued.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void queue_from_stacks_enqueue(struct queue_from_stacks* qfs, void* val) {
  assert(qfs);
  stack_push(qfs->s1, val);
  return;
}

/*
 * This function should return the value stored at the front of a given
 * queue-from-stacks *without* removing that value.
 *
 * Params:
 *   qfs - the queue-from-stacks from which to query the front value.  May not be NULL.
 */
void* queue_from_stacks_front(struct queue_from_stacks* qfs) {
  assert(qfs);
  void* new_val;
  //if  s1 and s2 all is NULL,it means is emputy
  if(stack_isempty(qfs->s1)&&stack_isempty(qfs->s2)){
    assert(0);
  }else if(stack_isempty(qfs->s2)){
      //when s2 is emputy, means 可以把s1入栈数据放入出栈s2中
      while(stack_isempty(qfs->s1)==0){
            // when it ==0, means s1 is still have value need push in s2

            new_val = stack_pop(qfs->s1);//得到栈顶的值
            stack_push(qfs->s2,new_val);//放入s2栈中
      }
  }
  //when it finsh, it means s1 all valude is push in s2,so we need get s2 top
  return stack_top(qfs->s2);
}

/*
 * This function should dequeue a value from a given queue-from-stacks and
 * return the dequeued value.
 *
 * Params:
 *   qfs - the queue-from-stacks from which a value is to be dequeued.  May not
 *     be NULL.
 *
 * Return:
 *   This function should return the value that was dequeued.
 */
void* queue_from_stacks_dequeue(struct queue_from_stacks* qfs) {
  assert(qfs);
  void* new_top;
  if(stack_isempty(qfs->s2)){
    new_top = queue_from_stacks_front(qfs);
  }else{
    new_top = stack_top(qfs->s2);
  }

  return stack_pop(qfs->s2);
}

/*
 * This file contains a simple implementation of a dynamic array.  See the
 * documentation below for more information on the individual functions in
 * this implementation.
 */

#include <stdlib.h>
#include <assert.h>

#include "dynarray.h"

/*
 * This structure is used to represent a single dynamic array.
 */
 //因为需要时间复杂度为O(1),所以不能使用循环，并且需要一个指针“逻辑第一个数值”的下标
 //添加一个logi_start为指针指向第一个存入数值
struct dynarray {
  void** data;
  int size;
  int capacity;
  int logi_start;
};

#define DYNARRAY_INIT_CAPACITY 4

/*
 * This function allocates and initializes a new, empty dynamic array and
 * returns a pointer to it.
 */
struct dynarray* dynarray_create() {
  struct dynarray* da = malloc(sizeof(struct dynarray));
  assert(da);

  da->data = malloc(DYNARRAY_INIT_CAPACITY * sizeof(void*));
  assert(da->data);
  da->size = 0;
  da->capacity = DYNARRAY_INIT_CAPACITY;
  da->logi_start = 0;
  return da;
}

/*
 * This function frees the memory associated with a dynamic array.  Freeing
 * any memory associated with values stored in the array is the responsibility
 * of the caller.
 *
 * Params:
 *   da - the dynamic array to be destroyed.  May not be NULL.
 */
void dynarray_free(struct dynarray* da) {
  assert(da);
  free(da->data);
  free(da);
}

/*
 * This function returns the size of a given dynamic array (i.e. the number of
 * elements stored in it, not the capacity).
 */
int dynarray_size(struct dynarray* da) {
  assert(da);
  return da->size;
}


/*
 * Auxilliary function to perform a resize on a dynamic array's underlying
 * storage array.
 */
void _dynarray_resize(struct dynarray* da, int new_capacity) {
  assert(new_capacity > da->size);

  /*
   * Allocate space for the new array.
   */
  void** new_data = malloc(new_capacity * sizeof(void*));
  assert(new_data);

  /*
   * Copy data from the old array to the new one.
   */
  for (int i = 0; i < da->size; i++) {
    new_data[i] = da->data[i];
  }

  /*
   * Put the new array into the dynarray struct.
   */
  free(da->data);
  da->data = new_data;
  da->capacity = new_capacity;
}

/*
 * This function inserts a new value to a given dynamic array.  The new element
 * is always inserted at the *end* of the array.
 *
 * Params:
 *   da - the dynamic array into which to insert an element.  May not be NULL.
 *   val - the value to be inserted.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void dynarray_insert(struct dynarray* da,void* val) {
  assert(da);

  /*
   * Make sure we have enough space for the new element.  Resize if needed.
   */
  if (da->size == da->capacity) {
    _dynarray_resize(da, 2 * da->capacity);
  }

  /*
   * Put the new element at the end of the array.
   */
  da->data[da->size] = val;
  da->size++;
}

/*
 * This function removes an element at a specified index from a dynamic array.
 * All existing elements following the specified index are moved forward to
 * fill in the gap left by the removed element.
 *
 * Params:
 *   da - the dynamic array from which to remove an element.  May not be NULL.
 *   idx - the index of the element to be removed.  The value of `idx` must be
 *     between 0 (inclusive) and n (exclusive), where n is the number of
 *     elements stored in the array.
 */
void dynarray_remove(struct dynarray* da, int idx) {
  assert(da);
  assert(idx < da->size && idx >= 0);

  /*
   * Move all elements behind the one being removed forward one index,
   * overwriting the element to be removed in the process.
   */
   for (int i = idx; i < da->size - 1; i++) {
     da->data[i] = da->data[i+1];
   }
  //da->data[idx]=NULL;
  da->size--;
}

/*
 * This function returns the value of an existing element in a dynamic array.
 *
 * Params:
 *   da - the dynamic array from which to get a value.  May not be NULL.
 *   idx - the index of the element whose value should be returned.  The value
 *     of `idx` must be between 0 (inclusive) and n (exclusive), where n is the
 *     number of elements stored in the array.
 */
void* dynarray_get(struct dynarray* da, int idx) {
  assert(da);
  assert(idx < da->size && idx >= 0);

  return da->data[idx];
}

/*
 * This function updates (i.e. overwrites) the value of an existing element in
 * a dynamic array.
 *
 * Params:
 *   da - the dynamic array in which to set a value.  May not be NULL.
 *   idx - the index of the element whose value should be updated.  The value
 *     of `idx` must be between 0 (inclusive) and n (exclusive), where n is the
 *     number of elements stored in the array.
 *   val - the new value to be set.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void dynarray_set(struct dynarray* da, int idx, void* val) {
  assert(da);
  assert(idx < da->size && idx >= 0);

  da->data[idx] = val;
}

//for isempty function ,return 1 or 0;
int dynarray_isempty(struct dynarray* da){
  assert(da);
  return da->size==0;
}
//get the size compare with capa
int dynarray_capa_check(struct dynarray* da){
  if (da->size >= da->capacity){
    return 1;
  }else {
    return 0;
  }
}

int dynarray_get_size(struct dynarray* da){
  return da->size;
}

int dynarray_get_capa(struct dynarray* da){
  return da->capacity;
}

//自写添加函数enqueue
void dynarray_enqueue(struct dynarray* da, void* val){

  //判断是否需要扩容，如果需要则扩容,并恩照逻辑开始位置赋予进新的数组
  //此处关键是(da->logi_start+i)%da->capacity语句
  if(da->size == da->capacity){
    int new_capa = 2 * da->capacity;
    void** new_da = malloc(new_capa * sizeof(void*));

    for(int i=0;i<da->capacity;i++){
      int phy = (da->logi_start+i)%da->capacity;
      new_da[i]=da->data[phy];

    }
    //因为扩容，所以需要free旧的动态数组，并将新的扩容数组赋值给da指针
    free(da->data);
    da->data = new_da;
    da->logi_start = 0;
    da->capacity = new_capa;
  }
  //如果不需要扩容，则需要找到在此数组中何处添加新val；
  //关键语句是(da->logi_start+ da->size)%da->capacity
  int new_address=(da->logi_start+ da->size)%da->capacity;
  da->data[new_address]= val;
  da->size++;
  //因为添加元素，所以需要size++
}
//得到第一个入队的元素的值
void* dynarray_front(struct dynarray* da){
    return da->data[da->logi_start];
}
//需要删除队列的第一个入队元素并输出该值，所以将第一个入队的位置处赋值NULL
void* dynarray_dequeue(struct dynarray* da){
  void* star_val = da->data[da->logi_start];
  da->data[da->logi_start]=NULL;
  //因为删除了第一个入队元素，所以下标后移并size-1；
  da->logi_start++;
  da->size--;
  return star_val;
}

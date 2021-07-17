/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name:Tenghuan Li
 * Email:liten@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "stack.h"
#include "list.h"
/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
 struct bst* bst_create() {
   struct bst* new_bst = malloc(sizeof(struct bst));
   assert(new_bst);
   new_bst->root = NULL;
 //  new_bst->root = malloc(sizeof(struct bst_node));
 //  new_bst->root->right=NULL;
 //  new_bst->root->left=NULL;
 //  struct bst_node* new_node = malloc(sizeof(struct bst_node));
 // new_node->left = NULL;
 // new_node->right = NULL;
 // new_bst->root = new_node;

   return new_bst;
 }

 /*
  * This function should free the memory associated with a BST.  While this
  * function should up all memory used in the BST itself, it should not free
  * any memory allocated to the pointer values stored in the BST.  This is the
  * responsibility of the caller.
  *
  * Params:
  *   bst - the BST to be destroyed.  May not be NULL.
  */
 void bst_free(struct bst* bst) {
 //  assert(bst);
   if(bst->root!=NULL){
     bst_node_free(bst->root);
   }
 //  free(bst->root);
   free(bst);
   return;
 }

 void* bst_node_free(struct bst_node* bst_node){
   if(bst_node->left!=NULL){
       bst_node->left = bst_node_free(bst_node->left);
     }
     if(bst_node->right!=NULL){
       bst_node->right = bst_node_free(bst_node->right);
     }
       free(bst_node);
       return NULL;
   // if(bst_node==NULL){
   //   return;
   // }else{
   //   struct bst_node* bst_left = bst_node->left;
   //   struct bst_node* bst_right = bst_node->right;
   //   bst_node->left = NULL;
   //   bst_node->right = NULL;
   //   free(bst_node);
   //   bst_node_free(bst_left);
   //   bst_node_free(bst_right);
   // }
 }
 /*
  * This function should return the total number of elements stored in a given
  * BST.
  *
  * Params:
  *   bst - the BST whose elements are to be counted.  May not be NULL.
  */
 int bst_size(struct bst* bst) {
   assert(bst);
   int a = 0;
   bst_node_size(bst->root,&a);
   return a;
 }

 void bst_node_size(struct bst_node* bst_node, int* size){
   //é™æ€å˜é‡ï¼šåªèˆ’é€‚åŒ–ä¸€æ¬¡ï¼›
 //  static int size = 0;
   if(bst_node){
 //    if( bst_node->left == NULL && bst_node->right == NULL ){

 //    }
     bst_node_size(bst_node->left,size);
     bst_node_size(bst_node->right,size);
     (*size)++;
   }
   return;
 }
 /*
  * This function should insert a new key/value pair into the BST.  The key
  * should be used to order the key/value pair with respect to the other data
  * stored in the BST.  The value should be stored along with the key, once the
  * right location in the tree is found.
  *
  * Params:
  *   bst - the BST into which a new key/value pair is to be inserted.  May not
  *     be NULL.
  *   key - an integer value that should be used to order the key/value pair
  *     being inserted with respect to the other data in the BST.
  *   value - the value being inserted into the BST.  This should be stored in
  *     the BST alongside the key.  Note that this parameter has type void*,
  *     which means that a pointer of any type can be passed.
  */
 void bst_insert(struct bst* bst, int key, void* value) {
   assert(bst);

   bst->root = bst_node_insert(bst->root,key,value);
   return;
 }

 void* bst_node_insert(struct bst_node* bst_node, int key, void* value){

   if(bst_node==NULL){
     struct bst_node* new_bst = malloc(sizeof(struct bst_node));
     new_bst->key = key;
     new_bst->value = value;
     new_bst->left = NULL;
     new_bst->right = NULL;
     //bst_node = new_bst;
     //new_bst = NULL;
     //free(new_bst);
     //bst_node = bst_node_creat(key,value);
     return new_bst;
   }
   if(bst_node->key > key){
     bst_node->left = bst_node_insert(bst_node->left,key,value);
   }else{
     bst_node->right = bst_node_insert(bst_node->right,key,value);
   }
   return bst_node;
 }

 /*
  * This function should remove a key/value pair with a specified key from a
  * given BST.  If multiple values with the same key exist in the tree, this
  * function should remove the first one it encounters (i.e. the one closest to
  * the root of the tree).
  *
  * Params:
  *   bst - the BST from which a key/value pair is to be removed.  May not
  *     be NULL.
  *   key - the key of the key/value pair to be removed from the BST.
  */
 void bst_remove(struct bst* bst, int key) {
 // assert(bst);
   // bst->root = bst_node_remove(bst->root,key);
   //return;

   struct bst_node* pare;//parent node
   struct bst_node* next;
   next = bst->root;
   pare = next;
   //find where is the node's key = key
   while(key!=next->key){
     pare = next;
     if(key >= next->key){
       next = next->right;
     }else{
       next = next->left;
     }
   }
   //for now we find the node,
   //first: the node don't find the children
   if(next->left ==NULL && next->right ==NULL){
     if(key < next->key){
       pare->right = NULL;
     }else{
       pare->right = NULL;
     }
   }else{
     //have one side child node for it
     if(next->left ==NULL || next->right ==NULL){
       if(next->right = NULL){
         if(key >= next->key){
           pare->right = next->left;
         }else{
           pare->left = next->left;
         }
       }else{
         if(key >= next->key){
           pare->left = next->right;
         }else{
           pare->right = next->right;
         }
       }
     }else{
       //this node have two childe node.
       //we need find the right tree small node
       struct bst_node* new_one = next->right;
       struct bst_node* new_two = new_one;
       //find the last right child it must be the small one.
       while(new_one->left){
         new_two = new_one;
         new_one = new_one->left;
       }
       new_one->left = next->left;
       if(next == bst->root){
         bst->root = new_two;
       }else{
         if(key>=pare->key){
           pare->right = new_two;
         }else{
           pare->left = new_two;
         }
       }
       if(new_two != next->right){
         new_two->left = new_one->right;
         new_one->right = next->right;
       }
     }
   }
   free(next);
   return;
 }

 // void* bst_node_remove(struct bst_node* bst_node, int key){
 //  if(bst_node == NULL){
   // this mean don't find key in the tree.
 //   return NULL;
 // }else if(bst_node->key > key){
 //   bst_node->left = bst_node_remove(bst_node->left,key);
 //   return bst_node;
 // }else if(bst_node->key < key){
 //   bst_node->right = bst_node_remove(bst_node->right,key);
 //   return bst_node;
 // }else {
 //   if(bst_node->left!=NULL && bst_node->right!=NULL){
 //     //this mean we find key and have 2 children or more than,
 //     struct bst_node* new = bst_node->right;
 //     struct bst_node* old = bst_node;
 //     if(new->left==NULL){
 //       new = old->left;
 //       old->key = new->key;
 //       old->value = new->value;
 //       new->right = old->right;
 //       free(old);
 //       return new;
 //     }else{
 //       while(new->left!=NULL){
 //         old = new;
 //         new = new->left;
 //       }
 //       bst_node->key = new->key;
 //       bst_node->value = new->value;
 //       old->left = new->right;
 //       bst_node->right = bst_node_remove(bst_node->right,new->key);
 //     }
 //     return bst_node;
 //   }else if(bst_node->left != NULL){
 //     // if only have left child,
 //     struct bst_node* left = bst_node->left;
 //     free(bst_node);
 //     return left;
 //   }else if(bst_node->right !=NULL){
 //     // if only have right child,
 //     struct bst_node* left = bst_node->left;
 //     free(bst_node);
 //     return left;
 //   }else{
 //     //this have no child, so just free it.
 //     free(bst_node);
 //     return NULL;
 //   }
 // }
 //   return;
 // }
 /*
  * This function should return the value associated with a specified key in a
  * given BST.  If multiple values with the same key exist in the tree, this
  * function should return the first one it encounters (i.e. the one closest to
  * the root of the tree).  If the BST does not contain the specified key, this
  * function should return NULL.
  *
  * Params:
  *   bst - the BST from which a key/value pair is to be removed.  May not
  *     be NULL.
  *   key - the key of the key/value pair whose value is to be returned.
  *
  * Return:
  *   Should return the value associated with the key `key` in `bst` or NULL,
  *   if the key `key` was not found in `bst`.
  */
 void* bst_get(struct bst* bst, int key) {
   assert(bst);

   return bst_node_get(bst->root,key);
 }

 void* bst_node_get(struct bst_node* bst, int key){
 //  assert(bst);
   if(bst){
     if(bst->key==key){
       return bst;
     }
     if(bst->key > key){
       bst_node_get(bst->left,key);
     }else{
       bst_node_get(bst->right,key);
     }
   }else{
     return NULL;
   }

 //  return bst_node_remove(bst,key);
 }


 /*****************************************************************************
  **
  ** BST iterator definition (extra credit only)
  **
  *****************************************************************************/

 /*
  * Structure used to represent a binary search tree iterator.  It contains
  * only a reference to a stack to be used to implement the iterator.
  *
  * You should not modify this structure.
  */
 struct bst_iterator {
   struct stack* stack;
 };

 /*
  * This function should allocate and initialize an iterator over a specified
  * BST and return a pointer to that iterator.
  *
  * Params:
  *   bst - the BST for over which to create an iterator.  May not be NULL.
  */
 struct bst_iterator* bst_iterator_create(struct bst* bst) {
   struct bst_iterator* new_bst = malloc(sizeof(struct bst_iterator));
   new_bst->stack = stack_create();
   bst_insert_iterator(bst->root, new_bst->stack);
   return new_bst;
 }

 void bst_insert_iterator(struct bst_node* bst, struct stack* stack){
   if(bst){
     bst_insert_iterator(bst->right, stack);
     stack_push(stack,bst->value);
     bst_insert_iterator(bst->left, stack);
   }
   return;
 }

 /*
  * This function should free all memory allocated to a given BST iterator.
  * It should NOT free any memory associated with the BST itself.  This is the
  * responsibility of the caller.
  *
  * Params:
  *   iter - the BST iterator to be destroyed.  May not be NULL.
  */
 void bst_iterator_free(struct bst_iterator* iter) {
   assert(iter);
   if(iter->stack){
     stack_free(iter->stack);
   }
 //  free(iter->stack);
   free(iter);
   return;
 }

 /*
  * This function should indicate whether a given BST iterator has more nodes
  * to visit.  It should specifically return 1 (true) if the iterator has at
  * least one more node to visit or 0 (false) if it does not have any more
  * nodes to visit.
  *
  * Param:
  *   iter - the BST iterator to be checked for remaining nodes to visit.  May
  *     not be NULL.
  */



 int bst_iterator_has_next(struct bst_iterator* iter) {
   assert(iter);
   if(stack_isempty(iter->stack)==1){
     return 0;
   }else return 1;
 }

 /*
  * This function should return both the value and key associated with the
  * current node pointed to by the specified BST iterator and advnce the
  * iterator to point to the next node in the BST (in in-order order).
  *
  * Because a function can't return two things, the key associated with the
  * current node should be returned the normal way, while its value should be
  * returned via the argument `value`.  Specifically, the argument `value`
  * is a pointer to a void pointer.  The current BST node's value (a void
  * pointer) should be stored at the address represented by `value` (i.e. by
  * dereferencing `value`).  This will look something like this:
  *
  *   *value = current_node->value;
  *
  * Parameters:
  *   iter - BST iterator.  The key and value associated with this iterator's
  *     current node should be returned, and the iterator should be updated to
  *     point to the next node in the BST (in in-order order).  May not be NULL.
  *   value - pointer at which the current BST node's value should be stored
  *     before this function returns.
  *
  * Return:
  *   This function should return the key associated with the current BST node
  *   pointed to by `iter`.
  */
 int bst_iterator_next(struct bst_iterator* iter, void** value) {
   // if (value) {
   //   *value = NULL;
   // }
   //because value == key, just different at void* and int.
   struct stack* st = iter->stack;
   *value = stack_pop(st);
   return *((int*)(*value));
 }

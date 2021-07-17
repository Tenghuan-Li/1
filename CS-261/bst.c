/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name:XUMING WU
 * Email:wuxum@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>

#include "bst.h"

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
  struct bst* bst = malloc(sizeof(struct bst));
  bst->root = NULL;
  return bst;
}

void bst_node_free(struct bst_node* bn){
  if (bn == NULL) {
    return;
  }
  else{
  struct bst_node* bl = bn->left;
  struct bst_node* br = bn->right;
  bn->left = NULL;
  bn->right = NULL;
  free(bn);
  bst_node_free(bl);
  bst_node_free(br);
  }
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
  if (bst->root != NULL) {
    bst_node_free(bst->root);
  }
  free(bst);
  return;
}


int count_node(struct bst_node* bn, int count){
  int a = count;
  if (bn) {
    a++;
    a = count_node(bn->left, a);
    a = count_node(bn->right, a);
  }
  return a;
}
/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */
int bst_size(struct bst* bst) {
  int count = 0;
  if (bst->root) {
    count = count_node(bst->root, count);
  }
  return count;
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
  struct bst_node* newnode = malloc(sizeof(struct bst_node));
  newnode->key = key;
  newnode->value = value;
  newnode->left = NULL;
  newnode->right = NULL;

  struct bst_node* curr = bst->root;
  struct bst_node* parent = curr;
  if (bst->root == NULL) {
    bst->root = newnode;
    return;
  }
  while (curr) {
    parent = curr;
    if (key < curr->key) {
      curr = curr->left;
    }
    else{
      curr = curr->right;
    }
  }
  if (key < parent->key) {
    parent->left = newnode;
  }
  else{
    parent->right = newnode;
  }
  return;
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
  struct bst_node* curr = bst->root;
  struct bst_node* parent = curr;
/*find the node*/
  while (key != curr->key) {
    parent = curr;
    if (key < curr->key) {
      curr = curr->left;
    }
    else{
      curr = curr->right;
    }
  }

  if (curr->left == NULL && curr->right == NULL) {
    /*no children*/
    if(key < parent->key){
      parent->left = NULL;
    }
    else{
      parent->right = NULL;
    }
  }
  else if (curr->left == NULL || curr->right == NULL) {
    /*one child*/
    if (curr->left == NULL) {
      if (key < parent->key) {
        parent->left = curr->right;
      }
      else{
        parent->right = curr->right;
      }
    }
    else{
      if (key < parent->key) {
        parent->left = curr->left;
      }
      else{
        parent->right = curr->left;
      }
    }
  }
  else{
    /*two children*/
    /*find the successor*/
    struct bst_node* succ = curr->right;
    struct bst_node* sucp = succ;
    while (succ->left) {
      sucp = succ;
      succ = succ->left;
    }

    succ->left = curr->left;
    if (succ != curr->right) {
      sucp->left = succ->right;
      succ->right = curr->right;
    }

    if (curr == bst->root) {
      bst->root = succ;
    }
    else{
      if (key < parent->key) {
        parent->left = succ;
      }
      else{
        parent->right = succ;
      }
    }

  }

  free(curr);
  return;
}

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
  struct bst_node* curr = bst->root;
  while(curr){
    if (key == curr->key) {
      return curr->value;
    }
    else if(key < curr->key){
      curr = curr->left;
    }
    else{
      curr = curr->right;
    }
  }
  return NULL;
}

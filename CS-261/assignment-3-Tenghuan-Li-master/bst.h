/*
 * This file contains the definition of the interface for the binary search
 * tree you'll implement.  You should not modify anything in this file.  You
 * can find descriptions of the binary search tree functions, including their
 * parameters and their return values, in bst.c.
 */

#ifndef __BST_H
#define __BST_H

/*
 * Structure used to represent a binary search tree.
 */
struct bst;
struct bst_node;
struct stack;
/*
 * Binary search tree interface function prototypes.  Refer to bst.c for
 * documentation about each of these functions.
 */
struct bst* bst_create();
void bst_free(struct bst* bst);
int bst_size(struct bst* bst);
void bst_insert(struct bst* bst, int key, void* value);
void bst_remove(struct bst* bst, int key);
void* bst_get(struct bst* bst, int key);

void* bst_node_free(struct bst_node* bst_node);
void bst_node_size(struct bst_node* bst_node,int* size);
void* bst_node_insert(struct bst_node* bst_node, int key, void* value);
//void* bst_node_remove(struct bst_node* bst, int key);
void* bst_node_get(struct bst_node* bst, int key);
/*
 * Structure used to represent a binary search tree iterator.
 */
struct bst_iterator;

/*
 * Binary search tree iterator interface prototypes.  Refer to bst.c for
 * documentation about each of these functions.
 */
 struct bst_iterator* bst_iterator_create(struct bst* bst);
 void bst_insert_iterator(struct bst_node* bst, struct stack* stack);
 void bst_iterator_free(struct bst_iterator* iter);
 int bst_iterator_has_next(struct bst_iterator* iter);
 int bst_iterator_next(struct bst_iterator* iter, void** value);

#endif

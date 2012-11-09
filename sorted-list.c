/*
 # Big-O Analyis

 All operations run in linear time. Sorted Lists have a linked list of nodes. Insertion
 is a linean operation which moves the first item in the list forward until it is even
 or less than the item before it.

 Creation is a constant-time operation, for obvious reasons. Destuction of SortedLists
 is a linear, recursive operation. Calling delete on the nodes after it, before it
 `free`'s itself.

 # Dealing with Modifications of the Original Sorted List

 When a node is added or removed from the linked list when an iterator is operating on
 it, it either traverses it (if the node is ahead of the current node) or it does not
 (if it is before the current node). This is an expected behavior and is the only one
 that treats data fairly. If one would like an unadultered list that will not change
 while it is being traversed, a mutex lock is a sound place to start.

 If the underlying data of the list is modified, then the behavior is undefined. This
 is an expected result, since the data may also be changed in many equally likely
 scenarios including, but not limited to: viruses, stack overflows from other programs,
 physical damage to the machine, and cosmic rays. I make neither an attempt at
 circumventing such things, nor predicting their probability and accounting for it.

 Cosmic rays.

 */
#include <stdio.h>
#include <stdlib.h>
#include "sorted-list.h"

struct SortedList* SLCreate(CompareFuncT cf) {
  struct SortedList* sl = malloc(sizeof(struct SortedList));
  sl->cf = cf;
  sl->head = NULL;
  sl->size = 0;

  return sl;
}

void destroy_node(struct Node * node) {
  free(node->data);
  free(node);
}

void destroy_nodes(struct Node * node) {
  if (!node) return;

  if (!node->next) {
    destroy_node(node);
  } else {
    destroy_nodes(node->next);
    destroy_node(node);
  }
}

void clear_nodes(struct Node * node) {
  if (!node) return;

  if (!node->next) {
    free(node);
  } else {
    clear_nodes(node->next);
    free(node);
  }
}

void SLDestroy(struct SortedList* list) {
  destroy_nodes(list->head);
  free(list);
}

void SLClear(struct SortedList * list) {
  clear_nodes(list->head);
  free(list);
}

void swap_nodes(struct Node * a, struct Node * b) {
  void * temp_data = a->data;
  a->data = b->data;
  b->data = temp_data;
}

struct Node * sort_nodes(struct Node * head, int (*cf)(void *, void *)) {
  /* bubble sort the first element */
  struct Node * first = head, * target = head->next;

  while (target && cf(first->data, target->data) <= 0) {
    swap_nodes(first, target);
    target = target->next;
    first = first->next;
  }

  return head;
}

/* sets the head of the list to be the sorted linked list, returns the truthy ness of
   the assignment
 */
int sort_list(struct SortedList * list) {
  return !!(list->head = sort_nodes(list->head, list->cf));
}

int SLInsert(struct SortedList* list, void *newObj) {
  struct Node * new = malloc(sizeof(struct Node));

  new->data = newObj;
  new->next = list->head;

  list->head = new;
  list->size += 1;

  return sort_list(list);
}

int SLInsertUnique(struct SortedList * list, void *newObj) {
  struct SortedListIterator * iter = SLCreateIterator(list);
  struct Node * new = NULL;
  int should_add = 1;
  void * item = NULL;

  while ((item = SLNextItem(iter))) {
    if (list->cf(item, newObj) == 0) {
      should_add = 0;
      break;
    }
  }

  SLDestroyIterator(iter);

  if (should_add) {
    new = malloc(sizeof(struct Node));
    new->data = newObj;
    new->next = list->head;

    list->head = new;
    list->size += 1;

    return sort_list(list);
  } else {
    return 0;
  }
}

int SLRemove(struct SortedList* list, void *newObj) {
  struct Node * prev_node = NULL, * curr_node = list->head;

  while (curr_node && list->cf(curr_node->data, newObj) != 0) {
    prev_node = curr_node;
    curr_node = curr_node->next;
  }

  if (curr_node) {
    prev_node->next = curr_node->next;
    destroy_node(curr_node);

    sort_list(list);

    list->size -= 1;

    return 1;
  } else if (prev_node) {
    destroy_node(prev_node);
    list->head = NULL;
    return 1;
  } else {
    return 0;
  }
}



struct SortedListIterator* SLCreateIterator(struct SortedList* list) {
  struct SortedListIterator * sli = malloc(sizeof(struct SortedListIterator));

  sli->curr_node = list->head;

  return sli;
}

void SLDestroyIterator(struct SortedListIterator* iter) {
  free(iter);
}

void *SLNextItem(struct SortedListIterator* iter) {
  void * data;

  if (iter->curr_node) {
    data = iter->curr_node->data;
    iter->curr_node = iter->curr_node->next;
    return data;
  } else {
    return NULL;
  }
}

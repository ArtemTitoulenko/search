#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>

struct Node {
  void * data;
  struct Node * next;
};

struct SortedList {
  int (*cf)(void *, void *);
  int size;
  struct Node * head;
};

struct SortedListIterator {
  struct Node * curr_node;
};

typedef int (*CompareFuncT)(void *, void *);


struct SortedList* SLCreate(CompareFuncT cf);
void SLDestroy(struct SortedList* list);
void SLClear(struct SortedList * list);

int SLInsert(struct SortedList* list, void *newObj);
int SLInsertUnique(struct SortedList * list, void * newObj);
int SLRemove(struct SortedList* list, void *newObj);

struct SortedListIterator* SLCreateIterator(struct SortedList* list);
void SLDestroyIterator(struct SortedListIterator* iter);

void * SLNextItem(struct SortedListIterator* iter);

#endif

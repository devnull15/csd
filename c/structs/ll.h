#ifndef LL_H
#define LL_H
#include "node.h"
//definitions to chose sorting algorithm
#define LL_MERGESORT 0

typedef struct ll {
  node_t * top;
  int size;
} ll_t;

ll_t* ll_create(int *data, int size);

void ll_print(ll_t *ll);

int ll_get(ll_t *ll, int i);

void ll_sort(ll_t *ll, int algo);

int *ll_toarray(ll_t *ll);

void ll_del(ll_t *ll, int i);

void ll_insert(ll_t *ll, int i);

void ll_delall(ll_t *ll);

void ll_destroy(ll_t *ll);

#endif

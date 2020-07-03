#ifndef CLL_H
#define CLL_H
#include "node.h"
//definitions to chose sorting algorithm
#define CLL_MERGESORT 0

typedef struct cll {
  node_t * top;
  int size;
} cll_t;

cll_t* cll_create(int *data, int size);

void cll_print(cll_t *cll);

void cll_preintrev(cll_t *cll);

int cll_get(cll_t *cll, int i);

void cll_sort(cll_t *cll, int algo);

int *cll_toarray(cll_t *cll);

void cll_del(cll_t *cll, int i);

void cll_insert(cll_t *cll, int item, int i);

void cll_delall(cll_t *cll);

void cll_destroy(cll_t *cll);


#endif

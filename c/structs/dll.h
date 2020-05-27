#ifndef DLL_H
#define DLL_H
#include "node.h"
//definitions to chose sorting algorithm
#define DLL_MERGESORT 0

typedef struct dll {
  node_t * top;
  node_t * bot;
  int size;
} dll_t;

dll_t* dll_create(int *data, int size);

void dll_print(dll_t *dll);

void dll_preintrev(dll_t *dll);

int dll_get(dll_t *dll, int i);

void dll_sort(dll_t *dll, int algo);

int *dll_toarray(dll_t *dll);

void dll_del(dll_t *dll, int i);

void dll_insert(dll_t *dll, int item, int i);

void dll_delall(dll_t *dll);

void dll_destroy(dll_t *dll);

#endif

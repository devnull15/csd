//#include <limits.h>
#include <stdio.h> 
#include <stdlib.h>

#include "node.h"
#include "cll.h"

static node_t *makeNode(int data, node_t *next, node_t *prev) {
  node_t *node = malloc(sizeof(struct node));
  node->data = data;
  node->next = next;
  node->prev = prev;
  return node;
}

cll_t* cll_create(int *data, int size) {
  cll_t *list = malloc(sizeof(struct cll));
  list->size = size;
  
  node_t *cur = makeNode(data[0],NULL,NULL);
  list->top = cur;
  
  node_t *next;
  
  for(int i=1; i<size;i++) {
    next = makeNode(data[i],NULL,cur);
    cur->next = next;
    cur = next;
  }
  
  next->next = list->top;
  list->top->prev = next;

  return list;
}

void cll_print(cll_t *cll) {
  node_t *curr = cll->top;
  for(int i=0;i<cll->size;i++) {
    printf("Node %i value: %i\n", i, curr->data);
    curr = curr->next;
  }
  return;
}

static void cll_print4ever(cll_t *cll) {
  node_t *curr = cll->top;
  while(curr!=NULL) {
    printf("Node value: %i\n", curr->data);
    curr = curr->next;
  }
  return;  
}


// Could make this O(n/2)
int cll_get(cll_t *cll, int i) {
  node_t *curr = cll->top;
  while(i <= cll->size) {
    if(i==0) { return curr->data; }
    --i;
    curr = curr->next;
  }

  return EXIT_FAILURE;
}

static void split(node_t *top, node_t **l, node_t **r, int size) {
  *l = top;
  node_t* temp = *l;
  for(int i=0; i<(size-1)/2; i++) {
    temp = temp->next;
  }
  *r = temp->next;
  temp->next->prev = NULL;
  temp->next = NULL;
  return;
}

node_t *merge(node_t *l, node_t *r) {
  //base case, if right side is NULL return left side and vice versa
  if(r==NULL) { return l; }
  if(l==NULL) { return r; }
  
  node_t *ret;
  if(l->data <= r->data) {
    ret = l;
    ret->next = merge(l->next,r);
  }
  else {
    ret = r;
    ret->next = merge(l,r->next);
    }

  ret->next->prev = ret;
  
  return ret;
}

static node_t *mergeSortHelper(node_t *top, int size) {
  node_t *l,*r;

  //base case node==NULL or next node==NULL; just return
  if(top==NULL || top->next==NULL) { return top; }
  
  //split list recursively
  split(top,&l,&r,size);

  //recurse using this funciton to keep splitting
  l = mergeSortHelper(l,(size+1)/2);
  r = mergeSortHelper(r,size/2);
  
  //merge the lists
  l = merge(l,r);

  return l;
}

static void mergeSort(cll_t *cll) {
  node_t *cur = cll->top;

  //have to un-circularize the list before sorting
  for(int i=1; i<cll->size; i++) {
    cur = cur->next;
  }
  cur->next = NULL;

  //sort
  cll->top = mergeSortHelper(cll->top,cll->size);

  //re-circularize list
  while(cur->next!=NULL) { cur = cur->next; }
  cur->next = cll->top;
  return;
}

void cll_sort(cll_t *cll,int algo) {
  //this is a list of the sorting algorithms available
  void (*sort[])(cll_t*) = {mergeSort};
  (*sort[algo])(cll);
  return;
}

int *cll_toarray(cll_t *cll) {
  int *ret = (int*)malloc(cll->size*sizeof(int));
  node_t *curr = cll->top;

  for(int i = 0; i<cll->size; i++) {
    ret[i] = curr->data;
    curr = curr->next;
  }
  return ret;
}

void cll_del(cll_t *cll, int i) {
  node_t *cur = cll->top;
  node_t *prev = NULL;

  //delete top node
  if(i==0) {
    cll->top = cur->next;
    cur->prev = NULL;
  }
  else {
    //all other cases
    while(cur!=NULL && i>0) {
      prev = cur;
      cur = cur->next;
      i--;
    }
    if(cur==NULL){ fprintf(stderr,"cll_del: index %i is out of bounds\n", i); return; }
    else {
      prev->next = cur->next;
      cur->next->prev = prev;
    }
  }
  
  free(cur);
  cll->size--;

  return;
}

void cll_insert(cll_t *cll, int item, int i) {
  node_t *cur = cll->top;
  node_t *prev = NULL;

  while(cur!=NULL && i>0) {
    prev = cur; 
    cur = cur->next;
    i--;
  }
  
  if(cur==NULL){ fprintf(stderr,"cll_insert: index %i is out of bounds\n", i); }
  else {
    if(prev==NULL){  //replace the first node
      cll->top = makeNode(item,cur,NULL);
      cur->prev = cll->top;
    }
    else {  // replace anything else
      prev->next = makeNode(item,cur,prev);
      cur->prev = prev->next;
    }
    cll->size++;
  }
  return;
}


void cll_delall(cll_t *cll) {
  while(cll->size != 0) {
    cll_del(cll,0);
  }
  return;
}

void cll_destroy(cll_t *cll) {
  cll_delall(cll);
  free(cll);
  return;
}

int main() {
  printf("Making linked list of 0-9 and -1...\n");
  int arr[] = {0,1,2,3,4,5,6,7,8,9,-1};
  int size = sizeof(arr)/sizeof(int);
  cll_t *list = cll_create(arr,size);
  cll_print(list);

  int i=5;
  printf("cll_get test. Getting item %i value should be %i\n", i, arr[i]);
  printf("cll_get: cll[%i] = %i\n", i, cll_get(list,i));

  i=20;
  printf("cll_get test - out of bounds. Getting item %i value should be %i\n", i, EXIT_FAILURE);
  printf("cll_get: cll[%i] = %i\n", i, cll_get(list,i));

  i=5;
  printf("cll_toarray test. Getting item %i value should be %i\n", i, arr[i]);
  printf("cll_toarray: ret[%i]=%i\n",i,cll_toarray(list)[i]);

  printf("Sorting list...\n");
  cll_sort(list,CLL_MERGESORT);
  cll_print(list);

  int item = 2015;
  i=0;
  printf("cll_insert test. Inserting %i into index %i\n", item, i);
  cll_insert(list,item,i);
  cll_print(list);

  item = 2015;
  i=15;
  printf("cll_insert test circular style. Inserting %i into index %i\n", item, i);
  cll_insert(list,item,i);
  cll_print(list);

  
  i=0;
  printf("cll_del test. Deleting index %i\n", i);
  cll_del(list,i);
  cll_print(list);

  i=15;
  printf("cll_del test circular style. Deleting index %i\n", i);
  cll_del(list,i);
  cll_print(list);

  printf("cll_delall test.\n");
  cll_delall(list);
  printf("Printing list, nothing should print:\n");
  cll_print(list);

  printf("cll_destroy test.\n");
  cll_destroy(list);
  printf("It works?\n");
  
  //cll_print4ever(list);
  
  return EXIT_SUCCESS;
}

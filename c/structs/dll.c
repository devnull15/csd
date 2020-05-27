//#include <limits.h>
#include <stdio.h> 
#include <stdlib.h>

#include "node.h"
#include "dll.h"

static node_t *makeNode(int data, node_t *next, node_t *prev) {
  node_t *node = malloc(sizeof(struct node));
  node->data = data;
  node->next = next;
  node->prev = prev;
  return node;
}

dll_t* dll_create(int *data, int size) {
  dll_t *list = malloc(sizeof(struct dll));
  list->size = size;
  
  node_t *cur = makeNode(data[0],NULL,NULL);
  list->top = cur;
  
  node_t *next;
  
  for(int i=1; i<size;i++) {
    next = makeNode(data[i],NULL,cur);
    cur->next = next;
    cur = next;
  }
  
  next->next = NULL;
  list->bot = next;

  return list;
}

void dll_print(dll_t *dll) {
  node_t *curr = dll->top;
  for(int i=0;i<dll->size;i++) {
    printf("Node %i value: %i\n", i, curr->data);
    curr = curr->next;
  }
  return;
}

void dll_printrev(dll_t *dll) {
  node_t *curr = dll->bot;
  for(int i=0;i<dll->size;i++) {
    printf("Node %i value: %i\n", dll->size-i-1, curr->data);
    curr = curr->prev;
  }
  return;
}

// Could make this O(n/2)
int dll_get(dll_t *dll, int i) {
  node_t *curr = dll->top;
  while(curr != NULL) {
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

static void mergeSort(dll_t *dll) {
  dll->top = mergeSortHelper(dll->top,dll->size);
  node_t *cur = dll->top;
  while(cur->next!=NULL) { cur = cur->next; }
  dll->bot = cur;
  return;
}

void dll_sort(dll_t *dll,int algo) {
  //this is a list of the sorting algorithms available
  void (*sort[])(dll_t*) = {mergeSort};
  (*sort[algo])(dll);
  return;
}

int *dll_toarray(dll_t *dll) {
  int *ret = (int*)malloc(dll->size*sizeof(int));
  node_t *curr = dll->top;

  for(int i = 0; i<dll->size; i++) {
    ret[i] = curr->data;
    curr = curr->next;
  }
  return ret;
}

void dll_del(dll_t *dll, int i) {
  node_t *cur = dll->top;
  node_t *prev = NULL;

  //delete top node
  if(i==0) {
    dll->top = cur->next;
    cur->prev = NULL;
  }
  else {
    //all other cases
    while(cur!=NULL && i>0) {
      prev = cur;
      cur = cur->next;
      i--;
    }
    if(cur==NULL){ fprintf(stderr,"dll_del: index %i is out of bounds\n", i); return; }
    else {
      prev->next = cur->next;
      cur->next->prev = prev;
    }
  }
  
  free(cur);
  dll->size--;

  return;
}

void dll_insert(dll_t *dll, int item, int i) {
  node_t *cur = dll->top;
  node_t *prev = NULL;

  while(cur!=NULL && i>0) {
    prev = cur; 
    cur = cur->next;
    i--;
  }
  
  if(cur==NULL){ fprintf(stderr,"dll_insert: index %i is out of bounds\n", i); }
  else {
    if(prev==NULL){  //replace the first node
      dll->top = makeNode(item,cur,NULL);
      cur->prev = dll->top;
    }
    else {  // replace anything else
      prev->next = makeNode(item,cur,prev);
      cur->prev = prev->next;
    }
    dll->size++;
  }
  return;
}


void dll_delall(dll_t *dll) {
  while(dll->size != 0) {
    dll_del(dll,0);
  }
  return;
}

void dll_destroy(dll_t *dll) {
  dll_delall(dll);
  free(dll);
  return;
}

int main() {
  printf("Making linked list of 0-9...\n");
  int arr[] = {0,1,2,3,4,5,6,7,8,9,-1};
  int size = sizeof(arr)/sizeof(int);
  dll_t *list = dll_create(arr,size);
  dll_printrev(list);

  int i=5;
  printf("dll_get test. Getting item %i value should be %i\n", i, arr[i]);
  printf("dll_get: dll[%i] = %i\n", i, dll_get(list,i));

  printf("dll_toarray test. Getting item %i value should be %i\n", i, arr[i]);
  printf("dll_toarray: ret[%i]=%i\n",i,dll_toarray(list)[i]);

  printf("Sorting list...\n");
  dll_sort(list,DLL_MERGESORT);
  dll_printrev(list);

  int item = 2015;
  i=0;
  printf("dll_insert test. Inserting %i into index %i\n", item, i);
  dll_insert(list,item,i);
  dll_printrev(list);

  item = -42;
  i=6;
  printf("dll_insert test. Inserting %i into index %i\n", item, i);
  dll_insert(list,item,i);
  dll_printrev(list);
  
  i=0;
  printf("dll_del test. Deleting index %i\n", i);  
  dll_del(list,i);
  dll_printrev(list);

  i=5;
  printf("dll_del test. Deleting index %i\n", i);
  dll_del(list,i);
  dll_printrev(list);

  printf("dll_delall test.\n");
  dll_delall(list);
  printf("Printing list, nothing should print:\n");
  dll_printrev(list);

  printf("dll_destroy test.\n");
  dll_destroy(list);
  printf("It works?\n");
  
  
  return EXIT_SUCCESS;
}

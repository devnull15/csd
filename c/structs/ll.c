//#include <limits.h>
#include <stdio.h> 
#include <stdlib.h>

#include "node.h"
#include "ll.h"

static node_t *makeNode(int data, node_t *next) {
  node_t *node = malloc(sizeof(struct node));
  node->data = data;
  node->next = next;
  return node;
}

ll_t* ll_create(int *data, int size) {
  ll_t *list = malloc(sizeof(struct ll));
  list->size = size;
  //makes the list starting from the last node and going backwards
  node_t *next = makeNode(data[size-1],NULL);
  for(int i=size-2; i>=0;i--) {
    node_t* curr = makeNode(data[i],next);
    next = curr;
  }
  list->top = next;
  return list;
}

void ll_print(ll_t *ll) {
  node_t *curr = ll->top;
  for(int i=0;i<ll->size;i++) {
    printf("Node %i value: %i\n", i, curr->data);
    curr = curr->next;
  }
  return;
}

int ll_get(ll_t *ll, int i) {
  node_t *curr = ll->top;
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
  r = mergeSortHelper(r,(size+1)/2);
  
  //merge the lists
  l = merge(l,r);

  return l;
}

static void mergeSort(ll_t *ll) {
  ll->top = mergeSortHelper(ll->top,ll->size);
  return;
}

void ll_sort(ll_t *ll,int algo) {
  //this is a list of the sorting algorithms available
  void (*sort[])(ll_t*) = {mergeSort};
  (*sort[algo])(ll);
  return;
}

int *ll_toarray(ll_t *ll) {
  int *ret = (int*)malloc(ll->size*sizeof(int));
  node_t *curr = ll->top;
  for(int i = 0; i<ll->size; i++) {
    ret[i] = curr->data;
    curr = curr->next;
  }
  return ret;
}

void ll_insert(ll_t *ll, int item, int i) {

  node_t *curr = ll->top;
  node_t *prev = NULL;
  while(curr!=NULL && i>=0) {
    prev = curr; 
    curr = curr->next;
    i--;
  }
  prev->next = makeNode(item,curr);
  ll->size++;
  return;
}
  
int main() {
  printf("Making linked list of 0-9...\n");
  int arr[] = {0,1,2,3,4,5,6,7,8,9,-1};
  int size = sizeof(arr)/sizeof(int);
  ll_t *list = ll_create(arr,size);
  ll_print(list);

  int i=5;
  printf("ll_get test. Getting item %i value should be %i\n", i, arr[i]);
  printf("ll_get: ll[%i] = %i\n", i, ll_get(list,i));

  printf("ll_toarray test. Getting item %i value should be %i\n", i, arr[i]);
  printf("ll_toarray: ret[%i]=%i\n",i,ll_toarray(list)[i]);

  printf("Sorting list...\n");
  ll_sort(list,LL_MERGESORT);
  ll_print(list);

  int item = 2015;
  printf("ll_insert test. Inserting %i into index %i\n", item, i);
  ll_insert(list,item,i);
  ll_print(list);

  
  return EXIT_SUCCESS;
}

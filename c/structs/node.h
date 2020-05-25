#ifndef NODE_H
#define NODE_H
typedef struct node {
  struct node* next;
  struct node* prev;
  int data;
}node_t;
#endif

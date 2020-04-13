#ifndef STACK_H
#define STACK_H
#include <limits.h>
#define STACK_FULL INT_MAX
#define STACK_EMPTY INT_MIN
  
typedef struct stack { 
  int top; 
  int size; 
  int* stack; 
}stack_t; 
  
stack_t* s_create(int size);

int s_full(stack_t* stack);
  
int s_empty(stack_t* stack);
  
int s_push(stack_t* stack, int item);
  
int s_pop(stack_t* stack);
  
int s_peek(stack_t* stack);

void s_error(int errno);

void s_destroy(stack_t* stack);

#endif

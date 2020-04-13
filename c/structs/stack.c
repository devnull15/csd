#include <limits.h>
#include <stdio.h> 
#include <stdlib.h> 
#include "stack.h"


stack_t* s_create(int size) {
  stack_t *s = (stack_t*)malloc(sizeof(stack_t));
  s->stack = (int*)malloc(size*sizeof(int));
  s->size = size;
  s->top = -1;
  return s;
}

int s_full(stack_t* stack) { 
  if((stack->top+1)==stack->size) { return 1; }
  else { return 0; }
  return -1;
}
  
int s_empty(stack_t* stack) { 
  if(stack->top==-1) { return 1;}
  else { return 0; }
  return -1;
} 
  
int s_push(stack_t* stack, int item) {
  if(s_full(stack)) { return STACK_FULL; }
  else { stack->stack[++(stack->top)] = item; }
  return 0;
} 
  
int s_pop(stack_t* stack) {
  if(s_empty(stack)) { return STACK_EMPTY; }
  else { return stack->stack[(stack->top)--]; }
  return 0;
} 
  
int s_peek(stack_t* stack) { 
  if(s_empty(stack)) { return STACK_EMPTY; } // STACK_EMPTY used as a special signal to indicate empty stack
  return stack->stack[stack->top];
} 

void s_error(int errno) { 
  char *msg;
  if(errno==STACK_FULL) { msg = "stack full"; }
  else if(errno==STACK_EMPTY) { msg = "stack empty"; }
  else { msg = "undefined."; }
  fprintf(stderr, "Error: %s\n", msg);
  return;
}

void s_destroy(stack_t* stack) {
  free(stack);
  //no way to check for a double free, programmer has to make sure they don't do that.
}
  
int main() { 
  int size = 2;
  int p;

  //add what should happen print lines
  stack_t *s = s_create(size);
  printf("Stack created with size %i!\n",size);
  printf("Checking if stack empty:\n");

  if((p=s_peek(s))==STACK_EMPTY) { printf("stack empty.\n"); }
  else { printf("peek: %i\n",p); }
  
  printf("Pushing - should be no error:\n");
  if(s_push(s,1)==STACK_FULL) { s_error(STACK_FULL); }
  printf("Pushing - should be no error:\n");
  if(s_push(s,2)==STACK_FULL) { s_error(STACK_FULL); }
  printf("Pushing - should have error:\n");
  if(s_push(s,3)==STACK_FULL) { s_error(STACK_FULL); }
  printf("Pushing - should have error:\n");
  if(s_push(s,4)==STACK_FULL) { s_error(STACK_FULL); }
  printf("Pushing - should have error:\n");
  if(s_push(s,5)==STACK_FULL) { s_error(STACK_FULL); }
  printf("Peek - top of stack should be 2:\n");
  if((p=s_peek(s))==STACK_EMPTY) { printf("stack empty.\n"); }
  else { printf("peek: %i\n",p); }

  printf("Pop - should be 2:\n");
  if((p=s_pop(s))==STACK_EMPTY) { s_error(STACK_EMPTY); }
  else{ printf("pop: %i\n",p); }
  printf("Pop - should be 1:\n");
  if((p=s_pop(s))==STACK_EMPTY) { s_error(STACK_EMPTY); }
  else{ printf("pop: %i\n",p); }
  printf("Pop - should fail:\n");
  if((p=s_pop(s))==STACK_EMPTY) { s_error(STACK_EMPTY); }
  else{ printf("pop: %i\n",p); }
  printf("Pop - should fail:\n");
  if((p=s_pop(s))==STACK_EMPTY) { s_error(STACK_EMPTY); }
  else{ printf("pop: %i\n",p); }
  printf("Pop - should fail:\n");
  if((p=s_pop(s))==STACK_EMPTY) { s_error(STACK_EMPTY); }
  else{ printf("pop: %i\n",p); }

  printf("Destroy - freeing the stack (no return value to validate)\n");
  s_destroy(s);

  return 0;
}

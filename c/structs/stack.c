#include <limits.h>
#include <stdio.h> 
#include <stdlib.h> 
#define STACK_FULL INT_MAX
#define STACK_EMPTY INT_MIN
  
struct Stack { 
  int top; 
  int size; 
  int* stack; 
}; 
  

struct Stack* create(int size) {
  struct Stack *s = (struct Stack*)malloc(sizeof(struct Stack));
  s->stack = (int*)malloc(size*sizeof(int));
  s->size = size;
  s->top = -1;
  
}

int s_full(struct Stack* stack) { 
  if((stack->top+1)==stack->size) { return 1; }
  else { return 0; }
  return -1;
}
  
int s_empty(struct Stack* stack) { 
  if(stack->top==-1) { return 1;}
  else { return 0; }
  return -1;
} 
  
int s_push(struct Stack* stack, int item) {
  if(s_full(stack)) { return STACK_FULL; }
  else { stack->stack[++(stack->top)] = item; }
  return 0;
} 
  
int s_pop(struct Stack* stack) {
  if(s_empty(stack)) { return STACK_EMPTY; }
  else { return stack->stack[(stack->top)--]; }
  return 0;
} 
  
int s_peek(struct Stack* stack) { 
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
  
int main() { 
  int size = 2;
  int p;

  struct Stack *s = create(size);
  printf("Stack created with size %i!\n",size);
  if((p=s_peek(s))==STACK_EMPTY) { printf("stack empty.\n"); }
  else { printf("peek: %i\n",p); }
  
  if(s_push(s,1)==STACK_FULL) { s_error(STACK_FULL); }
  if(s_push(s,2)==STACK_FULL) { s_error(STACK_FULL); }
  if(s_push(s,3)==STACK_FULL) { s_error(STACK_FULL); }
  if(s_push(s,4)==STACK_FULL) { s_error(STACK_FULL); }
  if(s_push(s,5)==STACK_FULL) { s_error(STACK_FULL); }
  if((p=s_peek(s))==STACK_EMPTY) { printf("stack empty.\n"); }
  else { printf("peek: %i\n",p); }

  if((p=s_pop(s))==STACK_EMPTY) { s_error(STACK_EMPTY); }
  else{ printf("pop: %i\n",p); }
  if((p=s_pop(s))==STACK_EMPTY) { s_error(STACK_EMPTY); }
  else{ printf("pop: %i\n",p); }
  if((p=s_pop(s))==STACK_EMPTY) { s_error(STACK_EMPTY); }
  else{ printf("pop: %i\n",p); }
  if((p=s_pop(s))==STACK_EMPTY) { s_error(STACK_EMPTY); }
  else{ printf("pop: %i\n",p); }
  if((p=s_pop(s))==STACK_EMPTY) { s_error(STACK_EMPTY); }
  else{ printf("pop: %i\n",p); }

} 
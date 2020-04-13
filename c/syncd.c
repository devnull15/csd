#include "structs/stack.h"

// https://www.hackerearth.com/practice/notes/the-make-command-and-makefiles/

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

  return 0;
}

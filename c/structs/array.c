#include <stdlib.h>

int array(){
  char mychar[30] = {0,1,2,3,4,5,6,7,8,9};
  return 0;
}

void malloc_array(int size){
  void *ptr = malloc(size);
  *(char*)ptr = 5;
  return;
}


int main(int argc, char **argv) {
  //array();
  malloc_array(10);
  return 0;
}

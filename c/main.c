#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

//3.1.11
void loops_n_ifs_stuff(){
  return;
}
///3.1.11

// 3.1.8/9/10 - function declarations
int by_value(int);
int* by_ref(int*);
void user_in();
void ptr_fun(int*,int);

// function definitions
int by_value(int val) {
  printf("I got this by value! Its address is now %p and the value is %i\n", &val, val);
  return val;
}

int* by_ref(int* val) {
  printf("I got this by reference! Its address is still %p and the value is %i\n", val, *val);
  return val;
}

void user_in(){
  char buf[10];
  printf("Enter your name, only the first ten characters are taken.\n");
  scanf("%10s",buf);
  char flagged = 0;
  for(int i = 0; i < sizeof(buf);i++) {
    if(isdigit(buf[i])) {
      flagged = 1;
    }
  }
  if(flagged) {
    printf("That's not a real name! Try again.\n");
    user_in();
  }
  else{ 
    printf("Thanks!\n");
  }
  return;
}

void ptr_fun(int* ptr, int val){
  printf("name (mem_addr):value\n");
  printf("ptr (%p):%i\n",ptr,*ptr);
  printf("val (%p):%i\n",&val,val);
  ptr = &val;
  printf("ptr is now: (%p):%i\n",ptr,*ptr);
  int arr[4] = {1,2,3,4};
  printf("arr: (%p):%i\n",arr,*arr);
  printf("arr[1]: (%p):%i\n",arr+1,*(arr+1));
  printf("arr[1]: (%p):%i\n",(void*)arr+sizeof(int),*(int*)((void*)arr+sizeof(int)));

  return;
}
///3.1.8/9/10

// 3.1.4
void print_array() {
  int array[] = {0,1,2,3,4,5,6,7,8,9,10};
  int size = sizeof(array) / sizeof(array[0]);
  printf("array: ");
  for(int j = 0; j < size; j++) {
    printf("%d, ",array[j]);
  }
  printf("\n");
  return;
}

void print_2darray() {
  int array[] = {0,1,2,3,4,5,6,7,8,9,10};
  int size = sizeof(array) / sizeof(array[0]);
  int d2array[size][size];
  printf("2darray:\n");
  for(int j = 0; j < size; j++) {
    for(int i = 0; i < size; i++) {
      d2array[j][i]=array[i];
    }
  }
  for(int j = 0; j < size; j++) {
    for(int i = 0; i < size; i++) {
      printf("%d, ",d2array[j][i]);
    }
    printf("\n");
  }
  return;
}
///3.1.4

// 3.1.5
void maths() {
  int a = 2;
  int b = 5;
  printf("0. a = %i; b = %i\n",a,b);
  printf("1. Postfix Inc/Dec: a++ = %i; a--  = %i; a = %i\n", a++,a--,a);
  printf("2. Prefix Inc/Dec: ++a = %i; --a  = %i; a = %i\n", ++a,--a,a);
  printf("3. */mod : a * a / a %% a = %i\n",a*a/a%a);
  printf("4. +-: a + a - a = %i\n",a+b-a);
 
  //  int result = a++ + --a - a * a / a % a;
  
  
  //  printf("result = %f\n", result);
  return;
}
///3.1.5

// 3.1.7
void file_shenanigans() {
  int buf_size = 255;
  char *fn = "./input.txt";
  FILE *f = fopen(fn,"w+");
  printf("Wrting to file...\n");
  fputs("Hello World!\n",f);
  printf("I'm currently at %ld in the file.\nTime to rewind!\n",ftell(f));
  rewind(f);
  char buf[buf_size];
  fgets(buf,buf_size,f);
  printf("File says: %s",buf);
  struct stat st;
  fstat(fileno(f),&st);
  printf("Here's what fstat says:\n");
  printf("File Size: \t\t%ld bytes\n",st.st_size);
  printf("Number of Links: \t%ld\n",st.st_nlink);
  printf("File inode: \t\t%ld\n",st.st_ino);
  printf("File Permissions: \t");
  printf( (S_ISDIR(st.st_mode)) ? "d" : "-");
  printf( (st.st_mode & S_IRUSR) ? "r" : "-");
  printf( (st.st_mode & S_IWUSR) ? "w" : "-");
  printf( (st.st_mode & S_IXUSR) ? "x" : "-");
  printf( (st.st_mode & S_IRGRP) ? "r" : "-");
  printf( (st.st_mode & S_IWGRP) ? "w" : "-");
  printf( (st.st_mode & S_IXGRP) ? "x" : "-");
  printf( (st.st_mode & S_IROTH) ? "r" : "-");
  printf( (st.st_mode & S_IWOTH) ? "w" : "-");
  printf( (st.st_mode & S_IXOTH) ? "x" : "-");
  printf("\n");
  printf("The file %s a symbolic link\n\n", (S_ISLNK(st.st_mode)) ? "is" : "is not");
  printf("Time to modify and append to the file...\n");
  rewind(f);
  fputs("Hewwo",f);
  rewind(f);
  fgets(buf,buf_size,f);
  printf("File says: %s",buf);

  //blah
  
  fseek(f,0,SEEK_END);
  fputs("Hello again!\n",f);
  rewind(f);
  while (fgets(buf, buf_size, f) != NULL) {
    printf("File says: %s",buf);
  }

  printf("Closing and deleting :(\n");
  fclose(f);
  remove(fn);
  

  return;
}
///3.1.7

int main(int argc, char **argv, char **envp) {
  printf("* 3.1.3 *\n");
  /** !!NOTE!!
      This is being compiled on Linux, but in WINDOWS and DOS there is also:
      typedef uint8_t BYTE;
      typedef uint16_t WORD;
      typedef uint32_t DWORD;
      typedef uint64_t QWORD;
   **/

  short s;
  int i;
  float f;
  char c;
  double d;
  long l;
  printf("short: %ld\n", sizeof(s));
  printf("int: %ld\n", sizeof(i));
  printf("float: %ld\n", sizeof(f));
  printf("char: %ld\n", sizeof(c));
  printf("double: %ld\n", sizeof(d));
  printf("long: %ld\n", sizeof(l));
  printf("* *\n\n");

  printf("* 3.1.4 *\n");
  print_array();
  print_2darray();
  printf("* *\n\n");

  printf("* 3.1.5 *\n");
  maths();
  printf("* *\n\n");

  printf("* 3.1.6 *\n");
  for(int i = 0; i < argc; i++){
    printf("arg %i: %s\n",i,argv[i]);
  }
  int env_size = sizeof(envp)/sizeof(envp[0]);
  for(int i = 0; i < env_size; i++){
    printf("env %i: %s\n",i,envp[i]);
  }    
  printf("* *\n\n");

  printf("* 3.1.7 *\n");
  file_shenanigans();
  printf("* *\n\n");

  printf("* 3.1.8/9/10 *\n");
  int val = 15;
  printf("This is in main, the variable 'val' is at %p and has the value %i\n",&val,val);
  by_value(val);
  by_ref(&val);
  void (*user_in_ptr)() = &user_in;
  (*user_in_ptr)();
  ptr_fun(&val,val);
  printf("* *\n\n");

  printf("* 3.1.11 *\n");
  loops_n_ifs_stuff();
  printf("* *\n\n");
  
  return 0;
}

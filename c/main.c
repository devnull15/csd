#include <stdio.h>
#include <sys/stat.h>


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
  printf("3. */%: a * a / a %% a = %i\n",a*a/a%a);
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
  printf("I'm currently at %i in the file.\nTime to rewind!\n",ftell(f));
  rewind(f);
  char buf[buf_size];
  fgets(buf,buf_size,f);
  printf("File says: %s",buf);
  struct stat st;
  fstat(fileno(f),&st);
  printf("Here's what fstat says:\n");
  printf("File Size: \t\t%d bytes\n",st.st_size);
  printf("Number of Links: \t%d\n",st.st_nlink);
  printf("File inode: \t\t%d\n",st.st_ino);
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
  
  fseek(f,0,SEEK_END);
  fputs("Hello again!\n",f);
  rewind(f);
  fgets(buf,buf_size,f);
  printf("File says: %s",buf);

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
  printf("short: %d\n", sizeof(s));
  printf("int: %d\n", sizeof(i));
  printf("float: %d\n", sizeof(f));
  printf("char: %d\n", sizeof(c));
  printf("double: %d\n", sizeof(d));
  printf("long: %d\n", sizeof(l));
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

  
  return 0;
}

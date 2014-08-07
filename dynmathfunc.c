#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <wait.h>
#include "dynmathfunc.h"

void create_function_source_file(char *expression, char *filename)
{
  char funcsrc[1000];
  FILE * fp;

  sprintf(funcsrc,"#include <math.h>%c" 
                  "double f(double x)%c{%c  return %s;%c}%c", 
	          '\n','\n','\n',expression,'\n','\n');

  fp = fopen(filename, "w");
  fprintf(fp,"%s", funcsrc);
  fclose(fp);
}

int create_library (char *filename_src, char *filename_dst)
{
  char command_str[1000];
  pid_t pid;
  int status;

  pid = fork();
  if(pid==-1)return 127;

  if(pid==0)
    {
      close(2);
      execlp("/usr/bin/gcc", "gcc", "-fPIC", "-shared", "-o", 
	     filename_dst, filename_src, "-lm", NULL);
    }

  wait(&status);
  return status; 
}

pt2Func GetFunc(char *lib_filename)
{
  void * lib_handle;
  pt2Func buf;
  lib_handle = dlopen(lib_filename, RTLD_LAZY);
  *(void**) (&buf) = dlsym(lib_handle, "f");
  return buf;
}

#ifdef TEST

//Kompilera med gcc -DTEST dynmathfunc.c -ldl

main()
{
  char funcstr[100];
  pt2Func myFunction;
  double x = 1.0;
  printf("f(x) = "); fgets(funcstr,sizeof(funcstr),stdin);
  printf("f(x) = %s", funcstr);
  create_function_source_file(funcstr,"func.c");
  create_library("func.c", "func.so");
  myFunction = GetFunc("./func.so");
  while(x!=0.0)
    {
      printf("x = "); scanf("%lf", &x);
      //Här ser vi ett anrop till en dynamiskt skapad funktion:
      printf("f(x) = %6.3lf.\n", (*myFunction)(x));
      //man måste alltså skriva (*myFunction)(x) för att få ut värdet,
      //men annars är det precis som en vanlig funktion i C som vi
      //skriver i en vanlig källkodsfil.
    }
}

#endif

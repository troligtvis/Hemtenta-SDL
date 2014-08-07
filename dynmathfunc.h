#ifndef DYNMATHFUNC_H
#define DYNMATHFUNC_H


void create_function_source_file(char *expression, char *filename);
int create_library (char *filename_src, char *filename_dst);

typedef double (*pt2Func)(double);
pt2Func GetFunc(char *lib_filename);

#endif

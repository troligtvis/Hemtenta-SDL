#ifndef _DISPUNIT_H
#define _DISPUNIT_H

struct dispunit
{
  double a, b, height;
  double gridlength;
  char functext[100];
};

//Creates a dispunit från the data in a, b, h, g, and f
void du_create(double a, double b, double h, double g, char f[], struct dispunit * du);

//Puts a dispunit in a binary file
void du_put(struct dispunit * du, char filename[]);

//Gets a dispunit stored in a binary file
void du_get(struct dispunit * du, char filename[]);

//Prints out the data in a dispunit
void du_print(struct dispunit *du);

#endif


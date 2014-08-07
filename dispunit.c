#include <stdio.h>
#include <string.h>
#include "dispunit.h"

void du_create(double a, double b, double h, double g, char f[], struct dispunit * du)
{
  du->a = a; du->b = b; du->height = h; du->gridlength = g; strcpy(du->functext, f);
}

void du_print(struct dispunit *du)
{
  printf("***|Display Unit|***\n");
  printf("a:\t%6.6lf\nb:\t%6.6lf\n", du->a, du->b);
  printf("Height:\t%6.6lf\n", du->height);
  printf("Grdln:\t%6.6lf\nFunc:\t%s\n", du->gridlength, du->functext);
  printf("********************\n");
}

void du_put (struct dispunit *du, char filename[])
{
  FILE *fp = fopen(filename,"wb");
  fwrite(du, sizeof(struct dispunit), 1, fp);
  fclose(fp);
}

void du_get (struct dispunit *du, char filename[])
{
  FILE *fp = fopen(filename,"rb");
  fread(du, sizeof(struct dispunit), 1, fp);
  fclose(fp);
}

#ifdef TEST

main()
{
  struct dispunit du1;
  struct dispunit du2;
  du_create(-1.0, 1.0, 0.0, 0.1, "sin(x)", &du1);
  du_print(&du1);
  du_put(&du1,"testa_du.du");
  du_get(&du2,"testa_du.du");
  du_print(&du2);
}

#endif

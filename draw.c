#include <SDL/SDL.h>
#include "draw.h"

static int myabs(int x){if(x>0)return x; else return -x;}
static int min(m,n){if (m<n)return m; else return n;}
static int max(m,n){if (m>n)return m; else return n;}
static void swap(double n1, double n2) {double tmp=n1; n1=n2; n2=tmp;}

void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  Uint32 color = SDL_MapRGB(screen->format, R, G, B);
  switch (screen->format->BytesPerPixel)
  {
    case 1: // 8-bpp
      {
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
        *bufp = color;
      }
      break;
    case 2: // 15-bpp or 16-bpp
      {
        Uint16 *bufp;
        bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
        *bufp = color;
      }
      break;
    case 3: // 24-bpp mode, usually not used
      {
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
        if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
          bufp[0] = color;
          bufp[1] = color >> 8;
          bufp[2] = color >> 16;
        } else {
          bufp[2] = color;
          bufp[1] = color >> 8;
          bufp[0] = color >> 16;
        }
      }
      break;
    case 4: // 32-bpp
      {
        Uint32 *bufp;
        bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
        *bufp = color;
      }
      break;
  }
} 

void DrawLine(SDL_Surface *screen, int x1, int y1, 
              int x2, int y2, Uint8 R, Uint8 G, Uint8 B)
{
  double x, y, k;
  int delta;
  
  if(x1==x2 && y1==y2)
    {
      DrawPixel(screen, x1, y1, R, G, B);
      return;
    }

  if(myabs(x1-x2)>abs(y1-y2))
    {
      x=x1; if(x2>x1)delta=1;else delta=-1;
      k = ((double)y2-(double)y1)/((double)x2-(double)x1);
      while( (delta==1 && x<=x2) || (delta==-1 && x>=x2) )
	{
	  y=k*(x-x1)+y1;
	  DrawPixel(screen,(int)x,(int)y,R,G,B);
	  x+=delta;
	}
    }
  else
    {
      y=y1; if(y2>y1)delta=1;else delta=-1;
      k = ((double)x2-(double)x1)/((double)y2-(double)y1);
      while( (delta==1 && y<=y2) || (delta==-1 && y>=y2) )
	{
	  x=k*(y-y1)+x1;
	  DrawPixel(screen,(int)x,(int)y,R,G,B);
	  y+=delta;
	}      
    }  
}



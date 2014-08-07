//
//  filename: display.c
//
//**************************************************************************
//  Skriver ut en matematisk formel från en binär fil som omvandlas till en
//  dynamisk funktion som kan användas i programmet. Programmet initierar en
//  graf där funktionen ritas ut. Integralen räknas även ut.
//
//  Created by Kj Drougge on 2012-10-10.
//  Copyright (c) 2012 Kj Drougge. All rights reserved.
//**************************************************************************

#include <stdio.h>
#include <SDL/SDL.h>
#include <string.h>
#include <wait.h>
#include <math.h>
#include "draw.h"
#include "dynmathfunc.h"
#include "dispunit.h"

/************************************************************************
/
/   integral_calculator: beräknar integralen från a till b.
/
************************************************************************/

void integral_calculator(pt2Func myFunction, double maxVal, double minVal)
{
  double y, x, area = 0;
  for (x = minVal; x <= maxVal;x+= 0.0001)
    { 
      y	    = (myFunction)(x);
      area += 0.0001*y;
    }
  if (area < 0) 
    {
      area = 0;
    }
  printf("Integralen: %lf ae.\n", area);
  printf("********************\n");
}

/************************************************************************
/
/   draw_frame_grid: ritar ut en ram även "grids" som kan ändra
/                    värde genom knapptryckningar.
/
************************************************************************/

void draw_frame_grid(SDL_Surface *screen, double moveX, double moveY, int size, double gridlength)
{
  double measuredSize = gridlength*size;
  double x;
    
  
  //Draw frame
  DrawLine(screen, 40, 40, 440, 40, 0,127,0 );
  DrawLine(screen, 440, 40, 440, 440, 0,127,0 );
  DrawLine(screen, 440, 440, 40, 440, 0,127,0 );
  DrawLine(screen, 40, 440, 40, 40, 0,127,0 );
  
  //x-axis
  if(240+moveY < 440 && 240+moveY > 40)
    DrawLine(screen, 40, 240+moveY, 440, 240+moveY, 0,255,0 );
  
  //y-axis
  if(240+moveX < 440 && 240+moveX > 40)
    DrawLine(screen, 240+moveX, 40, 240+moveX, 440, 0,255,0 );
    
  //Draw grids
  x = 0;
  while (240+x+moveX < 440 && 240+x+moveY > 40 && 240-x+moveY < 440) {
    if(240+x+moveX < 40)
      DrawLine(screen, 240+x+moveX, 238+moveY, 240+x+moveX, 242+moveY, 0,0,0);
    else
      DrawLine(screen, 240+x+moveX, 238+moveY, 240+x+moveX, 242+moveY, 0,255,0);
    
    x += measuredSize;
  }
  
  x = 0;
  while (240-x+moveX > 40 && 240-x+moveY < 440 && 240+x+moveY > 40) {
    if (240-x+moveX > 440) 
      DrawLine(screen, 240-x+moveX, 238+moveY, 240-x+moveX, 242+moveY, 0,0,0);
    else
            DrawLine(screen, 240-x+moveX, 238+moveY, 240-x+moveX, 242+moveY, 0,255,0);
        
        x += measuredSize;
    }
    
    x = 0;
    while (240+x+moveY < 440 && 240+x+moveX > 40 && 240-x+moveX < 440) {
      if(240+x+moveY < 40)
	DrawLine(screen, 238+moveX, 240+x+moveY, 242+moveX, 240+x+moveY, 0,0,0);
      else
	DrawLine(screen, 238+moveX, 240+x+moveY, 242+moveX, 240+x+moveY, 0,255,0);
      
      x += measuredSize;
    }
    
    x = 0;
    while (240-x+moveY > 40 && 240-x+moveX < 440 && 240+x+moveX > 40) {
      if(240-x+moveY > 440)
	DrawLine(screen, 238+moveX, 240-x+moveY, 242+moveX, 240-x+moveY, 0,0,0);
      else
	DrawLine(screen, 238+moveX, 240-x+moveY, 242+moveX, 240-x+moveY, 0,255,0);
      
      x += measuredSize;
    }
}

/************************************************************************
/
/   draw_function: ritar ut funktionen 
/
************************************************************************/

void draw_function(SDL_Surface *screen,double moveX, double moveY, pt2Func myFunction, int minVal, int maxVal,  int size)
{
  double y, x1,	x;
  for(x = minVal; x < maxVal; x+=0.001)
    {
      x1 = 240+x*size+moveX;
      y  = 240-(*myFunction)(x)*size+moveY;
      if(x1 < 440 && x1 > 40 && y < 440 && y > 40)
	DrawPixel(screen, x1, y, 0,255,255);
    }
}

int main(int argc, const char * argv[])
{
  struct dispunit disp;
  char filename[30];
  pt2Func myFunction;
  SDL_Surface *screen;
  double x, x1, y;
  double moveX = 0, moveY = 0;
  int size  = 10;
  
  
  // Hämtar binär fil från argumentinmatningen
  strcpy(filename, argv[1]);
  du_get(&disp, filename);
  
  // Skapar en dynamisk funktion och bibliotek
  create_function_source_file(disp.functext,"func.c");
  create_library("func.c", "func.so");
  myFunction = GetFunc("./func.so");
  
  // Initierar video
  atexit(SDL_Quit);
  screen = SDL_SetVideoMode(480, 480, 8, SDL_SWSURFACE);
  SDL_WM_SetCaption("Graph drawer", "Graph drawer");

  // Visar innehåll i display uniten.
  du_print(&disp);
  
  moveY = disp.height*size;
  
  draw_frame_grid(screen, moveX, moveY, size, disp.gridlength);
  draw_function(screen,moveX, moveY, myFunction, disp.a, disp.b, size);
  integral_calculator(myFunction, disp.b, disp.a);
  
  
  // Event för att zooma samt röra sig runt i grafen.
  int	done = 0;
  while (!done)
    {
      SDL_UpdateRect(screen, 0, 0, 480, 480);
      SDL_Event event;
      
      while (SDL_PollEvent (&event))
	{
	  switch (event.type)
	    {
	    case SDL_KEYDOWN:
	      switch (event.key.keysym.sym)
		{
                case (SDLK_PLUS):
		  size += 5;
		  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
		  draw_frame_grid(screen, moveX, moveY, size, disp.gridlength);
		  draw_function(screen,moveX, moveY, myFunction, disp.a, disp.b, size);
		  break;
		  
		case (SDLK_MINUS):
		  if(size > 10)
		    size -=	5;
		  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
		  draw_frame_grid(screen, moveX, moveY, size, disp.gridlength);
		  draw_function(screen,moveX, moveY, myFunction, disp.a, disp.b, size);                   
		  break;
		  
		case (SDLK_RIGHT):
		  moveX -= 10;
		  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
		  draw_frame_grid(screen, moveX, moveY, size, disp.gridlength);
		  draw_function(screen,moveX, moveY, myFunction, disp.a, disp.b, size);
		  break;
		  
		case (SDLK_LEFT):
		  moveX += 10;
		  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
		  draw_frame_grid(screen, moveX, moveY, size, disp.gridlength);
		  draw_function(screen,moveX, moveY, myFunction, disp.a, disp.b, size);
		  break;
		  
		case (SDLK_DOWN):
		  moveY -= 10;
		  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
		  draw_frame_grid(screen, moveX, moveY, size, disp.gridlength);
		  draw_function(screen,moveX, moveY, myFunction, disp.a, disp.b, size);
		  break;
		  
		case (SDLK_UP):
		  moveY += 10;
		  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
		  draw_frame_grid(screen, moveX, moveY, size, disp.gridlength);
		  draw_function(screen,moveX, moveY, myFunction, disp.a, disp.b, size);
		  break;
		  
                case (SDLK_q):
		  return 0;
		  
                default:
		  break;
		}
	      
	      break;
              
            case SDL_QUIT:
	      done = 1;
	      break;
              
            default:
	      break;
              
	    }
	  SDL_Delay(10); // Give the CPU a rest
	}
    }
}


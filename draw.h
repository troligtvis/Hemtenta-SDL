#ifndef DRAW_H
#define DRAW_H

#include <SDL/SDL.h>

void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B);
void DrawLine(SDL_Surface *screen, int x1, int y1, 
              int x2, int y2, Uint8 R, Uint8 G, Uint8 B);

#endif

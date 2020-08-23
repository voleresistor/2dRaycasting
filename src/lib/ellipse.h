/* ellipse.h */

#include <SDL2/SDL.h>
#include <math.h>

extern void draw_circle(SDL_Renderer* r, int x0, int y0, int radius);

extern int inside_circle(int x0, int y0, int r, int x1, int y1);
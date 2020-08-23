/* ellipse.c */

#include "ellipse.h"

void draw_circle(SDL_Renderer* r, int x0, int y0, int radius)
{
    const int diameter = radius * 2;

    int x = radius - 1;
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = tx - diameter;

    while (x >= y) {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint( r, x0 + x, y0 - y );
        SDL_RenderDrawPoint( r, x0 + x, y0 + y );
        SDL_RenderDrawPoint( r, x0 - x, y0 - y );
        SDL_RenderDrawPoint( r, x0 - x, y0 + y );
        SDL_RenderDrawPoint( r, x0 + y, y0 - x );
        SDL_RenderDrawPoint( r, x0 + y, y0 + x );
        SDL_RenderDrawPoint( r, x0 - y, y0 - x );
        SDL_RenderDrawPoint( r, x0 - y, y0 + x );

        if(error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if(error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
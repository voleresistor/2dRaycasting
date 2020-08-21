/* ray_sdl.h */

#include <SDL2/SDL.h>

/*
    Global SDL assets
*/
SDL_Window*     gWindow;
SDL_Renderer*   gRenderer;

/*
    Specific SDL assest
*/
SDL_Surface*    intro_image;
SDL_Texture*    intro_texture;

SDL_Event       event;
const Uint8*    keystates;

/*
    Loop controls
*/
int     quit;
int     quit_program;

/*
    Game controls
*/
int     play;
int     pause;

/*
    SDL use
*/
extern int init_sdl(int screen_width, int screen_height);
extern void event_sdl(SDL_Event* e);
extern void close_sdl();

/*
    Game rendering
*/

// Oops there's nothing here lol!
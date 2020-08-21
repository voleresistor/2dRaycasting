/* ray_sdl.c */

#include "ray_sdl.h"

int init_sdl(int screen_width, int screen_height)
{
    // Set vars to something
    gRenderer   = NULL;
    gWindow     = NULL;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL! Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create window
    gWindow = SDL_CreateWindow("SDL Raycasting", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
    if(gWindow == NULL)
    {
        printf("Error creating SDL window! Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL)
    {
        printf("Error creating SDL renderer! Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    else
    {
        // Set background
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0x0);
    }

    // Initialize SDL mixer
    /*if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0 )
    {
        return false;
    }*/

    // Set keyboard repeat


    return EXIT_SUCCESS;
}

void event_sdl(SDL_Event* e)
{
    // Clicking x
    if(e->type == SDL_QUIT)
    {
        quit = 1;
    }

    if(e->type == SDL_KEYDOWN)
    {
        switch(e->key.keysym.sym)
        {
            case SDLK_SPACE:
            pause = 1;
            break;

            case SDLK_RETURN:
            play = 1;
            break;

            case SDLK_q:
            quit = 1;
            break;
        }
    }
}

void drawShot(float x_pos, float y_pos, int shot_size_x, int shot_size_y)
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x0, 0x0);

    SDL_Rect shot_rect;
    shot_rect.x = x_pos;
    shot_rect.y = y_pos;
    shot_rect.w = shot_size_x;
    shot_rect.h = shot_size_y;

    SDL_RenderFillRect(gRenderer, &shot_rect);
}

void close_sdl()
{
    // Free up window and renderer
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    // Free up music file
    //Mix_FreeMusic(music);
    //music = NULL;

    // Close out SDL subsystems
    //Mix_CloseAudio();
    SDL_Quit();
}
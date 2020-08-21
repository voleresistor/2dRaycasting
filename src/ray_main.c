/*
    ray.c

    Simple program to explore raycasting in two dimensions.

    Written using C and SDL2.
*/

#include "ray_main.h"

/*
    ray_init()

    Initialize SDL systems and prepare the game engine
*/
int ray_init(void)
{
    /*
        A simple name and version banner
    */
    printf("===================================================\n\n");
    printf("            SDL Raycasting - v0.1\n");
    printf("           2020 ogden.andrew@gmail.com\n\n");
    printf("===================================================\n\n");

    /*
        Init SDL
    */
    if(init_sdl(SCREEN_WIDTH, SCREEN_HEIGHT) != 0)
    {
        printf("Failed to initialize SDL.\n");
        return -1;
    }

    /*
        Init game controls
    */
    quit_program    = 0;    // Not needed right now
    play            = 0;    // Not using this now either
    quit            = 0;
    pause           = 0;

    /*
        Init random
    */
    srand(clock());

    /*
        Show main menu
    */
    return ray_main();
}

/*
    ray_main()

    Operate the main game loop
*/
int ray_main(void)
{
    /*
        Set up these values for this loop
    */
    n_rays = 2048;
    n_bounds = 3;

    /*
        Create boundaries for our screen edges and random
        obstacles.

        Is it more efficient to calculate and trim all rays
        the intersect screencoundaries, or to let rays
        be rendered outside the bounds of the screen?
    */
    Boundary bounds[n_bounds];
    bounds[0]._x1 = 0;
    bounds[0]._x2 = SCREEN_WIDTH;
    bounds[0]._y1 = 0;
    bounds[0]._y2 = 0;

    bounds[1]._x1 = 0;
    bounds[1]._x2 = SCREEN_WIDTH;
    bounds[1]._y1 = SCREEN_HEIGHT;
    bounds[1]._y2 = SCREEN_HEIGHT;

    bounds[2]._x1 = 0;
    bounds[2]._x2 = 0;
    bounds[2]._y1 = 0;
    bounds[2]._y2 = 0;

    bounds[3]._x1 = SCREEN_WIDTH;
    bounds[3]._x2 = SCREEN_WIDTH;
    bounds[3]._y1 = 0;
    bounds[3]._y2 = SCREEN_HEIGHT;

    /* n_bounds number of random additional boundaries */
    for(int i = 4; i < n_bounds + 4; i++)
    {
        Boundary b;
        b._x1 = rand() % 1024;
        b._y1 = rand() % 768;
        b._x2 = rand() % 1024;
        b._y2 = rand() % 768;

        bounds[i] = b;
    }

    /*
        All light rays should originate from the same
        point and will use the same algorithm to
        calculate outer points.
    */
    LightRay rays[n_rays];
    printf("%f\n", (TAU / n_rays));
    for(int i = 0; i < n_rays; i++)
    {
        a = (TAU / n_rays) * i;
        LightRay r;
        r._x_pos = SCREEN_WIDTH / 2;
        r._y_pos = SCREEN_HEIGHT / 2;
        r._x_vect = r._x_pos + SCREEN_WIDTH * cos(a) + 0.5;
        r._y_vect = r._y_pos + SCREEN_WIDTH * sin(a) + 0.5;
        rays[i] = r;
    }
    
    while(quit == 0)
    {
        /*
            Handle events

            Even though I'm using KeyBoardState, this is still
            necessary to poll the mouse and keyboard state for
            SDL_GetKeyBoardState and SDL_GetMouseState.
        */
        while(SDL_PollEvent(&event) != 0)
        {
            event_sdl(&event);
        }

        /*
            Keystates allow smoother control for games when compared to 
            keypresses pulled direcly out of the SDL_Event function
        */
        keystates = SDL_GetKeyboardState(NULL);
        SDL_GetMouseState(&m_x, &m_y);

        /*
            Reset all the rays to a diameter of SCREEN_WIDTH * 2 to
            make them cast all the way to the edge of the screen if
            unobstructed.

            Also set the origin of all rays to the current mouse
            position.
        */
        for(int i = 0; i < n_rays; i++)
        {
            a = (TAU / n_rays) * i;
            rays[i]._x_pos = m_x;
            rays[i]._y_pos = m_y;
            rays[i]._x_vect = (int)rays[i]._x_pos + (SCREEN_WIDTH * 2) * cos(a);
            rays[i]._y_vect = (int)rays[i]._y_pos + (SCREEN_WIDTH * 2) * sin(a);
        }

        /*
            Reset renderer for this new frame
        */
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0x0);
        SDL_RenderClear(gRenderer);

        /*
            For each boundary, pass it to the renderer and
            check if a ray will intersect. If yes, calculate
            the point of intersection and store it in the
            intersect object.

            With the intersection point stored, trim the
            intersecting ray down to that point.
        */
        for(int i = 0; i < n_bounds; i++)
        {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0x0);
            SDL_RenderDrawLine(gRenderer, bounds[i]._x1, bounds[i]._y1, bounds[i]._x2, bounds[i]._y2);

            for(int j = 0; j < n_rays; j++)
            {
                if(check_intersection(&rays[j], &bounds[i], &intersect) == 0)
                {
                    trim_ray(&rays[j], &intersect);
                }
            }
        }

        /*
            Render the light rays in a light blue.
        */
        SDL_SetRenderDrawColor(gRenderer, 0xDA, 0xFB, 0xFA, 0x0);
        for(int i = 0; i < n_rays; i++)
        {
            SDL_RenderDrawLine(gRenderer, rays[i]._x_pos, rays[i]._y_pos, rays[i]._x_vect, rays[i]._y_vect);
        }

        /*
            Present new frame
        */
        SDL_RenderPresent(gRenderer);

        /*
            16 ms delay is ~62fps
        */
        SDL_Delay(16);
    }

    close_sdl();

    return EXIT_SUCCESS;
}

/*
    check_intersection()

    Checks whether a light ray and a bounday line are intersecting.
    Updates positions in Intersection* i and returns 0 if intersection,
    otherwise just returns 1 without touching Intersection* i.
*/
int check_intersection(LightRay* ray, Boundary* b, Intersection* i)
{
    float x1 = b->_x1;
    float y1 = b->_y1;
    float x2 = b->_x2;
    float y2 = b->_y2;

    float x3 = ray->_x_pos;
    float y3 = ray->_y_pos;
    float x4 = ray->_x_vect;
    float y4 = ray->_y_vect;

    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if(den == 0.0)
    {
        return 1;
    }

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    float u = (((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den) * -1.0;

    if(t > 0.0 && t < 1.0 && u > 0.0 && u < 1.0)
    {
        get_intersection(ray, b, i, t);
        return 0;
    }

    return 1;
}

/*
    get_intersection()

    Returns void. Updates Intersection* i
    with the x and y points of intersection.
*/
void get_intersection(LightRay* ray, Boundary* b, Intersection* i, float t)
{
    /*
        The addition of 0.5 is a stupid little hack to help
        approximating when converting back to int.
    */
    float x_i = (b->_x1 + t * (b->_x2 - b->_x1)) + 0.5;
    float y_i = (b->_y1 + t * (b->_y2 - b->_y1)) + 0.5;

    i->_x_pos = x_i;
    i->_y_pos = y_i;
}

/*
    trim_ray()

    Set ray line segment outer points to those provided by
    Intersection* i.
*/
void trim_ray(LightRay* ray, Intersection* i)
{
    /* Mouse left X point */
    if(i->_x_pos < ray->_x_pos && i->_x_pos > ray->_x_vect)
    {
        ray->_x_vect = i->_x_pos;
    }
    
    /* Mouse right X point */
    if(i->_x_pos > ray->_x_pos && i->_x_pos < ray->_x_vect)
    {
        ray->_x_vect = i->_x_pos;
    }

    /* Mouse up Y point */
    if(i->_y_pos < ray->_y_pos && i->_y_pos > ray->_y_vect)
    {
        ray->_y_vect = i->_y_pos;
    }
    
    /* Mouse down Y point */
    if(i->_y_pos > ray->_y_pos && i->_y_pos < ray->_y_vect)
    {
        ray->_y_vect = i->_y_pos;
    }
}

/*
    main()
*/
int main(void)
{
    return ray_init();
}
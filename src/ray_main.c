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
    /* A simple name and version banner */
    printf("===================================================\n\n");
    printf("            SDL Raycasting - v0.1\n");
    printf("           2020 ogden.andrew@gmail.com\n\n");
    printf("===================================================\n\n");

    /* Init SDL */
    if(init_sdl(SCREEN_WIDTH, SCREEN_HEIGHT) != 0)
    {
        printf("Failed to initialize SDL.\n");
        return -1;
    }

    /* SDL settings */
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    /* Init game controls */
    quit_program    = 0;    // Not needed right now
    play            = 0;    // Not using this now either
    quit            = 0;
    pause           = 0;

    /* Init random */
    srand(clock());

    /* Run the program */
    return ray_main();
}

/*
    ray_main()

    Operate the main game loop
*/
int ray_main(void)
{
    /* CControl value for this instance of the program */
    s_radius            = 15;           // Default light radius
    n_rays              = 360;
    n_bounds            = 5;            // First four are screen edges
    m_luminance         = 25;           // Whole ints between 0 and 100

    /* Create boundary object and boundaries */
    Boundary bounds[n_bounds];

    for(int i = 0; i < n_bounds; i++)
    {
        Boundary b;
        b._x1 = rand_i(0, SCREEN_WIDTH);
        b._y1 = rand_i(0, SCREEN_HEIGHT);
        b._x2 = rand_i(0, SCREEN_WIDTH);
        b._y2 = rand_i(0, SCREEN_HEIGHT);

        bounds[i] = b;
    }

    /*
        All light rays should originate from the same
        point and will use the same algorithm to
        calculate outer points.
    */
    Light lights[3];
    for(int i = 0; i < 3; i++)
    {
        Light l;
        new_light(&l, s_radius, n_rays, m_luminance);

        lights[i] = l;
    }
    // Light light_1;
    // new_light(&light_1, s_radius, n_rays, m_luminance);

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

        /* Handle mouse and keyboard input */
        SDL_GetMouseState(&m_x, &m_y);

        /* TODO: Handle this in a function */
        keystates = SDL_GetKeyboardState(NULL);
        if(keystates[SDL_SCANCODE_Q])
        {
            if(m_luminance > 0)
            {
                m_luminance--;
                printf("%d\n", m_luminance);
            }
        }
        else if(keystates[SDL_SCANCODE_E])
        {
            if(m_luminance < 100)
            {
                m_luminance++;
                printf("%d\n", m_luminance);
            }
        }

        /* Reposition light to mouse pointer */
        for(int i = 0; i < 3; i++)
        {
            if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
            {
                if(lights[i]._m_lock == 1 && m_select == 1)
                {
                    move_light(&lights[i], m_x, m_y);
                }
                else if(inside_circle(lights[i]._x_pos, lights[i]._y_pos, lights[i]._radius, m_x, m_y) == 1 &&
                        m_select == 0)
                {
                    lights[i]._m_lock = 1;
                    m_select = 1;
                    move_light(&lights[i], m_x, m_y);
                }
            }
            else
            {
                lights[i]._m_lock = 0;
                m_select = 0;
            }
        }

        /* Update _luminance */
        for(int i = 0; i < 3; i++)
        {
            lights[i]._luminance  = map(m_luminance, 0, 100, lights[i]._radius, SCREEN_WIDTH);
        }
        
        /* reset all the rays to a mapped length based on _luminance. */
        for(int i = 0; i < 3; i++)
        {
            reset_light(&lights[i]);
        }

        /* Reset renderer for this new frame */
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0xFF);
        SDL_RenderClear(gRenderer);

        /*
            For each boundary, pass it to the renderer and
            check if a ray will intersect. If yes, calculate
            the point of intersection and store it in the
            intersect object.

            With the intersection point stored, trim the
            intersecting ray down to that point.
        */
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        for(int i = 0; i < n_bounds; i++)
        {
            SDL_RenderDrawLine(gRenderer, bounds[i]._x1, bounds[i]._y1, bounds[i]._x2, bounds[i]._y2);

            for(int j = 0; j < 3; j++)
            {
                do_block_light(&lights[j], &bounds[i]);
            }
        }

        /* Render the light rays and light in light blue. */
        SDL_SetRenderDrawColor(gRenderer, 0xDA, 0xFB, 0xFA, 0x32);
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < n_rays; j++)
            {
                SDL_RenderDrawLine(gRenderer, lights[i]._rays[j]._x_pos, lights[i]._rays[j]._y_pos,
                                   lights[i]._rays[j]._x_endpt, lights[i]._rays[j]._y_endpt);
            }

            draw_circle(gRenderer, lights[i]._x_pos, lights[i]._y_pos, lights[i]._radius);
        }

        /* Present new frame */
        SDL_RenderPresent(gRenderer);

        /* 16 ms delay is ~62fps */
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
int check_intersection(Ray* ray, Boundary* b, Intersection* i)
{
    float x1 = b->_x1;
    float y1 = b->_y1;
    float x2 = b->_x2;
    float y2 = b->_y2;

    float x3 = ray->_x_pos;
    float y3 = ray->_y_pos;
    float x4 = ray->_x_endpt;
    float y4 = ray->_y_endpt;

    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if(den == 0.0)
    {
        return 0;
    }

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    float u = (((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den) * -1.0;

    if(t > 0.0 && t < 1.0 && u > 0.0 && u < 1.0)
    {
        get_intersection(ray, b, i, t);
        return 1;
    }

    return 0;
}

/*
    get_intersection()

    Returns void. Updates Intersection* i
    with the x and y points of intersection.
*/
void get_intersection(Ray* ray, Boundary* b, Intersection* i, float t)
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
    do_block_light()

    Calculates intersections for each ray of the given light
    against the given bound and updates the rays.
*/
void do_block_light(Light* l, Boundary* b)
{
    Intersection intersection;

    for(int i = 0; i < l->_n_rays; i++)
    {
        if(check_intersection(&l->_rays[i], b, &intersection) == 1)
        {
            trim_ray(&l->_rays[i], &intersection);
        }
    }
}

/*
    trim_ray()

    Set ray line segment outer points to those provided by
    Intersection* i.
*/
void trim_ray(Ray* ray, Intersection* i)
{
    /* Mouse left X point */
    if(i->_x_pos < ray->_x_pos && i->_x_pos > ray->_x_endpt)
    {
        ray->_x_endpt = i->_x_pos;
    }
    
    /* Mouse right X point */
    if(i->_x_pos > ray->_x_pos && i->_x_pos < ray->_x_endpt)
    {
        ray->_x_endpt = i->_x_pos;
    }

    /* Mouse up Y point */
    if(i->_y_pos < ray->_y_pos && i->_y_pos > ray->_y_endpt)
    {
        ray->_y_endpt = i->_y_pos;
    }
    
    /* Mouse down Y point */
    if(i->_y_pos > ray->_y_pos && i->_y_pos < ray->_y_endpt)
    {
        ray->_y_endpt = i->_y_pos;
    }
}

/*
    new_light()

    Create a new light object of the given radius.
*/
void new_light(Light* l, int radius_, int n_rays_, int m_luminance_)
{
    l->_radius      = radius_;
    l->_x_pos       = rand_i(0, SCREEN_WIDTH);
    l->_y_pos       = rand_i(0, SCREEN_HEIGHT);
    l->_x_vect      = rand_i(-5, 5);
    l->_y_vect      = rand_i(-5, 5);
    l->_n_rays      = n_rays_;

    l->_rays        = malloc(n_rays_ * sizeof(Ray));
    l->_luminance   = map(m_luminance_, l->_radius, 100, 0 , SCREEN_WIDTH);

    new_rays(l);
}

/*
    reset_light()

    Resets all rays maximum sized based on luminance.
    This allows for proper calculation of ray to obstruction
    intersction points later in the loop.
*/
void reset_light(Light* l)
{
    float _a;

    for(int i = 0; i < l->_n_rays; i++)
    {
        _a = (TAU / l->_n_rays) * i;

        l->_rays[i]._x_endpt    = (int)l->_rays[i]._x_pos + l->_luminance * cos(_a);
        l->_rays[i]._y_endpt    = (int)l->_rays[i]._y_pos + l->_luminance * sin(_a);
    }
}

/*
    new_ray()

    Create and return a new Ray object.
*/
void new_rays(Light* l)
{
    float _a;

    for(int i = 0; i < l->_n_rays; i++)
    {
        _a = (TAU / l->_n_rays) * i;

        Ray r;
        r._x_pos       = l->_x_pos;
        r._y_pos       = l->_y_pos;
        r._x_endpt     = r._x_pos + l->_luminance * cos(_a) + 0.5;
        r._y_endpt     = r._y_pos + l->_luminance * sin(_a) + 0.5;

        l->_rays[i] = r;
    }
}

/*
    move_light()

    Move the light and rays to the given x and y coordinate.
*/
void move_light(Light* l, int x, int y)
{
    l->_x_pos = x;
    l->_y_pos = y;

    for(int i = 0; i < n_rays; i++)
    {
        l->_rays[i]._x_pos = x;
        l->_rays[i]._y_pos = y;
    }
}

/*
    main()
*/
int main(void)
{
    return ray_init();
}
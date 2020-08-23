/* ray.h */

/* Standard Libraries */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

/* Custom Libraries */
#include "ray_sdl.h"
#include "lib/util.h"

/*
    Ray struct
*/
typedef struct {
    int _x_pos;
    int _y_pos;
    int _x_endpt;
    int _y_endpt;
} Ray;

/*
    Light struct
*/
typedef struct {
    int _x_pos;
    int _y_pos;
    int _x_vect;
    int _y_vect;
    int _radius;
    int _n_rays;
    int _luminance;
    Ray* _rays;
} Light;

/*
    Boundary struct
*/
typedef struct {
    int _x1;
    int _y1;
    int _x2;
    int _y2;
} Boundary;

/*
    Intersection point struct
*/
typedef struct {
    int _x_pos;
    int _y_pos;
} Intersection;

/*
    Helpful macros
*/
#define SCREEN_WIDTH        1024
#define SCREEN_HEIGHT       768
#define PI                  3.14159265
#define TAU                 6.28318530

/*
    Var declarations
*/
/* Holds last calculated intersection point */
Intersection intersect;

/* Mouse x and y position for this frame. */
int m_x;
int m_y;

/* Control number of on screen objects */
int n_rays;
int n_bounds;

/* Control light properties */
int m_luminance;
int s_radius;

/* MATH */
float a;

/*
    Function declaration
*/
extern int main(void);
extern int ray_init(void);
extern int ray_main(void);
extern int check_intersection(Ray* ray, Boundary* b, Intersection* i);

extern void get_intersection(Ray* ray, Boundary* b, Intersection* i, float t);
extern void trim_ray(Ray* ray, Intersection* i);
extern void new_light(Light* l, int radius_, int n_rays_, int m_luminance_);
extern void new_rays(Light* l);
extern void move_light(Light* l, int x, int y);
extern void reset_light(Light* l);
extern void do_block_light(Light* l, Boundary* b);
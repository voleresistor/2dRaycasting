/* ray.h */

/* Standard Libraries */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

/* Custom Libraries */
#include "ray_sdl.h"

/*
    LightRay struct
*/
typedef struct {
    int _x_pos;
    int _y_pos;
    int _x_vect;
    int _y_vect;
} LightRay;

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

/* MATH */
float a;

/*
    Function declaration
*/
extern int main(void);
extern int ray_init(void);
extern int ray_main(void);

extern int check_intersection(LightRay* ray, Boundary* b, Intersection* i);
extern void get_intersection(LightRay* ray, Boundary* b, Intersection* i, float t);

extern void trim_ray(LightRay* ray, Intersection* i);
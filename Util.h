#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <algorithm>
#include "Vector3.h"

#define PI 3.1415926535f

#define WHITE   Vector3(255, 255, 255)
#define BLACK   Vector3(0,   0,   0)
#define RED     Vector3(255, 0,   0)
#define GREEN   Vector3(0,   255, 0)
#define BLUE    Vector3(0,   0,   255)
#define CYAN    Vector3(0,   255, 255)
#define YELLOW  Vector3(255, 255, 0)
#define MAGENTA Vector3(255, 0,   255)
#define PURPLE  Vector3(102, 51,  153)

#define X_AXIS  Vector3(1,   0,   0)
#define Y_AXIS  Vector3(0,   1,   0)
#define Z_AXIS  Vector3(0,   0,   1)

/*
 * TRACE:     A normal ray-traced image. Nothing fancy.
 * SPHERE:    Does a lens trace through a spherical lens.
 * BICONCAVE: Does a lens trace through a biconcave lens.
 * BICONVEX:  Does a lens trace through a biconvex lens.
 * PCONCAVE:  Does a lens trace through a plano-concave lens.
 * PCONVEX:   Does a lens trace through a plano-convex lens.
 *
 * NOTE: Depth of field and stereo imaging can be enabled for each of these modes.
 *       For all lens tracing, DoF is very tricky to set as the lens reduces the effect.
 */
#define TRACE     0
#define SPHERE    1
#define BICONCAVE 2
#define BICONVEX  3
#define PCONCAVE  4
#define PCONVEX   5

double deg_to_rad(double deg);// { return deg * PI / 180.0; }

//Maps in to the a number between out_min and out_max.
double map(double in, double in_min, double in_max, double out_min, double out_max);

double rad_to_deg(double rad);

//Rotates a vector around an arbitrary axis by a given amount.
Vector3 rotAroundAxis(Vector3 axis, Vector3 vector, double theta); 
#endif

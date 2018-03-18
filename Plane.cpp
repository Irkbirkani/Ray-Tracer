#include "Plane.h"

Plane::Plane(Vector3 c, Vector3 n, Vector3 col)
{
    center = c; 
    N = n; 
    color = col;
}


bool Plane::intersect(Ray ray, Vector3 &p)
{
    Vector3 dir = ray.direction - ray.origin;
    double d = ((center - ray.origin).dot(N)) / (dir.dot(N));

    if (d <= 0)
        return false;

    p = dir * d + ray.origin;
    return true;
}

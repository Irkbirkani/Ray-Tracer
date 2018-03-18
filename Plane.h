#ifndef PLANE_H
#define PLANE_H

#include "Util.h"
#include "Ray.h"

class Plane 
{
public:
    Vector3 center;
    Vector3 color;
    Vector3 N;

    Plane(Vector3 c = Vector3(), Vector3 n = Vector3(1), Vector3 col = WHITE);

    //Returns true if a ray intersects with a plane.
    bool intersect(Ray ray, Vector3 &p);
};

#endif

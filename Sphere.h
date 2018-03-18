#ifndef SPHERE_H
#define SPHERE_H

#include "Util.h"
#include "Ray.h"
#include "Image.h"

class Sphere {

public:
    Vector3 center, color;
    Image image;
    double radius;
    bool texMap; 

    Sphere(Vector3 c = Vector3(), double r = 1000, Vector3 col = WHITE,
               bool tm = false, Image img = NULL);

    // Returns true if a ray intersects with the sphere.
    // The closest point of intersection is set as t.
    bool intersect(Ray ray, double &t);

    // Returns true if a ray intersects with the sphere. 
    // The farthest point of intersection is set as t.
    bool intersectFar(Ray ray, double &t);

    // Returns the barycentric coordinates of a point on the sphere.
    void getSphereUV(Vector3 normal, double &u, double &v);

    // Returns the color of the texture for a specific point on a Sphere.
    Vector3 getTex(Vector3 normal);
};
#endif

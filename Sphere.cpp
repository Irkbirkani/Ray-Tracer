#include "Sphere.h"

Sphere::Sphere(Vector3 c, double r, Vector3 col, bool tm, Image img)
{
    center = c; 
    radius = r; 
    color = col; 
    texMap = tm; 
    image = img;
}

bool Sphere::intersect(Ray ray, double &t) 
{
    //Calculate the a b and c values for the quadratic equation.
    double a =  ray.direction.x * ray.direction.x +
               ray.direction.y * ray.direction.y +
               ray.direction.z * ray.direction.z;
    double b = (ray.direction.x * (ray.origin.x - center.x) +
               ray.direction.y * (ray.origin.y - center.y) +
               ray.direction.z * (ray.origin.z - center.z)) * 2;
    double c = (ray.origin.x - center.x) * (ray.origin.x - center.x) +
              (ray.origin.y - center.y) * (ray.origin.y - center.y) +
              (ray.origin.z - center.z) * (ray.origin.z - center.z) -
              (radius * radius);
    //Calculate the discriminate of the quadratic equation.
    double disc = (b * b) - (4 * a * c);

    //If the discriminate is less than 0 then there is no intersection and the funtion terminates.
    if (disc < 0)
        return false;

    //Calculate the two points of intersection and set t to be the smaller.
    double t0 = (-b - sqrt(disc)) / (2 * a);
    double t1 = (-b + sqrt(disc)) / (2 * a);

    t = (t0 < t1 && t0 > 0.001) ? t0 : t1;
    return true;
}


bool Sphere::intersectFar(Ray ray, double &t)
{
    //Calculate the a b and c values for the quadratic equation.
    double a = ray.direction.x * ray.direction.x +
        ray.direction.y * ray.direction.y +
        ray.direction.z * ray.direction.z;
    double b = (ray.direction.x * (ray.origin.x - center.x) +
        ray.direction.y * (ray.origin.y - center.y) +
        ray.direction.z * (ray.origin.z - center.z)) * 2;
    double c = (ray.origin.x - center.x) * (ray.origin.x - center.x) +
        (ray.origin.y - center.y) * (ray.origin.y - center.y) +
        (ray.origin.z - center.z) * (ray.origin.z - center.z) -
        (radius * radius);
    //Calculate the discriminate of the quadratic equation.
    double disc = (b * b) - (4 * a * c);

    //If the discriminate is less than 0 then there is no intersection and the funtion terminates.
    if (disc < 0)
        return false;

    //Calculate the two points of intersection and set t to be the smaller.
    double t0 = (-b - sqrt(disc)) / (2 * a);
    double t1 = (-b + sqrt(disc)) / (2 * a);

    t = (t0 > t1 && t0 > 0.001) ? t0 : t1;
    //printf("t0 = %lf; t1 = %lf; t = t1\n", t0, t1);
    return true;
}

void Sphere::getSphereUV(Vector3 normal, double &u, double &v)
{
    u = atan2(normal.x, normal.z) / (2 * PI) + 0.5f;
    v = normal.y * 0.5f + 0.5f;
}

Vector3 Sphere::getTex(Vector3 normal) {

    //Find the barycentric coordinates.
    double u, v;
    Sphere::getSphereUV(normal, u, v);
    //Map the barycentric coordinates to points on the texture.
    int s = map(u, 0, 1, 0, image.width), t = map(v, 0, 1, 0, image.height);

    //Finds the rgb values of the given point and retuns them as a vector.
    unsigned char r = image.image[3 * (t*image.width + s)], g = image.image[3 * (t*image.width + s) + 1], b = image.image[3 * (t*image.width + s) + 2];
    return Vector3((int)r, (int)g, (int)b);
}


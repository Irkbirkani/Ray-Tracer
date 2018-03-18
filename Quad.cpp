#include "Quad.h"

Quad::Quad(Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 n, Vector3 col, bool tm, Image img)
{
    A = a; 
    B = b; 
    C = c; 
    D = d; 
    normal = n; 
    color = col; 
    texMap = tm;
    image = img;
}


bool Quad::intersect(Ray ray, double &t)
{
    //Finds the point of intersection between the ray and the plane the quad is on.
    Vector3 AB = B - A, AD = D - A;
    double d = ((A - ray.origin).dot(normal)) / (ray.direction.dot(normal));
    Vector3 P = ray.direction * d + ray.origin;

    //Finds the barycentric coordinates of the point of intesection.
    //If the point is greater than one or less than 0 then there is no intersection between the point and the plane.
    u = (-A.x * AD.y + A.y * AD.x - AD.x * P.y + AD.y * P.x) / (AB.x * AD.y - AB.y * AD.x);
    v = (-A.x * AB.y + A.y * AB.x - AB.x * P.y + AB.y * P.x) / (AB.y * AD.x - AB.x * AD.y);
    if ((0<u && u<1) && (0<v && v<1))
    {
        t = d;
        return true;
    }
    return false;
}


Vector3 Quad::getTex(Vector3 pos) {
    //Map the barycentric coordinates to points between the image height and width.
    int s = map(u, 0, 1, 0, image.width), t = map(v, 0, 1, 0, image.height);

    //Finds the rgb values of the given point and retuns them as a vector.
    unsigned char r = image.image[3 * (t*image.width + s)];
    unsigned char g = image.image[3 * (t*image.width + s) + 1];
    unsigned char b = image.image[3 * (t*image.width + s) + 2];
    return Vector3((int)r, (int)g, (int)b);
}

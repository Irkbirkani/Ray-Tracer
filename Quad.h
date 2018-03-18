#ifndef QUAD_H
#define QUAD_H

#include "Image.h"
#include "Ray.h"
#include "Util.h"

class Quad {
public:
    Vector3 A, B, C, D, normal, color;
    Image image;
    bool texMap;
    double u, v;
    Quad(Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 n, Vector3 col, bool tm, Image img);
    bool intersect(Ray ray, double &t);
    Vector3 getTex(Vector3 pos);
};

#endif

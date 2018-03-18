#ifndef LENS_H
#define LENS_H

#include "Sphere.h"
#include "Plane.h"

class Lens {
public:
    Sphere lens[2];
    Plane plane;
    double refracIdx;
    Lens(Sphere frontOfLens, Sphere backOfLens, Plane pl, double refractionIndex);
    void changePos(double offset);
    Ray refract(Ray ray, int refracType);
};

#endif

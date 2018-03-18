#ifndef CAMERA_H 
#define CAMERA_H

#include "Vector3.h"

class Camera {
public:
    Vector3 position, direction, up;
    double aperature;
    Camera();
    Camera(Vector3 pos, Vector3 dir, Vector3 u, double a);
};

#endif

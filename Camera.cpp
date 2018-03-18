#include "Camera.h"

Camera::Camera() {
    position = Vector3(0, 0, -100);
    direction = Vector3(0, 0, 1);
    up = Vector3(0, 1, 0);
    aperature = 10;
}

Camera::Camera(Vector3 pos, Vector3 dir, Vector3 u, double a) {
    position = pos; 
    direction = dir; 
    up = u;
    aperature = a;
 }

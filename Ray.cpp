#include "Ray.h"

Ray::Ray() { origin = Vector3(); direction = Vector3(); }
Ray::Ray(Vector3 o, Vector3 d) { origin = o; direction = d; }


#include "Util.h"

double toMM(double offset, double value){ return value*(63.0/offset); }

double deg_to_rad(double deg) { return deg * PI / 180.0; }

double rad_to_deg(double rad) { return rad * 180.0 / PI; }

double map(double in, double in_min, double in_max, double out_min, double out_max)
{
    double slope = (out_max - out_min) / (in_max - in_min);
    return (in - in_min) * slope + out_min;
}

Vector3 rotAroundAxis(Vector3 axis, Vector3 vector, double theta) {

    axis = axis.normalize();
    Vector3 t = axis;
    if (std::abs(axis.x) - std::min(std::abs(axis.x),
        std::min(std::abs(axis.y), std::abs(axis.z))) < 0.000001) {
        t.x = 1;
    }
    else if (std::abs(axis.y) - std::min(std::abs(axis.x),
             std::min(std::abs(axis.y), std::abs(axis.z))) < 0.000001) {
        t.y = 1;
    }
    else if (std::abs(axis.z) - std::min(std::abs(axis.x),
             std::min(std::abs(axis.y), std::abs(axis.z))) < 0.000001) {
        t.z = 1;
    }
    Vector3 u = axis.cross(t).normalize();
    Vector3 v = axis.cross(u);

    Vector3 res = vector;
    res = Vector3(u.x*res.x + u.y*res.y + u.z*res.z,
                  v.x*res.x + v.y*res.y + v.z*res.z,
                  axis.x*res.x + axis.y*res.y + axis.z*res.z);
    res = res.rotAroundZ(theta);
    res = Vector3(u.x*res.x + v.x*res.y + axis.x*res.z,
                  u.y*res.x + v.y*res.y + axis.y*res.z,
                  u.z*res.x + v.z*res.y + axis.z*res.z);
    return res;
}


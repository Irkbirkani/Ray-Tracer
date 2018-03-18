#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <algorithm>
#include "Vector3.h"

#define PI 3.1415926535f

#define WHITE   Vector3(255, 255, 255)
#define BLACK   Vector3(0,   0,   0)
#define RED     Vector3(255, 0,   0)
#define GREEN   Vector3(0,   255, 0)
#define BLUE    Vector3(0,   0,   255)
#define CYAN    Vector3(0,   255, 255)
#define YELLOW  Vector3(255, 255, 0)
#define MAGENTA Vector3(255, 0,   255)
#define PURPLE  Vector3(102, 51,  153)

#define X_AXIS  Vector3(1,   0,   0)
#define Y_AXIS  Vector3(0,   1,   0)
#define Z_AXIS  Vector3(0,   0,   1)

/*
 * TRACE:     A normal ray-traced image. Nothing fancy.
 * SPHERE:    Does a lens trace through a spherical lens.
 * BICONCAVE: Does a lens trace through a biconcave lens.
 * BICONVEX:  Does a lens trace through a biconvex lens.
 * PCONCAVE:  Does a lens trace through a plano-concave lens.
 * PCONVEX:   Does a lens trace through a plano-convex lens.
 *
 * NOTE: Depth of field and stereo imaging can be enabled for each of these modes.
 *       For all lens tracing, DoF is very tricky to set as the lens reduces the effect.
 */
#define TRACE     0
#define SPHERE    1
#define BICONCAVE 2
#define BICONVEX  3
#define PCONCAVE  4
#define PCONVEX   5

double deg_to_rad(double deg);// { return deg * PI / 180.0; }

//Maps in to the a number between out_min and out_max.
double map(double in, double in_min, double in_max, double out_min, double out_max);
//{
//    double slope = (out_max - out_min) / (in_max - in_min);
//    return (in - in_min) * slope + out_min;
//}

double rad_to_deg(double rad);// { return rad * 180.0 / PI; }

//Rotates a vector around an arbitrary axis by a given amount.
Vector3 rotAroundAxis(Vector3 axis, Vector3 vector, double theta); /* {

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
*/
//class  Vector3 {
//public:
//    double x, y, z;
//    Vector3() { x = y = z = 0; }
//    Vector3(double v) { x = y = z = v; }
//    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
//
//    //Returns the dot product of two vectors.
//    double dot(Vector3 v) { return (x * v.x + y * v.y + z * v.z); }
//
//    //Returns the sum of two vectors.
//    Vector3 operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
//
//    //Returns the difference between two vectors.
//    Vector3 operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
//
//    //Returns the negative of a vector.
//    Vector3 operator - () { return Vector3(-x, -y, -z); }
//
//    //Return the product of a vector and a scalar.
//    Vector3 operator * (double d) const { return Vector3(x*d, y*d, z*d); }
//
//    //Returns the quotient of a vector and a scalar.
//    Vector3 operator / (double d) const { return Vector3(x / d, y / d, z / d); }
//
//    //Returns true if the x, y, and z coordinates are equal
//    bool operator == (const Vector3& v) const {
//        return (std::abs(x - v.x) < 0.000001 && std::abs(y - v.y) < 0.000001 && std::abs(z - v.z) < 0.000001);
//    }
//
//    //Returns true if one of the x, y, or z coordinates does not equal the others.
//    bool operator != (const Vector3& v) const {
//        return ( x - v.x >= 0.000001 || y - v.y >= 0.000001 || z - v.z >= 0.000001);
//    }
//
//    //Returns the magnitude of a vector.
//    double magnitude() { return sqrt(x*x + y*y + z*z); }
//
//    //Normalize the vector.
//    Vector3 normalize() const {
//        double mg = sqrt(x*x + y*y + z*z);
//        return Vector3(x / mg, y / mg, z / mg);
//    }
//
//    //Returns the distance between two vectors.
//    double distance(Vector3 v) {
//        return sqrt((x - v.x) * (x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z));
//     }
//
//    //Returns the cross product of two vectors.
//    Vector3 cross(Vector3 v) { return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
//
//    //Rotates the Vector around the X axis by a given amount.
//    Vector3 rotAroundX(double theta) {
//         return Vector3(x, cos(deg_to_rad(theta))*y + (-sin(deg_to_rad(theta))*z),
//                        sin(deg_to_rad(theta))*y + cos(deg_to_rad(theta))*z);
//     }
//
//    //Rotates the Vector around the Y axis by a given amount.
//    Vector3 rotAroundY(double theta) {
//        return Vector3(cos(deg_to_rad(theta))*x + (-sin(deg_to_rad(theta))*z),
//                       y, sin(deg_to_rad(theta))*x + cos(deg_to_rad(theta))*z);
//        }
//
//    //Rotates the Vector around the Z axis by a given amount.
//    Vector3 rotAroundZ(double theta) {
//        return Vector3(cos(deg_to_rad(theta))*x - sin(deg_to_rad(theta))*y,
//                      sin(deg_to_rad(theta))*x + cos(deg_to_rad(theta))*y, z);
//    }
//
//    //Retuns the angle between two vectors.
//    double getAngleBetween(Vector3 v) {
//         double frac = this->dot(v) / (this->magnitude()*v.magnitude());
//         if(frac > 1.0) return 0;
//         else if(frac < -1.0) return PI;
//         else return acos(frac);
//    }
//
//    //Prints the Vector.
//    void print(char* str) { printf("%sVector(%lf, %lf, %lf)",str, x, y, z); }
//    void print() { printf("Vector(%lf, %lf, %lf)", x, y, z); }
//
//    //Prints the Vector with a new line.
//    void println(char* str) { printf("%sVector(%lf, %lf, %lf)\n",str, x, y, z); }
//    void println() { printf("Vector(%lf, %lf, %lf)\n", x, y, z); }
//
//};
// A simple ray class.
//class Ray {
//public:
//    Vector3 origin;
//    Vector3 direction;
//
//    Ray() { origin = Vector3(); direction = Vector3(); }
//    Ray(Vector3 o, Vector3 d) { origin = o; direction = d; }
//};
//// A simple Image class.
//class Image {
//public:
//    unsigned char * image;
//    int width, height, channels;
//    Image(char * filename = NULL)
//    {
//        if (filename != NULL)
//        {
//            stbi_set_flip_vertically_on_load(false);
//            image = stbi_load(filename, &width, &height, &channels, STBI_rgb);
//        }
//    }
//};
//
//
////A simple Camera class.
//class Camera {
//public:
//    Vector3 position, direction, up;
//    double aperature;
//    Camera() {
//        position = Vector3(0, 0, -100);
//        direction = Vector3(0, 0, 1);
//        up = Vector3(0, 1, 0);
//        aperature = 10;
//    }
//
//    Camera(Vector3 pos, Vector3 dir, Vector3 u, double a) { position = pos; direction = dir; up = u; aperature = a; }
//};
#endif

#include "Util.h"
#include "Vector3.h"
#include <algorithm>

Vector3::Vector3() { x = y = z = 0; }
Vector3::Vector3(double v) { x = y = z = v; }
Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

//Returns the dot product of two vectors.
double Vector3::dot(Vector3 v) { return (x * v.x + y * v.y + z * v.z); }

//Returns the sum of two vectors.
Vector3 Vector3::operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }

//Returns the difference between two vectors.
Vector3 Vector3::operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }

//Returns the negative of a vector.
Vector3 Vector3::operator - () const { return Vector3(-x, -y, -z); }

//Return the product of a vector and a scalar.
Vector3 Vector3::operator * (double d) const { return Vector3(x*d, y*d, z*d); }

//Returns the quotient of a vector and a scalar.
Vector3 Vector3::operator / (double d) const { return Vector3(x / d, y / d, z / d); }

//Returns true if the x, y, and z coordinates are equal
bool Vector3::operator == (const Vector3& v) const {
    return (std::abs(x - v.x) < 0.000001 && std::abs(y - v.y) < 0.000001 && std::abs(z - v.z) < 0.000001);
}

//Returns true if one of the x, y, or z coordinates does not equal the others.
bool Vector3::operator != (const Vector3& v) const {
    return ( x - v.x >= 0.000001 || y - v.y >= 0.000001 || z - v.z >= 0.000001);
}

//Returns the magnitude of a vector.
double Vector3::magnitude() { return sqrt(x*x + y*y + z*z); }

//Normalize the vector.
Vector3 Vector3::normalize() {
    double mg = sqrt(x*x + y*y + z*z);
    return Vector3(x / mg, y / mg, z / mg);
}

//Returns the distance between two vectors.
double Vector3::distance(Vector3 v) {
    return sqrt((x - v.x) * (x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z));
 }

//Returns the cross product of two vectors.
Vector3 Vector3::cross(Vector3 v) { return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }

//Rotates the Vector around the X axis by a given amount.
Vector3 Vector3::rotAroundX(double theta) {
     return Vector3(x, cos(deg_to_rad(theta))*y + (-sin(deg_to_rad(theta))*z),
                    sin(deg_to_rad(theta))*y + cos(deg_to_rad(theta))*z);
 }

//Rotates the Vector around the Y axis by a given amount.
Vector3 Vector3::rotAroundY(double theta) {
    return Vector3(cos(deg_to_rad(theta))*x + (-sin(deg_to_rad(theta))*z),
                   y, sin(deg_to_rad(theta))*x + cos(deg_to_rad(theta))*z);
    }

//Rotates the Vector around the Z axis by a given amount.
Vector3 Vector3::rotAroundZ(double theta) {
    return Vector3(cos(deg_to_rad(theta))*x - sin(deg_to_rad(theta))*y,
                  sin(deg_to_rad(theta))*x + cos(deg_to_rad(theta))*y, z);
}

//Retuns the angle between two vectors.
double Vector3::getAngleBetween(Vector3 v) {
     double frac = this->dot(v) / (this->magnitude()*v.magnitude());
     if(frac > 1.0) return 0;
     else if(frac < -1.0) return PI;
     else return acos(frac);
}

//Prints the Vector.
void Vector3::print(char* str) { printf("%sVector(%lf, %lf, %lf)",str, x, y, z); }
void Vector3::print() { printf("Vector(%lf, %lf, %lf)", x, y, z); }

//Prints the Vector with a new line.
void Vector3::println(char* str) { printf("%sVector(%lf, %lf, %lf)\n",str, x, y, z); }
void Vector3::println() { printf("Vector(%lf, %lf, %lf)\n", x, y, z); }



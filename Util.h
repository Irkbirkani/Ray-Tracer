/* Includes helpful defines and classes*/

#pragma once

#include <cmath>

#define PI 3.1415926535f
#define WHITE Vector3(255, 255, 255)
#define BLACK Vector3(0,   0,   0)
#define RED   Vector3(255, 0,   0)
#define GREEN Vector3(0,   255, 0)
#define BLUE  Vector3(0,   0,   255)


struct Vector3 {
	float x, y, z;
	Vector3() { x = y = z = 0; }
	Vector3(float v) { x = y = z = v; }
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	float dot(Vector3 v) { return (x * v.x + y * v.y + z * v.z); }
	Vector3 operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3 operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	Vector3 operator * (float d) const { return Vector3(x*d, y*d, z*d); }
	Vector3 operator / (float d) const { return Vector3(x / d, y / d, z / d); }
	Vector3 normalize() const {
		float mg = sqrt(x*x + y*y + z*z);
		return Vector3(x / mg, y / mg, z / mg);
	}
	float distance(Vector3 v) { return sqrt((x - v.x) * (x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z)); }
};


void map(float in, float in_min, float in_max, float out_min, float out_max, int &out)
{
	float slope = (out_max - out_min) / (in_max - in_min);
	out = (in - in_min) * slope + out_min;
}


class Ray {
public:
	Vector3 origin;
	Vector3 direction;

	Ray() { origin = Vector3(); direction = Vector3(); }
	Ray(Vector3 o, Vector3 d) { origin = o; direction = d; }
};
/* Includes helpful defines and classes*/

#pragma once

#include <cmath>
//Define required for stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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


struct Vector3 {
	float x, y, z;
	Vector3() { x = y = z = 0; }
	Vector3(float v) { x = y = z = v; }
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	//Returns the dot product of two vectors.
	float dot(Vector3 v) { return (x * v.x + y * v.y + z * v.z); }

	//Returns the sum of two vectors.
	Vector3 operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }

	//Returns the difference between two vectors.
	Vector3 operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }

	//Return the product of a vector and a scalar.
	Vector3 operator * (float d) const { return Vector3(x*d, y*d, z*d); }

	//Returns the quotient of a vector and a scalar.
	Vector3 operator / (float d) const { return Vector3(x / d, y / d, z / d); }

	//Returns the magnitude of a vector.
	float magnitude() { return sqrt(x*x + y*y + z*z); }

	//Normalize the vector.
	Vector3 normalize() const {
		float mg = sqrt(x*x + y*y + z*z);
		return Vector3(x / mg, y / mg, z / mg);
	}

	//Returns the distance between two vectors.
	float distance(Vector3 v) { return sqrt((x - v.x) * (x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z)); }
	
	//Returns the cross product of two vectors.
	Vector3 cross(Vector3 v) { return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }

	//Rotates the Vector around the X axis by a given amount.
	Vector3 rotAroundX(float theta) { return Vector3(x, cos(theta)*y + (-sin(theta)*z), sin(theta)*y + cos(theta)*z); }

	//Rotates the Vector around the Y axis by a given amount.
	Vector3 rotAroundY(float theta) { return Vector3(cos(theta)*x + (-sin(theta)*z), y, sin(theta)*x + cos(theta)*z); }

	//Rotates the Vector around the Z axis by a given amount.
	Vector3 rotAroundZ(float theta) { return Vector3(cos(theta)*x + (-sin(theta)*y), sin(theta)*x + cos(theta)*y, z); }

	//Retuns the angle between two vectors.
	float getAngleBetween(Vector3 v) { return acos(this->dot(v) / (this->magnitude()*v.magnitude())); }
};

//Maps in to the a number between out_min and out_max.
float map(float in, float in_min, float in_max, float out_min, float out_max)
{
	float slope = (out_max - out_min) / (in_max - in_min);
	return (in - in_min) * slope + out_min;
}

class Ray {
public:
	Vector3 origin;
	Vector3 direction;

	Ray() { origin = Vector3(); direction = Vector3(); }
	Ray(Vector3 o, Vector3 d) { origin = o; direction = d; }
};

//Rotates a vector around an arbitrary axis by a given amount.
Vector3 rotAroundAxis(Vector3 axis, Vector3 vector, float theta) {
	//Find the angles between passed in axis and the x and y axis.
	float thetaX = axis.getAngleBetween(X_AXIS), thetaY = axis.getAngleBetween(Y_AXIS);

	//Rotate the vector around the X and Y axis my the previously found angles. This causes the passsed in axis to be on the Z axis.
	Vector3 u = vector.rotAroundY(thetaY);
	Vector3 v =      u.rotAroundX(thetaX);
	//Rotate the vector around the Z axis by the passed in amount.
	Vector3 w =      v.rotAroundX(theta );
	//Rotate the vector back around the X and Y axis.
	Vector3 g =      w.rotAroundX(-thetaX);
	return           g.rotAroundY(-thetaY);
}

//A simple Image class.
class Image {
public:
	unsigned char* image;
	int width, height, channels;
	Image(char *filename = NULL)
	{
		if (filename != NULL)
		{
			stbi_set_flip_vertically_on_load(false);
			image = stbi_load(filename, &width, &height, &channels, STBI_rgb);
		}
	}
};


//A simple Camera class.
class Camera {
public:
	Vector3 position, direction, up;
	float aperature;
	Camera() {
		position = Vector3(0, 0, -100);
		direction = Vector3(0, 0, 1);
		up = Vector3(0, 1, 0);
		aperature = 10;
	}

	Camera(Vector3 pos, Vector3 dir, Vector3 u, float a) { position = pos; direction = dir; up = u; aperature = a; }
};
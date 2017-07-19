#pragma once
#include "Util.h"
#include "Image.h"

class Sphere {
public:
	Vector3 center;
	float radius;
	Vector3 color;
	Image image;
	bool texMap;
	Sphere(Vector3 c = Vector3(), float r = 1000, Vector3 col = WHITE, bool tm = false, Image img = NULL) { center = c; radius = r; color = col; texMap = tm; image = img; }
	bool intersect(Ray ray, float &t)
	{
		Vector3 p1 = ray.origin;    // Camera location
		Vector3 p2 = ray.direction; // Pixel being traced
		float a = (p2.x - p1.x) * (p2.x - p1.x) + // x
			(p2.y - p1.y) * (p2.y - p1.y) + // y
			(p2.z - p1.z) * (p2.z - p1.z);  // z
		float b = ((p2.x - p1.x) * (p1.x - center.x) +
			(p2.y - p1.y) * (p1.y - center.y) +
			(p2.z - p1.z) * (p1.z - center.z)) * 2;
		float c = (p1.x - center.x) * (p1.x - center.x) +
			(p1.y - center.y) * (p1.y - center.y) +
			(p1.z - center.z) * (p1.z - center.z) -
			(radius * radius);
		float disc = (b * b) - (4 * a * c);

		if (disc < 0)
			return false;

		float t0 = (-b - sqrt(disc)) / (2 * a);
		float t1 = (-b + sqrt(disc)) / (2 * a);

		if (t0 < t1)
			t = t0;
		else
			t = t1;
		return true;
	}
	void getSphereUV(Vector3 normal, float &u, float &v)
	{
		u = atan2(normal.x, normal.z) / (2 * PI) + 0.5f;
		v = normal.y * 0.5f + 0.5f;
	}

	Vector3 getTex(Vector3 normal) {
		float u, v;
		getSphereUV(normal, u, v);
		int s, t;
		map(u, 0, 1, 0, image.width, s);
		map(v, 0, 1, 0, image.height, t);

		unsigned char r = image.image[3 * (t*image.width + s)], g = image.image[3 * (t*image.width + s) + 1], b = image.image[3 * (t*image.width + s) + 2];
		return Vector3((int)r, (int)g, (int)b);

	}
};

class Plane
{
public:
	Vector3 center;
	Vector3 color;
	Vector3 N;
	Plane(Vector3 c = Vector3(0, 0, 0), Vector3 _N = Vector3(1, 1, 1), Vector3 col = Vector3(255, 255, 255)) { center = c; N = _N; color = col; }

	bool intersect(Ray ray, Vector3 &p)
	{
		Vector3 dir = ray.direction - ray.origin;
		float d = ((center - ray.origin).dot(N)) / (dir.dot(N));

		if (d <= 0)
			return false;

		p = dir * d + ray.origin;
		return true;
	}
};

class Quad
{
public:
	Vector3 A, B, C, D, normal, color;
	Image image;
	bool texMap;
	Quad(Vector3 a = Vector3(0, 0, 500), Vector3 b = Vector3(500, 0, 100), Vector3 c = Vector3(500, 500, 100), Vector3 d = Vector3(0, 500, 100),
		Vector3 n = Vector3(0, 0, -1), Vector3 col = WHITE, bool tm = false, Image img = NULL) {
		A = a; B = b; C = c; D = d; normal = n; color = col; texMap = tm;image = img;
	}
	bool intersect(Ray ray, Vector3 &p)
	{
		Vector3 dir = ray.direction - ray.origin;
		Vector3 AB = B - A, AD = D - A;
		float d = ((A - ray.origin).dot(normal)) / (dir.dot(normal));
		Vector3 P = dir * d + ray.origin;
		u = (-A.x * AD.y + A.y * AD.x - AD.x * P.y + AD.y * P.x) / (AB.x * AD.y - AB.y * AD.x);
		v = (-A.x * AB.y + A.y * AB.x - AB.x * P.y + AB.y * P.x) / (AB.y * AD.x - AB.x * AD.y);
		if ((0<u && u<1) && (0<v && v<1))
		{
			p = P;
			return true;
		}
		return false;
	}

	void getQuadUV(Vector3 P, float &u, float &v)
	{
		Vector3 AB = B - A, AD = D - A;
		u = (-A.x * AD.y + A.y * AD.x - AD.x * P.y + AD.y * P.x) / (AB.x * AD.y - AB.y * AD.x);
		v = (-A.x * AB.y + A.y * AB.x - AB.x * P.y + AB.y * P.x) / (AB.y * AD.x - AB.x * AD.y);
	}

	Vector3 getTex(Vector3 pos) {

		int s, t;
		map(u, 0, 1, 0, image.width, s);
		map(v, 0, 1, 0, image.height, t);

		unsigned char r = image.image[3 * (t*image.width + s)], g = image.image[3 * (t*image.width + s) + 1], b = image.image[3 * (t*image.width + s) + 2];
		return Vector3((int)r, (int)g, (int)b);

	}

private:
	float u, v;
};


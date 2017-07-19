#include <stdio.h>
#include "Raytracer.h"
#include "Image.h"


int main() {
	int width = 500, height = 500;
	float stereoOffset = width / 10.0;

	Camera left  = Camera(Vector3(width / 2.0f - stereoOffset, height / 2.0f, -100), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
	Camera right = Camera(Vector3(width / 2.0f + stereoOffset, height / 2.0f, -100), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
	Camera cameras[2] = { left,right };
	RayTracer rt = RayTracer(cameras, width, height);

	float num = 1;
	std::vector<Sphere> spheres;
	spheres.resize(num*num*num);
	std::vector<Quad> quad;

	for (float x = 0; x < num; x++) {
		for (float y = 0; y < num; y++) {
			for (float z = 0; z < num; z++) {
				spheres[x * num * num + y * num + z] = Sphere(Vector3(width/2, height/2, z * 150 + 100), 100.0f, WHITE, true, Image("Images/cobblestone.jpg"));
			}
		}
	}
	
	Vector3 light = Vector3(width / 2.0f, height / 2.0f, -100);

	rt.trace(spheres, quad, light, 25, "Scene/scene13.ppm");

	return 0;
}

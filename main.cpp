#include <stdio.h>
#include "Raytracer.h"
#include "Image.h"


int main() {
	int width = 500, height = 500;
	float stereoOffset = width / 10.0f;

	Camera left  = Camera(Vector3(width / 2.0f - stereoOffset, height / 2.0f, -500), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
	Camera right = Camera(Vector3(width / 2.0f + stereoOffset, height / 2.0f, -500), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
	Camera cameras[2] = { left,right };
	RayTracer rt = RayTracer(cameras, width, height);

	float num = 3;
	std::vector<Sphere> spheres;
	spheres.resize(num*num*num);
	std::vector<Quad> quads;

	for (float x = 0; x < num; x++) {
		for (float y = 0; y < num; y++) {
			for (float z = 0; z < num; z++) {
				spheres[x * num * num + y * num + z] = Sphere(Vector3(x * 150 + 100, y * 150 + 100, z * 150 + 50), 50.0f, WHITE, true, Image("Images/cobblestone.jpg"));
			}
		}
	}
	//spheres[0] = Sphere(Vector3(width/2, height/2, 1250), 1250.0f, WHITE, true, Image("Images/cobblestone.jpg"));
	//Quad quad;
	
	Vector3 light = Vector3(width / 2.0f, height / 2.0f, -100);

	rt.trace(spheres, quads, light, 100, "stereo/stereoTest56.ppm");

	return 0;
}

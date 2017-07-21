#include <stdio.h>
#include "Raytracer.h"
#include "Image.h"


int main() {
	int width = 672, height = 800;
	float stereoOffset = width / 10.0f;

	Camera left  = Camera(Vector3(-stereoOffset, 0, -500), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
	Camera right = Camera(Vector3( stereoOffset, 0, -500), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
	Camera cameras[2] = { left,right };
	RayTracer rt = RayTracer(cameras, width, height);

	float num = 5;
	std::vector<Sphere> spheres;
	spheres.resize(num*num*num);
	std::vector<Quad> quads;

	for (float x = 0; x < num; x++) {
		for (float y = 0; y < num; y++) {
			for (float z = 0; z < num; z++) {
				spheres[x * num * num + y * num + z] = Sphere(Vector3(map(x,0,num-1,-250,250), map(y,0,num-1,-300,300), map(z, 0, num - 1, 0, 600)), 50.0f, WHITE, true, Image("Images/orange-stripes.jpg"));
			}
		}
	}
	//spheres[0] = Sphere(Vector3(width/2, height/2, 1250), 1250.0f, WHITE, true, Image("Images/cobblestone.jpg"));
	//Quad quad;
	
	Vector3 light = Vector3(width / 2.0f, height / 2.0f, -100);

	rt.trace(0, spheres, quads, light, 1000, "stereo/stereoTest59.ppm");

	return 0;
}

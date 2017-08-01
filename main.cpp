#include <stdio.h>
#include "Raytracer.h"


int main() {

	//Set the camera locations and create the Ray Tracer.
	int width = 200, height = 200;
	float stereoOffset = width / 10.0f;

	Camera left  = Camera(Vector3(0, 0, -500), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
	Camera right = Camera(Vector3( stereoOffset, 0, -500), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
	Camera cameras[2] = { left,right };
	RayTracer rt = RayTracer(cameras, width, height);


	//Create the Scene.
	float num = 5;
	std::vector<Sphere> spheres;
	spheres.resize(num*num*num);
	std::vector<Quad> quads;

	for (float x = 0; x < num; x++) {
		for (float y = 0; y < num; y++) {
			for (float z = 0; z < num; z++) {
				spheres[x * num * num + y * num + z] = Sphere(Vector3(map(x,0,num-1,-90,90), map(y,0,num-1,-90,90), map(z, 0, num - 1, 0, 600)), 10.0f, WHITE, true, Image("Images/blue-pentagons.jpg"));
			}
		}
	}
	//spheres[0] = Sphere(Vector3(0, 0, 500), 500.0f, WHITE, false, Image("Images/cobblestone.jpg"));
	//Quad quad;
	
	Vector3 light = Vector3(0, 0, -100);

	//Trace the scene.
	rt.trace(-100, spheres, quads, light, false, 1, "Lens/lens1.ppm");

	return 0;
}

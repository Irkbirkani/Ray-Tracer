#include <stdio.h>
#include "Raytracer.h"
#include <sstream>

int main() {
	int width = 672/4, height = 800/4;
	double stereoOffset = width / 10.0;
	double stepSize = 4.0;
	double distance = width, lensRadius = width/16.0, aperature = 0.001, refractionIndex = 1.1;
	double dStep = (width * 2 - width / 4.0) / stepSize, lrStep = (width / 4.0 - width / 16) / stepSize, riStep = 0.1, aprStep = (0.5- 0.001) / stepSize;
	Image textures[3] = { Image("Images/blue-pentagons.jpg"), Image("Images/sphereTex/jupiter.jpg"), Image("Images/clouds.jpg") };
	Lens lens(Sphere(Vector3(), lensRadius, WHITE, false), refractionIndex);

	Camera left  = Camera(Vector3(), Vector3(0, 0, 1), Vector3(0, 1, 0), aperature);
	Camera right = Camera(Vector3( stereoOffset, 0, -500), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
	Camera cameras[2] = { left,right };
	RayTracer rt = RayTracer(cameras, width, height);


	//Create the Scene.
	double num = 5;
	std::vector<Sphere> spheres;
	spheres.resize(num*num*num);
	std::vector<Quad> quads;

	for (double x = 0; x < num; x++) {
		for (double y = 0; y < num; y++) {
			for (double z = 0; z < num; z++) {
				spheres[x * num * num + y * num + z] = Sphere(Vector3(map(x, 0, num - 1, -(width - 10), width - 10), map(y, 0, num - 1, -(height - 10), height - 10), map(z, 0, num - 1, 100, 600)), 10.0, WHITE, true, textures[(int)(x * num * num + y * num + z) % 3]);
			}
		}
	}

	Vector3 light = Vector3(0, 0, -100);

	std::stringstream  ri(std::stringstream::in | std::stringstream::out);
	std::stringstream spR(std::stringstream::in | std::stringstream::out);
	std::stringstream apr(std::stringstream::in | std::stringstream::out);
	std::stringstream dis(std::stringstream::in | std::stringstream::out);

	for (int i = 0; i <= 6; i++) {
		for (int sphR = 0; sphR < stepSize; sphR++) {
			for (int aperR = 0; aperR < stepSize; aperR++) {
				for (int d = 0; d < stepSize; d++) {
					ri  << refractionIndex;
					spR << lensRadius;
					apr << aperature;
					dis << -distance;
					
					std::string filename = "Lens/batch1/ri_" + ri.str() + "_spR_" + spR.str() + "_apr_" + apr.str() + "_dis_" + dis.str() + ".ppm";
					
					rt.lensTrace(-distance, 60.0 / 180 * PI, 55.0 / 180 * PI, spheres, quads, lens, light, true, 100, filename);

					ri.str(std::string());
					spR.str(std::string());
					apr.str(std::string());
					dis.str(std::string());
					distance += dStep;
				}
				aperature +=aprStep;
			}
			lensRadius += lrStep;
		}
		refractionIndex += riStep;
	}
	
	
	//Trace the scene.
	//rt.lensTrace(-distance, 60.0/180*PI, 55.0/180*PI, spheres, quads, lens, light, true, 10, "Lens/lens20.ppm");

	return 0;
}

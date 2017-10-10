#include <stdio.h>
#include "Raytracer.h"
#include <sstream>

int main() {
	int width = 168, height = 200;
	double stereoOffset = width / 10.0;
	double stepSize = 10.0;
	double disMult = 2.0;
	double distance = 0.0, lensRadius = width/16.0, aperature = 0.01, refractionIndex = 1.0;
	double dStep = 0.1, lrStep = (width / 2.0 - width / 16) / stepSize, riStep = 0.1;
	Image textures[3] = { Image("Images/blue-pentagons.jpg"), Image("Images/sphereTex/jupiter.jpg"), Image("Images/eyechart.png") };
	Lens lens(Sphere(Vector3(), lensRadius, WHITE, false), Sphere(Vector3(), lensRadius, WHITE, false), refractionIndex);

	Camera left  = Camera(Vector3(), Vector3(0, 0, 1), Vector3(0, 1, 0), aperature);
	Camera right = Camera(Vector3( stereoOffset, 0, -500), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
	Camera cameras[2] = { left,right };
	RayTracer rt = RayTracer(cameras, width, height);


	//Create the Scene.
	double num = 3;
	std::vector<Sphere> spheres; 
	spheres.resize(num*num*num);
	std::vector<Quad> quads;

	/*for (double x = 0; x < num; x++) {
		for (double y = 0; y < num; y++) {
			for (double z = 0; z < num; z++) {
				spheres[x * num * num + y * num + z] = Sphere(Vector3(map(x, 0, num - 1, -250, 250), map(y, 0, num - 1, -250, 250), map(z, 0, num - 1, 300, 600)), 125.0, WHITE, true, textures[(int)(x * num * num + y * num + z) % 3]);
			}
		}
	}
*/
	spheres.resize(5);
	spheres[0] = Sphere(Vector3(-200, -200, 300), 125.0, WHITE, true, textures[0]);
	spheres[1] = Sphere(Vector3(-67,  -67,  400), 125.0, WHITE, true, textures[1]);
	spheres[2] = Sphere(Vector3( 67,   67,  500), 125.0, WHITE, true, textures[2]);
	spheres[3] = Sphere(Vector3( 200,  200, 600), 125.0, WHITE, true, textures[0]);
	spheres[4] = Sphere(Vector3( 333,  333, 700), 125.0, WHITE, true, textures[1]);

	Vector3 light = Vector3(0, 0, -100);

	std::stringstream  ri(std::stringstream::in | std::stringstream::out);
	std::stringstream spR(std::stringstream::in | std::stringstream::out);
	//std::stringstream apr(std::stringstream::in | std::stringstream::out);
	std::stringstream dist(std::stringstream::in | std::stringstream::out);

	std::string filename;

	/*lens.lens[0].center.z = -lensRadius * 3 / 2.0;
	lens.lens[1].center.z = lensRadius * 3 / 2.0;

	filename = "Lens/nonSphere/Concave3.2_ri_" + ri.str() + "_spR_" + spR.str() + "_dis_" + dist.str() + ".ppm";
	rt.concaveLensTrace(-width * 10, 60.0 / 180 * PI, 55.0 / 180 * PI, spheres, quads, lens, light, true, 100, filename);
*/
	for (int i = 0; i < 6; i++) {
		for (int sphR = 0; sphR < stepSize; sphR++) {
			for (int dis = 5; dis < 10; dis++) {
				ri << lens.refracIdx;
				spR << lens.lens[0].radius;
				dist << distance;

				if (dis <= 4) {
					filename = "Lens/nonSphere/batch2/Convex_ri_" + ri.str() + "_spR_" + spR.str() + "_dis_" + dist.str() + ".ppm";
					rt.convexLensTrace(-width, 60.0 / 180 * PI, 55.0 / 180 * PI, spheres, quads, lens, light, true, 100, filename);
				}
				else {
					filename = "Lens/nonSphere/batch3.5/Concave_ri_" + ri.str() + "_spR_" + spR.str() + "_dis_" + dist.str() + ".ppm";
					rt.concaveLensTrace(-width*10, 60.0 / 180 * PI, 55.0 / 180 * PI, spheres, quads, lens, light, true, 100, filename);
				}

				ri.str(std::string());
				spR.str(std::string());
				dist.str(std::string());

				disMult += dStep;
				distance =  disMult * lens.lens[0].radius;
				lens.lens[0].center.z = -distance / 2.0;
				lens.lens[1].center.z =  distance / 2.0;
			}
			distance = 0.0;
			disMult = 2.0;
			lens.lens[0].center.z = 0;
			lens.lens[1].center.z = 0;
			lens.lens[0].radius += lrStep;
			lens.lens[1].radius += lrStep;
		}
		lens.lens[0].radius = width / 16;
		lens.lens[1].radius = width / 16;
		lens.refracIdx += riStep;
	}

	//for (int i = 0; i <= 6; i++) {
	//	for (int sphR = 0; sphR < stepSize; sphR++) {
	//		for (int aperR = 0; aperR < stepSize; aperR++) {
	//			//for (int d = 0; d < stepSize; d++) {
	//				ri  << lens.refracIdx;
	//				spR << lens.lens[0].radius;
	//				apr << rt.cameras[0].aperature;
	//				//dis << -distance;
	//				
	//				std::string filename = "Lens/batch4/ri_" + ri.str() + "_spR_" + spR.str() + "_apr_" + apr.str() + ".ppm";
	//				
	//				rt.sphereLensTrace(-width, 60.0 / 180 * PI, 55.0 / 180 * PI, spheres, quads, lens, light, true, 100, filename);

	//				ri.str(std::string());
	//				spR.str(std::string());
	//				apr.str(std::string());
	//				//dis.str(std::string());
	//				//distance += dStep;
	//			//}
	//			//distance = width;
	//			rt.cameras[0].aperature += aprStep;
	//		}
	//		rt.cameras[0].aperature = .001;
	//		lens.lens[0].radius += lrStep;
	//	}
	//	lens.lens[0].radius = width / 16;
	//	lens.refracIdx += riStep;
	//}
	
	
	//Trace the scene.
	//rt.lensTrace(-distance, 60.0/180*PI, 55.0/180*PI, spheres, quads, lens, light, true, 100, "Lens/lens33.ppm");

	return 0; 
}

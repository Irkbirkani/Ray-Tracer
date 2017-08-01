#pragma once
#include <algorithm>
#include <fstream>
#include <vector>

#include "Shape.h"



class RayTracer {
public:
	Camera cameras[2]; //0 = Left eye, 1 = right eye
	int width, height;

	RayTracer() { cameras[0] = Camera(); cameras[1] = Camera(); width = 500; height = 500; }
	RayTracer(Camera cam[2], float w, float h) { cameras[0] = cam[0], cameras[1] = cam[1]; width = w; height = h; }

	/* 
	Render a stereo image of the scene. 
		Arguments:
			z:       The depth of the image plane.
			spheres: A vector of Spheres comprising in the scene.
			quads:   A vector of quads comprising the scene.
			light:   The lights position. Currently only supporting one light source.
			DoF:     Enables Depth of Field.
			samples: The number of samples taken for DoF. Set to 1 if DoF is not enabled.
			file:    The filepath for the file you wish to write to. Must be a .ppm.
	*/
	void stereoTrace(float z, std::vector<Sphere> spheres, std::vector<Quad> quads, Vector3 light, bool DoF, int samples, const char * file)
	{
		//Open the output stream and set the paramaters for the ppm file.
		std::ofstream out(file);
		out << "P3\n" << width * 2 << ' ' << height << ' ' << "255\n";

		//Create color and set the camera to the left eye.
		Vector3 color;
		Camera camera = cameras[0];

		//Calculate the stereo offset for the left eye.
		float offset = -width / 10.0f;
		//Find adjustment amout for x and y.
		float w = width / 2 + (width / 2 * z) / -camera.position.z, h = height / 2 + (height / 2 * z) / -camera.position.z;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width * 2; x++) {

				//switch cameras for stereo
				if (x == width) {
					camera = cameras[1];
					offset = width / 10.0;
				}
				//reset color
				color = Vector3();
				for (int s = 0; s < samples; s++) {

					//Create newPos and find newX and newY.
					Vector3 newPos = camera.position;
					float newX = (x % width) * (2 * w / width) - w + offset, newY = y*(2 * h / height) - h;
					//Check if Depth of field is enabled. If it is find a new random camera location and set newPos to that location.
					if (DoF) {
						float t = 2 * PI * (rand() / (float)RAND_MAX) * camera.aperature;
						float u = (rand() / (float)RAND_MAX)*camera.aperature + (rand() / (float)RAND_MAX) * camera.aperature;
						float r = u > camera.aperature ? 2 * camera.aperature - u : u;
						//create new ray
						newPos = Vector3(camera.position.x + (r*cos(t)), camera.position.y + (r*sin(t)), camera.position.z);
					}
					Ray ray = Ray(newPos, Vector3(newX, newY, z) - newPos);

					float spT, qdT;

					//Find the closest sphere and quad in the scene.
					Sphere *sph = checkSphereIntersect(ray, spheres);
					Quad   *qd = checkQuadIntersect(ray, quads);

					//Check and see if sph and qd are not nullptrs. Of they are then there was no intersections.
					bool sphInter = false;
					if (sph != nullptr) 
						sphInter = sph->intersect(ray, spT);
					
					bool qdInter = false;
					if (qd != nullptr)
						qdInter = qd->intersect(ray, qdT);

					//If there are intersections between both a sphere and a quad then return the color of the one that is closer.
					if (sphInter && qdInter) {
						if (spT < qdT) {
							Vector3 pos = ray.origin + ray.direction * spT;
							Vector3 norm = (pos - sph->center).normalize();
							float dif = std::max(0.0f, norm.dot((light - pos).normalize()));
							if (sph->texMap)
								color = color + sph->getTex(norm) * dif;
							else
								color = color + sph->color *dif;
						}
						else {
							Vector3 pos = ray.direction * qdT + ray.origin;
							float dif = std::max(0.0f, qd->normal.dot((light - pos).normalize()));
							if (qd->texMap)
								color = color + qd->getTex(pos) * dif;
							else
								color = color + qd->color * dif;
						}
					}

					//Return the color of a sphere.
					if (sphInter&&!qdInter)
					{
						Vector3 pos = ray.origin + ray.direction * spT;
						Vector3 norm = (pos - sph->center).normalize();
						float dif = std::max(0.0f, norm.dot((light - pos).normalize()));
						if (sph->texMap)
							color = color + sph->getTex(norm) * dif;
						else
							color = color + sph->color *dif;
					}
					else 
						color = color + BLACK;


					//return the color of a quad.
					 if (qdInter&&!sphInter)
					{
						 Vector3 pos = ray.direction * qdT + ray.origin;
						 float dif = std::max(0.0f, qd->normal.dot((light - pos).normalize()));
						 if (qd->texMap)
							 color = color + qd->getTex(pos) * dif;
						 else
							 color = color + qd->color * dif;
					}
					else
						color = color + BLACK;
				}

				// write out color
				out << (int)color.x / samples << ' '
					<< (int)color.y / samples << ' '
					<< (int)color.z / samples << '\n';
			}
			//reset cameras
			camera = cameras[0];
			offset = -width / 10.0f;
		}
	}

	/*
	Render an image of the scene. Uses camera[0] for the default camera.
		Arguments:
			z:       The depth of the image plane.
			spheres: A vector of Spheres comprising in the scene.
			quads:   A vector of quads comprising the scene.
			light:   The lights position. Currently only supporting one light source.
			DoF:     Enables Depth of Field.
			samples: The number of samples taken for DoF. Set to 1 if DoF is not enabled.
			file:    The filepath for the file you wish to write to. Must be a .ppm.
	*/
	void trace(float z, std::vector<Sphere> spheres, std::vector<Quad> quads, Vector3 light, bool DoF, int samples, const char * file)
	{
		//Open the output stream and set the paramaters for the ppm file.
		std::ofstream out(file);
		out << "P3\n" << width << ' ' << height << ' ' << "255\n";

		//Create color and set the camera to the left eye.
		Vector3 color;
		Camera camera = cameras[0];

		//Find adjustment amout for x and y.
		float w = width / 2 + (width / 2 * z) / -camera.position.z, h = height / 2 + (height / 2 * z) / -camera.position.z;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				//reset color
				color = Vector3(0, 0, 0);
				for (int s = 0; s < samples; s++) {

					//Create newPos and find newX and newY.
					Vector3 newPos = camera.position;
					float newX = x*(2 * w / width) - w, newY = y*(2 * h / height) - h;

					//Check if Depth of field is enabled. If it is find a new random camera location and set newPos to that location.
					if (DoF) {
						//find new random camera position
						float t = 2 * PI * (rand() / (float)RAND_MAX) * camera.aperature;
						float u = (rand() / (float)RAND_MAX)*camera.aperature + (rand() / (float)RAND_MAX) * camera.aperature;
						float r = u > camera.aperature ? 2 * camera.aperature - u : u;
						newPos = Vector3(camera.position.x + (r*cos(t)), camera.position.y + (r*sin(t)), camera.position.z);
					}
					//Create the new ray.
					Ray ray = Ray(newPos, Vector3(newX, newY, z) - newPos);

					float spT, qdT;

					//Find the closest sphere and quad in the scene.
					Sphere *sph = checkSphereIntersect(ray, spheres);
					Quad   *qd = checkQuadIntersect(ray, quads);

					//Check and see if sph and qd are not nullptrs. Of they are then there was no intersections.
					bool sphInter = false;
					if (sph != nullptr)
						sphInter = sph->intersect(ray, spT);
					
					bool qdInter = false;
					if (qd != nullptr)
						qdInter = qd->intersect(ray, qdT);

					//If there are intersections between both a sphere and a quad then return the color of the one that is closer.
					if (sphInter && qdInter) {
						if (spT < qdT) {
							Vector3 pos = ray.origin + ray.direction * spT;
							Vector3 norm = (pos - sph->center).normalize();
							float dif = std::max(0.0f, norm.dot((light - pos).normalize()));
							if (sph->texMap)
								color = color + sph->getTex(norm) * dif;
							else
								color = color + sph->color *dif;
						}
						else {
							Vector3 pos = ray.direction * qdT + ray.origin;
							float dif = std::max(0.0f, qd->normal.dot((light - pos).normalize()));
							if (qd->texMap)
								color = color + qd->getTex(pos) * dif;
							else
								color = color + qd->color * dif;
						}
					}

					//Return the color of a sphere.
					if (sphInter && !qdInter)
					{
						Vector3 pos = ray.origin + ray.direction * spT;
						Vector3 norm = (pos - sph->center).normalize();
						float dif = std::max(0.0f,norm.dot((light - pos).normalize()));
						
							//printf("dif = %f\n", norm.dot((light - pos).normalize()));
						if (sph->texMap)
							color = color + sph->getTex(norm) * dif;
						else
							color = color + sph->color *dif;
					}
					else
						color = color + BLACK;

					//return the color of a quad.
					if (qdInter && !sphInter)
					{
						Vector3 pos = ray.direction * qdT + ray.origin;
						float dif = std::max(0.0f, qd->normal.dot((light - pos).normalize()));
						if (qd->texMap)
							color = color + qd->getTex(pos) * dif;
						else
							color = color + qd->color * dif;
					}
					else
						color = color + BLACK;
				}

				// write out color
				out << (int)color.x / samples << ' '
					<< (int)color.y / samples << ' '
					<< (int)color.z / samples << '\n';
			}
		}
	}

	/*
	Render an image of the scene through a lens. Uses camera[0] for the default camera.
	Arguments:
	z:       The depth of the image plane.
	spheres: A vector of Spheres comprising in the scene.
	quads:   A vector of quads comprising the scene.
	lens:    The lens distorting the image.
	light:   The lights position. Currently only supporting one light source.
	DoF:     Enables Depth of Field.
	samples: The number of samples taken for DoF. Set to 1 if DoF is not enabled.
	file:    The filepath for the file you wish to write to. Must be a .ppm.
	*/
	void lensTrace(float z, std::vector<Sphere> spheres, std::vector<Quad> quads, Lens lens, Vector3 light, bool DoF, int samples, const char * file)
	{
		//Open the output stream and set the paramaters for the ppm file.
		std::ofstream out(file);
		out << "P3\n" << width << ' ' << height << ' ' << "255\n";

		//Create color and set the camera to the left eye.
		Vector3 color;
		Camera camera = cameras[0];

		//Find adjustment amout for x and y.
		float w = width / 2 + (width / 2 * z) / -camera.position.z, h = height / 2 + (height / 2 * z) / -camera.position.z;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				//reset color
				color = Vector3(0, 0, 0);
				for (int s = 0; s < samples; s++) {

					//Create newPos and find newX and newY.
					Vector3 newPos = camera.position;
					float newX = x*(2 * w / width) - w, newY = y*(2 * h / height) - h;

					//Check if Depth of field is enabled. If it is find a new random camera location and set newPos to that location.
					if (DoF) {
						//find new random camera position
						float t = 2 * PI * (rand() / (float)RAND_MAX) * camera.aperature;
						float u = (rand() / (float)RAND_MAX)*camera.aperature + (rand() / (float)RAND_MAX) * camera.aperature;
						float r = u > camera.aperature ? 2 * camera.aperature - u : u;
						newPos = Vector3(camera.position.x + (r*cos(t)), camera.position.y + (r*sin(t)), camera.position.z);
					}
					//Create the new ray.
					Ray ray = Ray(Vector3(newX, newY, z), newPos - Vector3(newX, newY, z));
					ray = lens.refract(ray);

					float spT, qdT;

					//Find the closest sphere and quad in the scene.
					Sphere *sph = checkSphereIntersect(ray, spheres);
					Quad   *qd = checkQuadIntersect(ray, quads);

					//Check and see if sph and qd are not nullptrs. Of they are then there was no intersections.
					bool sphInter = false;
					if (sph != nullptr)
						sphInter = sph->intersect(ray, spT);

					bool qdInter = false;
					if (qd != nullptr)
						qdInter = qd->intersect(ray, qdT);

					//If there are intersections between both a sphere and a quad then return the color of the one that is closer.
					if (sphInter && qdInter) {
						if (spT < qdT) {
							Vector3 pos = ray.origin + ray.direction * spT;
							Vector3 norm = (pos - sph->center).normalize();
							float dif = std::max(0.0f, norm.dot((light - pos).normalize()));
							if (sph->texMap)
								color = color + sph->getTex(norm) * dif;
							else
								color = color + sph->color *dif;
						}
						else {
							Vector3 pos = ray.direction * qdT + ray.origin;
							float dif = std::max(0.0f, qd->normal.dot((light - pos).normalize()));
							if (qd->texMap)
								color = color + qd->getTex(pos) * dif;
							else
								color = color + qd->color * dif;
						}
					}

					//Return the color of a sphere.
					if (sphInter && !qdInter)
					{
						Vector3 pos = ray.origin + ray.direction * spT;
						Vector3 norm = (pos - sph->center).normalize();
						float dif = std::max(0.0f, norm.dot((light - pos).normalize()));

						//printf("dif = %f\n", norm.dot((light - pos).normalize()));
						if (sph->texMap)
							color = color + sph->getTex(norm) * dif;
						else
							color = color + sph->color *dif;
					}
					else
						color = color + BLACK;

					//return the color of a quad.
					if (qdInter && !sphInter)
					{
						Vector3 pos = ray.direction * qdT + ray.origin;
						float dif = std::max(0.0f, qd->normal.dot((light - pos).normalize()));
						if (qd->texMap)
							color = color + qd->getTex(pos) * dif;
						else
							color = color + qd->color * dif;
					}
					else
						color = color + BLACK;
				}

				// write out color
				out << (int)color.x / samples << ' '
					<< (int)color.y / samples << ' '
					<< (int)color.z / samples << '\n';
			}
		}
	}

	//Find the closest sphere with an intersection with the ray.
	Sphere* checkSphereIntersect(Ray ray, std::vector<Sphere> &spheres) {
		Sphere *sOut = nullptr;
		float minSphere = 10000000000, t;
		for (int s = 0; s < spheres.size(); s++) {
			if (spheres[s].intersect(ray, t)) {
				if (t < minSphere) {
					minSphere = t;
					sOut = &spheres[s];
				}
			}
		}
		return sOut;
	}

	//Find the closest quad with an intersection with the ray.
	Quad* checkQuadIntersect(Ray ray, std::vector<Quad> quads) {
		Quad* qOut = nullptr;
		float minQuad = 10000000000, t;
		for (int q = 0; q < quads.size(); q++) {
			if (quads[q].intersect(ray, t)) {
				if (t < minQuad) {
					minQuad = t;
					qOut = &quads[q];
				}
			}
		}
		return qOut;
	}
};

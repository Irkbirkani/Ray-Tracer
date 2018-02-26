#pragma once
#include <algorithm>
#include <fstream>
#include <vector>

#include "Shape.h"

using std::vector;
using std::string;

class RayTracer {
public:
    Camera camera; //0 = Left eye, 1 = right eye
    int width, height;

    RayTracer() { camera = Camera(); width = 500; height = 500; }
    RayTracer(Camera cam, double w, double h) { camera = cam; width = w; height = h; }

    /*
     * Render an image of the scene. Uses cameras[0] for the default camera.
     *     Arguments:
     *         z:         The depth of the image plane.
     *         spheres:   A vector of Spheres in the scene.
     *         quads:     A vector of quads in the scene.
     *         light:     The lights position. Currently only supporting one light source.
     *         stereo:    Enables stereo image rendering. 
     *         DoF:       Enables Depth of Field.
     *         samples:   The number of samples taken for DoF. Set to 1 if DoF is not enabled.
     *         file:      The filepath for the file you wish to write to. Must be a .ppm.
     *         traceType: The type of trace you wish to run. 
     *                    Includes TRACE, SPHERE, PCONCAVE, BICONCAVE, PCONVEX, and BICONVEX.
     *                    See Util.h for destrictions of traceTypes.
    */
    void trace(double z, vector<Sphere> spheres, vector<Quad> quads, Lens lens, Vector3 light, bool stereo, bool DoF, int samples, string file, int traceType)
    {
        Camera leftCamera, rightCamera;
        Lens   leftLens = lens, rightLens = lens;
        double offset;
        if(stereo) {
            offset = width / 10.0;
            leftCamera = Camera(Vector3(camera.position.x+offset, camera.position.y, camera.position.x),
                               camera.direction, camera.up, camera.aperature);
            leftLens.changePos(offset);
            rightCamera = Camera(Vector3(camera.position.x-offset, camera.position.y, camera.position.x),
                               camera.direction, camera.up, camera.aperature);
            rightLens.changePos(-offset);
        }        

        //Open the output stream and set the paramaters for the ppm file.
        std::ofstream out(file);
        if(stereo)
            out << "P3\n" << width*2 << ' ' << height << ' ' << "255\n";
        else
            out << "P3\n" << width << ' ' << height << ' ' << "255\n";

        //Create color and set the camera to the left eye.
        Vector3 color;
        
        //Find adjustment amout for x and y.
        double center;
        if(traceType == PCONVEX || traceType == PCONCAVE)
            center = abs(lens.lens[0].center.z - lens.plane.center.z);
        else
            center = (abs(lens.lens[0].center.z - lens.lens[0].radius) + abs(lens.lens[1].center.z - lens.lens[1].radius)) / 2;
        double theta=atan(lens.lens[1].radius/center);
        double v = -z*tan(theta);
        double theta2 = atan((width/2.0)/(height/2.0));
        double h = (v*cos(theta2));
        double w = (v*sin(theta2));

        double newWidth = stereo ? width*2 : width;
        for (int y = 0; y < height; y++) {
            if(stereo) {
                offset = width / 10.0;
                camera = leftCamera;
                lens = leftLens;
            }
            for (int x = 0; x < newWidth; x++) {

                if(stereo && x == width) {
                    offset = -width / 10.0;
                    camera = rightCamera;
                    lens = rightLens;
                }
                //reset color
                color = Vector3(0, 0, 0);
                for (int s = 0; s < samples; s++) {

                    //Create newPos and find newX and newY.
                    Vector3 newPos = camera.position;
                    double newX, newY;

                    if (stereo)
                        newX = (x % width) * (2 * w /width) - w + offset;
                    else 
                        newX = x*(2 * w / width) - w;
                    
                    newY = y*(2*h / height) - h;

                    //Check if Depth of field is enabled. If it is find a new random camera location and set newPos to that location.
                    if (DoF) {
                        //find new random camera position
                        double t = 2 * PI * (rand() / (double)RAND_MAX) * camera.aperature;
                        double u = (rand() / (double)RAND_MAX)*camera.aperature + (rand() / (double)RAND_MAX) * camera.aperature;
                        double r = u > camera.aperature ? 2 * camera.aperature - u : u;
                        newPos = Vector3(camera.position.x + (r*cos(t)), camera.position.y + (r*sin(t)), camera.position.z);
                    }
                    //Create the new ray.
                    Ray ray;
                    switch(traceType) {
                        case TRACE:     ray = Ray(newPos, Vector3(newX, newY, z) - newPos);
                                        break;
                        case SPHERE:    ray = lens.sphereRefract(Ray(Vector3(newX, newY, z), (newPos - Vector3(newX, newY, z)).normalize()));
                                        break;
                        case BICONCAVE: ray = lens.refract(Ray(Vector3(newX, newY, z), (newPos - Vector3(newX, newY, z)).normalize()), BICONCAVE);
                                        break;
                        case BICONVEX:  ray = lens.refract(Ray(Vector3(newX, newY, z), (newPos - Vector3(newX, newY, z)).normalize()), BICONVEX);
                                        break;
                        case PCONVEX:   ray = lens.refract(Ray(Vector3(newX, newY, z), (newPos - Vector3(newX, newY, z)).normalize()), PCONVEX);
                                        break;
                        case PCONCAVE:  ray = lens.refract(Ray(Vector3(newX, newY, z), (newPos - Vector3(newX, newY, z)).normalize()), PCONCAVE);
                                        break;
                    }

                    double spT, qdT;

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
                            double dif = std::max(0.0, norm.dot((light - pos).normalize()));
                            if (sph->texMap)
                                color = color + sph->getTex(norm) * dif;
                            else
                                color = color + sph->color *dif;
                        }
                        else {
                            Vector3 pos = ray.direction * qdT + ray.origin;
                            double dif = std::max(0.0, qd->normal.dot((light - pos).normalize()));
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
                        double dif = std::max(0.0,norm.dot((light - pos).normalize()));

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
                        double dif = std::max(0.0, qd->normal.dot((light - pos).normalize()));
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
    Sphere* checkSphereIntersect(Ray ray, vector<Sphere> &spheres) {
        Sphere *sOut = nullptr;
        double minSphere = 10000000000, t;
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
    Quad* checkQuadIntersect(Ray ray, vector<Quad> quads) {
        Quad* qOut = nullptr;
        double minQuad = 10000000000, t;
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

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
    RayTracer(Camera cam[2], double w, double h) { cameras[0] = cam[0], cameras[1] = cam[1]; width = w; height = h; }

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
    void stereoTrace(double z, std::vector<Sphere> spheres, std::vector<Quad> quads, Vector3 light, bool DoF, int samples, std::string file)
    {
        //Open the output stream and set the paramaters for the ppm file.
        std::ofstream out(file);
        out << "P3\n" << width * 2 << ' ' << height << ' ' << "255\n";

        //Create color and set the camera to the left eye.
        Vector3 color;
        Camera camera = cameras[0];

        //Calculate the stereo offset for the left eye.
        double offset = -width / 10.0;
        //Find adjustment amout for x and y.
        double w = width / 2 + (width / 2 * z) / -camera.position.z, h = height / 2 + (height / 2 * z) / -camera.position.z;
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
                    double newX = (x % width) * (2 * w / width) - w + offset, newY = y*(2 * h / height) - h;
                    //Check if Depth of field is enabled. If it is find a new random camera location and set newPos to that location.
                    if (DoF) {
                        double t = 2 * PI * (rand() / (double)RAND_MAX) * camera.aperature;
                        double u = (rand() / (double)RAND_MAX)*camera.aperature + (rand() / (double)RAND_MAX) * camera.aperature;
                        double r = u > camera.aperature ? 2 * camera.aperature - u : u;
                        //create new ray
                        newPos = Vector3(camera.position.x + (r*cos(t)), camera.position.y + (r*sin(t)), camera.position.z);
                    }
                    Ray ray = Ray(newPos, Vector3(newX, newY, z) - newPos);

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
                    if (sphInter&&!qdInter)
                    {
                        Vector3 pos = ray.origin + ray.direction * spT;
                        Vector3 norm = (pos - sph->center).normalize();
                        double dif = std::max(0.0, norm.dot((light - pos).normalize()));
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
            //reset cameras
            camera = cameras[0];
            offset = -width / 10.0;
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
    void trace(double z, std::vector<Sphere> spheres, std::vector<Quad> quads, Vector3 light, bool DoF, int samples, std::string file)
    {
        //Open the output stream and set the paramaters for the ppm file.
        std::ofstream out(file);
        out << "P3\n" << width << ' ' << height << ' ' << "255\n";

        //Create color and set the camera to the left eye.
        Vector3 color;
        Camera camera = cameras[0];

        //Find adjustment amout for x and y.
        double w = width / 2 + (width / 2 * z) / -camera.position.z, h = height / 2 + (height / 2 * z) / -camera.position.z;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                //reset color
                color = Vector3(0, 0, 0);
                for (int s = 0; s < samples; s++) {

                    //Create newPos and find newX and newY.
                    Vector3 newPos = camera.position;
                    double newX = x*(2 * w / width) - w, newY = y*(2 * h / height) - h;

                    //Check if Depth of field is enabled. If it is find a new random camera location and set newPos to that location.
                    if (DoF) {
                        //find new random camera position
                        double t = 2 * PI * (rand() / (double)RAND_MAX) * camera.aperature;
                        double u = (rand() / (double)RAND_MAX)*camera.aperature + (rand() / (double)RAND_MAX) * camera.aperature;
                        double r = u > camera.aperature ? 2 * camera.aperature - u : u;
                        newPos = Vector3(camera.position.x + (r*cos(t)), camera.position.y + (r*sin(t)), camera.position.z);
                    }
                    //Create the new ray.
                    Ray ray = Ray(newPos, Vector3(newX, newY, z) - newPos);

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

    /*
    Render an image of the scene through a sphere lens. Uses camera[0] for the default camera.
    Arguments:
    z:       The depth of the image plane.
    spheres: A vector of Spheres comprising in the scene.
    quads:   A vector of quads comprising the scene.
    lens:    The lens the rays are passed through.
    light:   The lights position. Currently only supporting one light source.
    DoF:     Enables Depth of Field.
    samples: The number of samples taken for DoF. Set to 1 if DoF is not enabled.
    file:    The filepath for the file you wish to write to. Must be a .ppm.
    */
    void sphereLensTrace(double z,double vertFov, double horizFov, std::vector<Sphere> spheres, std::vector<Quad> quads, Lens lens, Vector3 light, bool DoF, int samples, std::string file)
    {
        //Open the output stream and set the paramaters for the ppm file.
        std::ofstream out(file);
        out << "P3\n" << width << ' ' << height << ' ' << "255\n";

        //Create color and set the camera to the left eye.
        Vector3 color;
        Camera camera = cameras[0];

        //Find adjustment amout for x and y.
        double w = -z*tan(horizFov), h = -z*tan(vertFov);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                //reset color
                color = Vector3(0, 0, 0);
                for (int s = 0; s < samples; s++) {

                    //Create newPos and find newX and newY.
                    Vector3 newPos = camera.position;
                    double newX = map(x, 0, width, -w, w), newY = map(y, 0, height, -h, h);

                    //Check if Depth of field is enabled. If it is find a new random camera location and set newPos to that location.
                    if (DoF) {
                        //find new random camera position
                        double t = 2 * PI * (rand() / (double)RAND_MAX) * camera.aperature;
                        double u = (rand() / (double)RAND_MAX)*camera.aperature + (rand() / (double)RAND_MAX) * camera.aperature;
                        double r = u > camera.aperature ? 2 * camera.aperature - u : u;
                        newPos = Vector3(camera.position.x + (r*cos(t)), camera.position.y + (r*sin(t)), camera.position.z);
                    }
                    //Create the new ray
                    Ray ray = lens.sphereRefract(Ray(Vector3(newX, newY, z), newPos - Vector3(newX, newY, z)));

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
                        double dif = std::max(0.0, norm.dot((light - pos).normalize()));

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

    /*
    Render an image of the scene through a concave lens. Uses camera[0] for the default camera.
    Arguments:
    z:       The depth of the image plane.
    spheres: A vector of Spheres comprising in the scene.
    quads:   A vector of quads comprising the scene.
    lens:    The lens the rays are passed through.
    light:   The lights position. Currently only supporting one light source.
    DoF:     Enables Depth of Field.
    samples: The number of samples taken for DoF. Set to 1 if DoF is not enabled.
    file:    The filepath for the file you wish to write to. Must be a .ppm.
    */
    void convexLensTrace(double z, double vertFov, double horizFov, std::vector<Sphere> spheres, std::vector<Quad> quads, Lens lens, Vector3 light, bool DoF, int samples, std::string file)
    {
        //Open the output stream and set the paramaters for the ppm file.
        std::ofstream out(file);
        out << "P3\n" << width << ' ' << height << ' ' << "255\n";

        //Create color and set the camera to the left eye.
        Vector3 color;
        Camera camera = cameras[0];

        //std::ofstream out1(file+".txt");

        //Find adjustment amout for x and y.
        double theta=atan(lens.lens[1].radius/(std::abs(lens.lens[1].center.z - camera.position.z)));
        double v = -z*tan(theta);
        double theta2 = atan((width/2.0)/(height/2.0));
        double h = (v*cos(theta2)), w = (v*sin(theta2));
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                //reset color
                color = Vector3(0, 0, 0);
                for (int s = 0; s < samples; s++) {

                    //Create newPos and find newX and newY.
                    Vector3 newPos = camera.position;
                    double newX = map(x, 0, width, -w, w), newY = map(y, 0, height, -h, h);

                    //Check if Depth of field is enabled. If it is find a new random camera location and set newPos to that location.
                    if (DoF) {
                        //find new random camera position
                        double t = 2 * PI * (rand() / (double)RAND_MAX) * camera.aperature;
                        double u = (rand() / (double)RAND_MAX)*camera.aperature + (rand() / (double)RAND_MAX) * camera.aperature;
                        double r = u > camera.aperature ? 2 * camera.aperature - u : u;
                        newPos = Vector3(camera.position.x + (r*cos(t)), camera.position.y + (r*sin(t)), camera.position.z);
                    }
                    //Create the new ray
                    Vector3 newRayDir = (newPos - Vector3(newX, newY, z)).normalize();
                    //out1 << "Incoming Ray: origin: Vector(" << newX << ", " << newY << ", " << z
                    //     << ")  direction: Vector(" << newRayDir.x << ", " << newRayDir.y << ", " << newRayDir.z << ")\n";
                    Ray ray = lens.convexRefract(Ray(Vector3(newX, newY, z), newRayDir));

                    //out1 << "Outgoing Ray: origin: Vector(" << ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z
                    //     << ") direction: Vector(" << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << ")\n\n";

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
                        double dif = std::max(0.0, norm.dot((light - pos).normalize()));

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

    /*
    Render an image of the scene through a concave lens. Uses camera[0] for the default camera.
    Arguments:
    z:       The depth of the image plane.
    spheres: A vector of Spheres comprising in the scene.
    quads:   A vector of quads comprising the scene.
    lens:    The lens the rays are passed through.
    light:   The lights position. Currently only supporting one light source.
    DoF:     Enables Depth of Field.
    samples: The number of samples taken for DoF. Set to 1 if DoF is not enabled.
    file:    The filepath for the file you wish to write to. Must be a .ppm.
    */
    void concaveLensTrace(double z, double vertFov, double horizFov, std::vector<Sphere> spheres, std::vector<Quad> quads, Lens lens, Vector3 light, bool DoF, int samples, std::string file)
    {
        //Open the output stream and set the paramaters for the ppm file.
        std::ofstream out(file);
        out << "P3\n" << width << ' ' << height << ' ' << "255\n";

        //Create color and set the camera to the left eye.
        Vector3 color;
        Camera camera = cameras[0];

        //Find adjustment amout for x and y.
        double theta=atan(lens.lens[1].radius/(std::abs(lens.lens[1].center.z - camera.position.z)));
        double v = -z*tan(theta);
        double theta2 = atan((width/2.0)/(height/2.0));
        double h = (v*cos(theta2)), w = (v*sin(theta2));
        //double w = v, h = v;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                //reset color
                color = Vector3(0, 0, 0);
                for (int s = 0; s < samples; s++) {

                    //Create newPos and find newX and newY.
                    Vector3 newPos = camera.position;
                    double newX = map(x, 0, width, -w, w), newY = map(y, 0, height, -h, h);

                    //printf("newX=%lf; newY=%lf\n",newX,newY);

                    //Check if Depth of field is enabled. If it is find a new random camera location and set newPos to that location.
                    if (DoF) {
                        //find new random camera position
                        double t = 2 * PI * (rand() / (double)RAND_MAX) * camera.aperature;
                        double u = (rand() / (double)RAND_MAX)*camera.aperature + (rand() / (double)RAND_MAX) * camera.aperature;
                        double r = u > camera.aperature ? 2 * camera.aperature - u : u;
                        newPos = Vector3(camera.position.x + (r*cos(t)), camera.position.y + (r*sin(t)), camera.position.z);
                    }
                    //Create the new ray
                    Ray ray = lens.concaveRefract(Ray(Vector3(newX, newY, z), (newPos - Vector3(newX, newY, z)).normalize()));

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
                        double dif = std::max(0.0, norm.dot((light - pos).normalize()));

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
    Sphere* checkSphereIntersect(Ray ray, std::vector<Sphere> &spheres) {
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
    Quad* checkQuadIntersect(Ray ray, std::vector<Quad> quads) {
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

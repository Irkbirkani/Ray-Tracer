#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <algorithm>
#include <fstream>
#include <vector>
#include <iostream>

#include "Quad.h"
#include "Lens.h"
#include "Camera.h"

using std::vector;
using std::string;
using std::ofstream;

class RayTracer {
public:
    Camera camera;
    int width, height;

    RayTracer();// { camera = Camera(); width = 500; height = 500; }
    RayTracer(Camera cam, double w, double h);// { camera = cam; width = w; height = h; }

    /*
     * Render an image of the scene. Uses cameras[0] for the default camera.
     *     Arguments:
     *         z:         The depth of the image plane. Usually negative width.
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
    void trace(double z, double offset, vector<Sphere> spheres, vector<Quad> quads, Lens lens, 
                Vector3 light, bool stereo, bool DoF, int samples, string file, int traceType);

    // Find the closest sphere with an intersection with the ray.
    Sphere* checkSphereIntersect(Ray ray, vector<Sphere> &spheres);

    // Find the closest quad with an intersection with the ray.
    Quad* checkQuadIntersect(Ray ray, vector<Quad> quads); 
};
#endif

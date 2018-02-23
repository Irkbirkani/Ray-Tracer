#include "Raytracer.h"

using std::vector;
int main() {
    int width = 500, height = 500;

    Image textures[3] = { Image("Images/blue-pentagons.jpg"),
                          Image("Images/sphereTex/jupiter.jpg"),
                          Image("Images/eyechart.png") };

    Plane cavePlane(Vector3(0,0,-5),  WHITE, Vector3(0,0,1));
    Plane  vexPlane(Vector3(0,0,-10), WHITE, Vector3(0,0,1));
    Lens concaveLens(Sphere(Vector3(0,0,-45), 20, WHITE, false),
                     Sphere(Vector3(0,0, 45), 20, WHITE, false), cavePlane, 1.0);
    Lens convexLens (Sphere(Vector3(0,0,-30), 20, WHITE, false),
                     Sphere(Vector3(0,0, 10), 20, WHITE, false), vexPlane, 1.0);

    Camera camera = Camera(Vector3(), Vector3(0, 0, 1), Vector3(0, 1, 0), 0.0001);

    RayTracer rt = RayTracer(camera, width, height);

    vector<Sphere> spheres;
    vector<Quad> quads;
    spheres.resize(5);
    
    spheres[0] = Sphere(Vector3(-150, -150, 600), 100.0, WHITE, true, textures[0]);
    spheres[1] = Sphere(Vector3(-100, -100, 900), 100.0, WHITE, true, textures[1]);
    spheres[2] = Sphere(Vector3( 0,    0,   1200), 100.0, WHITE, true, textures[2]);
    spheres[3] = Sphere(Vector3( 100,  100, 1500), 100.0, WHITE, true, textures[0]);
    spheres[4] = Sphere(Vector3( 200,  200, 1800), 100.0, WHITE, true, textures[1]);

    Vector3 light = Vector3(0, 0, -100);

    //rt.trace(-width, spheres, quads, convexLens,  light, false, true, 100, "biconvexTest.ppm",  BICONVEX);
    rt.trace(-width, spheres, quads, convexLens, light, true, true, 100, "biconvexTestStereo.ppm",  BICONVEX);

    return 0;

}

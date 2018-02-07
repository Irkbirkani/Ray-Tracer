#include "Raytracer.h"

using std::vector;
int main() {
    int width = 500, height = 500;

    Image textures[3] = { Image("Images/blue-pentagons.jpg"),
                          Image("Images/sphereTex/jupiter.jpg"),
                          Image("Images/eyechart.png") };

    Lens concaveLens(Sphere(Vector3(0,0,-45), 20, WHITE, false),
                     Sphere(Vector3(0,0, 45), 20, WHITE, false), 1.0);
    Lens convexLens (Sphere(Vector3(0,0,-30), 20, WHITE, false),
                     Sphere(Vector3(0,0, 10), 20, WHITE, false), 1.0);

    Camera left  = Camera(Vector3(), Vector3(0, 0, 1), Vector3(0, 1, 0), 0.0001);
    Camera right = Camera(Vector3(0, 0, -500), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
    Camera cameras[2] = { left,right };

    RayTracer rt = RayTracer(cameras, width, height);

    vector<Sphere> spheres;
    vector<Quad> quads;
    spheres.resize(5);
    spheres[0] = Sphere(Vector3(-40, -40, 200), 30.0, WHITE, true, textures[0]);
    spheres[1] = Sphere(Vector3(-20, -20,  250), 30.0, WHITE, true, textures[1]);
    spheres[2] = Sphere(Vector3( 0,   0,  300), 30.0, WHITE, true, textures[2]);
    spheres[3] = Sphere(Vector3( 20,  20, 350), 30.0, WHITE, true, textures[0]);
    spheres[4] = Sphere(Vector3( 40,  40, 400), 30.0, WHITE, true, textures[1]);

    Vector3 light = Vector3(0, 0, -100);

    rt.trace(-100.0, spheres, quads, concaveLens, light, true, 100, "concaveTest.ppm", CONCAVE);
    rt.trace(-100.0, spheres, quads, convexLens,  light, true, 100, "convexTest.ppm", CONVEX);

    return 0;

}

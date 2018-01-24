#include "Raytracer.h"

using std::vector;
int main() {
    int width = 150, height = 200;
    
    Image textures[3] = { Image("Images/blue-pentagons.jpg"), 
                          Image("Images/sphereTex/jupiter.jpg"),
                          Image("Images/eyechart.png") };

    Lens lens(Sphere(Vector3(0,0,-45), 15, WHITE, false),
              Sphere(Vector3(0,0, 45),  15, WHITE, false), 1.0);

    Camera left  = Camera(Vector3(), Vector3(0, 0, 1), Vector3(0, 1, 0), 0.0001);
    Camera right = Camera(Vector3(0, 0, -500), Vector3(0, 0, 1), Vector3(0, 1, 0), 10);
    Camera cameras[2] = { left,right };

    RayTracer rt = RayTracer(cameras, width, height);
    
    vector<Sphere> spheres;
    vector<Quad> quads;
    spheres.resize(5);
    spheres[0] = Sphere(Vector3(-200, -200, 300), 125.0, WHITE, true, textures[0]);
    spheres[1] = Sphere(Vector3(-67,  -67,  400), 125.0, WHITE, true, textures[1]);
    spheres[2] = Sphere(Vector3( 67,   67,  500), 125.0, WHITE, true, textures[2]);
    spheres[3] = Sphere(Vector3( 200,  200, 600), 125.0, WHITE, true, textures[0]);
    spheres[4] = Sphere(Vector3( 333,  333, 700), 125.0, WHITE, true, textures[1]);

    Vector3 light = Vector3(0, 0, -100);

    rt.concaveLensTrace(-width*10.0, 60.0 / 180 * PI, 55.0 / 180 * PI,
                        spheres, quads, lens, light, true, 100, "concavetest.ppm");

    return 0;

}

#include "Raytracer.h"
#include <sstream>
#include <string.h>

using std::vector;
using std::stringstream;
using std::string;

int main() {
    int width = 672*2, height = 1600;

    string img1 = "Images/blue-pentagons.jpg";
    string img2 = "Images/sphereTex/jupiter.jpg";
    string img3 = "Images/eyechart.png";
    Image textures[3] = { Image(img1),
                          Image(img2),
                          Image(img3) };

    Plane cavePlane(Vector3(0,0,0), WHITE, Vector3(0,0,1));
    Plane  vexPlane(Vector3(0,0, 0), WHITE, Vector3(0,0,1));
    Lens concaveLens(Sphere(Vector3(0,0,-30), 20, WHITE, false, Image("")),
                     Sphere(Vector3(0,0, 30), 20, WHITE, false, Image("")), cavePlane, 1.0);
    Lens convexLens (Sphere(Vector3(0,0,-10), 20, WHITE, false, Image("")),
                     Sphere(Vector3(0,0, 10), 20, WHITE, false, Image("")), vexPlane, 1.2);

    Camera camera = Camera(Vector3(), Vector3(0, 0, 1), Vector3(0, 1, 0), 0.0001);

    RayTracer rt = RayTracer(camera, width, height);

    vector<Sphere> spheres;
    vector<Quad> quads;
    spheres.resize(5);

    double offset = width/20.0;

    spheres[0] = Sphere(Vector3(-150, -150, 600),  100.0, WHITE, true, textures[0]);
    spheres[1] = Sphere(Vector3(-100, -100, 900),  100.0, WHITE, true, textures[1]);
    spheres[2] = Sphere(Vector3( 0,    0,   1200), 100.0, WHITE, true, textures[2]);
    spheres[3] = Sphere(Vector3( 100,  100, 1500), 100.0, WHITE, true, textures[0]);
    spheres[4] = Sphere(Vector3( 200,  200, 1800), 100.0, WHITE, true, textures[1]);

    Vector3 light = Vector3(0, 0, -100);

    float F;
    Lens lens = F > 0 ? convexLens : concaveLens;
    printf("Enter perscription: ");
    scanf("%f", &F);
    float d = toMM(offset,std::abs(lens.lens[0].radius - lens.lens[0].center.z))/1000.0;
    printf("distance to lens: %f\n",d);
    float Fc = F / (1 - d * F);
    printf("new perscription: %f\n", Fc);
    double ri = ((toMM(offset, lens.lens[0].radius)/1000) * Fc)+1;
    printf("new refactive index: %f\n", ri);
    lens.refracIdx = ri;

    if (F > 0) {
        rt.trace(-width, offset, spheres, quads, lens, light,
             true, false, 1, "plConvexImage.ppm", PCONVEX);
    } else {
        rt.trace(-width, offset, spheres, quads, lens, light,
             true, false, 1, "plConcaveImage.ppm", PCONCAVE);
    }

    return 0;
}

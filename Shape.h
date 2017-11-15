#pragma once
#include "Util.h"

class Sphere {
public:
    Vector3 center, color;
    Image image;
    double radius;
    bool texMap;
    Sphere(Vector3 c = Vector3(), double r = 1000, Vector3 col = WHITE, bool tm = false, Image img = NULL) { center = c; radius = r; color = col; texMap = tm; image = img; }

    //Returns true if a ray intersects with the sphere. The closest point of intersection is set as t.
    bool intersect(Ray ray, double &t)
    {
        //Calculate the a b and c values for the quadratic equation.
        double a =  ray.direction.x * ray.direction.x +
                   ray.direction.y * ray.direction.y +
                   ray.direction.z * ray.direction.z;
        double b = (ray.direction.x * (ray.origin.x - center.x) +
                   ray.direction.y * (ray.origin.y - center.y) +
                   ray.direction.z * (ray.origin.z - center.z)) * 2;
        double c = (ray.origin.x - center.x) * (ray.origin.x - center.x) +
                  (ray.origin.y - center.y) * (ray.origin.y - center.y) +
                  (ray.origin.z - center.z) * (ray.origin.z - center.z) -
                  (radius * radius);
        //Calculate the discriminate of the quadratic equation.
        double disc = (b * b) - (4 * a * c);

        //If the discriminate is less than 0 then there is no intersection and the funtion terminates.
        if (disc < 0)
            return false;

        //Calculate the two points of intersection and set t to be the smaller.
        double t0 = (-b - sqrt(disc)) / (2 * a);
        double t1 = (-b + sqrt(disc)) / (2 * a);

        t = (t0 < t1 && std::min(t0,t1) > 0.001) ? t0 : t1;
        return true;
    }

    //Returns true if a ray intersects with the sphere. The farthest point of intersection is set as t.
    bool intersectFar(Ray ray, double &t)
    {
        //Calculate the a b and c values for the quadratic equation.
        double a = ray.direction.x * ray.direction.x +
            ray.direction.y * ray.direction.y +
            ray.direction.z * ray.direction.z;
        double b = (ray.direction.x * (ray.origin.x - center.x) +
            ray.direction.y * (ray.origin.y - center.y) +
            ray.direction.z * (ray.origin.z - center.z)) * 2;
        double c = (ray.origin.x - center.x) * (ray.origin.x - center.x) +
            (ray.origin.y - center.y) * (ray.origin.y - center.y) +
            (ray.origin.z - center.z) * (ray.origin.z - center.z) -
            (radius * radius);
        //Calculate the discriminate of the quadratic equation.
        double disc = (b * b) - (4 * a * c);

        //If the discriminate is less than 0 then there is no intersection and the funtion terminates.
        if (disc < 0)
            return false;

        //Calculate the two points of intersection and set t to be the smaller.
        double t0 = (-b - sqrt(disc)) / (2 * a);
        double t1 = (-b + sqrt(disc)) / (2 * a);

        t = (t0 > t1 && std::max(t0, t1) > 0.001) ? t0 : t1;
        return true;
    }

    //Returns the barycentric coordinates of a point on the sphere.
    void getSphereUV(Vector3 normal, double &u, double &v)
    {
        u = atan2(normal.x, normal.z) / (2 * PI) + 0.5f;
        v = normal.y * 0.5f + 0.5f;
    }

    //Returns the color of the texture for a specific point on a Sphere.
    Vector3 getTex(Vector3 normal) {

        //Find the barycentric coordinates.
        double u, v;
        getSphereUV(normal, u, v);
        //Map the barycentric coordinates to points on the texture.
        int s = map(u, 0, 1, 0, image.width), t = map(v, 0, 1, 0, image.height);

        //Finds the rgb values of the given point and retuns them as a vector.
        unsigned char r = image.image[3 * (t*image.width + s)], g = image.image[3 * (t*image.width + s) + 1], b = image.image[3 * (t*image.width + s) + 2];
        return Vector3((int)r, (int)g, (int)b);

    }
};

class Plane
{
public:
    Vector3 center;
    Vector3 color;
    Vector3 N;
    Plane(Vector3 c = Vector3(0, 0, 0), Vector3 _N = Vector3(1, 1, 1), Vector3 col = Vector3(255, 255, 255)) { center = c; N = _N; color = col; }

    //Returns true if a ray intersects with a plane.
    bool intersect(Ray ray, Vector3 &p)
    {
        Vector3 dir = ray.direction - ray.origin;
        double d = ((center - ray.origin).dot(N)) / (dir.dot(N));

        if (d <= 0)
            return false;

        p = dir * d + ray.origin;
        return true;
    }
};

class Quad
{
public:
    Vector3 A, B, C, D, normal, color;
    Image image;
    bool texMap;
    Quad(Vector3 a = Vector3(0, 0, 100), Vector3 b = Vector3(500, 0, 100), Vector3 c = Vector3(500, 500, 100), Vector3 d = Vector3(0, 500, 100),
        Vector3 n = Vector3(0, 0, -1), Vector3 col = WHITE, bool tm = false, Image img = NULL) {
        A = a; B = b; C = c; D = d; normal = n; color = col; texMap = tm;image = img;
    }

    //Returns true if a Ray intersects with the Quad.
    bool intersect(Ray ray, double &t)
    {
        //Finds the point of intersection between the ray and the plane the quad is on.
        Vector3 AB = B - A, AD = D - A;
        double d = ((A - ray.origin).dot(normal)) / (ray.direction.dot(normal));
        Vector3 P = ray.direction * d + ray.origin;

        //Finds the barycentric coordinates of the point of intesection.
        //If the point is greater than one or less than 0 then there is no intersection between the point and the plane.
        u = (-A.x * AD.y + A.y * AD.x - AD.x * P.y + AD.y * P.x) / (AB.x * AD.y - AB.y * AD.x);
        v = (-A.x * AB.y + A.y * AB.x - AB.x * P.y + AB.y * P.x) / (AB.y * AD.x - AB.x * AD.y);
        if ((0<u && u<1) && (0<v && v<1))
        {
            t = d;
            return true;
        }
        return false;
    }

    //Returns the color of a texture for a specific point on a Quad.
    Vector3 getTex(Vector3 pos) {
        //Map the barycentric coordinates to points between the image height and width.
        int s = map(u, 0, 1, 0, image.width), t = map(v, 0, 1, 0, image.height);

        //Finds the rgb values of the given point and retuns them as a vector.
        unsigned char r = image.image[3 * (t*image.width + s)], g = image.image[3 * (t*image.width + s) + 1], b = image.image[3 * (t*image.width + s) + 2];
        return Vector3((int)r, (int)g, (int)b);

    }

private:
    double u, v;
};

class Lens {
public:
    Sphere lens[2];
    double refracIdx;
    Lens() { lens[0] = Sphere(), lens[1] = Sphere(),  refracIdx = 1.0; }
    Lens(Sphere frontLens, Sphere backLens, double refractionIndex) { lens[0] =  frontLens, lens[1] = backLens, refracIdx = refractionIndex; }

    //Refracts through a sphere. Uses lens[0] for intersections.
    Ray sphereRefract(Ray ray) {

        //Find point of intersection on lens and find the normal at that point.
        double p;
        lens[0].intersect(ray, p);
        Vector3 pos = ray.origin + ray.direction * p;
        Vector3 norm = (pos - lens[0].center).normalize();

        //Find the angle between the incoming ray and the normal.
        double theta1 = (-ray.direction).getAngleBetween(norm);

        //Find the angle between the refracted ray and the normal.
        double theta2 = asin(1.0*sin(theta1) / refracIdx);

        //Find the new direction.
        Vector3 rotAxis = (ray.direction.cross(norm)).normalize();
        Vector3 newDir = rotAroundAxis(rotAxis, -norm, theta2);

        //Find new origin of the ray coming out of the lens.
        lens[0].intersect(Ray(pos, newDir), p);

        //Find the normal at the new position.
        pos = pos + newDir * p;
        norm = (pos - lens[0].center).normalize();

        //Find angle between newDir and new normal.
        theta1 = (-newDir).getAngleBetween(-norm);

        //Find the angle between the refracted ray and the normal.
        theta2 = asin(refracIdx*sin(theta1) / 1.0);

        //Find the new direction.
         rotAxis = (newDir.cross(norm)).normalize();
         newDir = rotAroundAxis(rotAxis, norm, theta2);

         Vector3 test = ray.direction.normalize();

        //Return the new ray.
        return Ray(pos, newDir);
    }

    Ray concaveRefract(Ray ray) {
        
        double p;
        lens[0].intersectFar(ray, p);
        Vector3 pos = ray.origin + ray.direction * p;
        Vector3 norm = (lens[0].center - pos).normalize();

        //Find the angle between the incoming ray and the normal.
        double theta1 = (-ray.direction).getAngleBetween(norm);

        //Find the angle between the refracted ray and the normal.
        double theta2 = asin(1.0*sin(theta1) / refracIdx);

        //Find the new direction.
        Vector3 rotAxis = (ray.direction.cross(norm)).normalize();
        Vector3 newDir = rotAroundAxis(rotAxis, -norm, theta2);

        //Find new origin of the ray coming out of the lens.
        lens[1].intersect(Ray(pos, newDir), p);

        //Find the normal at the new position.
        pos = pos + newDir * p;
        norm = (lens[1].center - pos).normalize();

        //Find angle between newDir and new normal.
        theta1 = (-newDir).getAngleBetween(-norm);

        //Find the angle between the refracted ray and the normal.
        theta2 = asin(refracIdx*sin(theta1) / 1.0);

        //Find the new direction.
        rotAxis = (newDir.cross(norm)).normalize();
        newDir = rotAroundAxis(rotAxis, norm, theta2);

        Vector3 test = ray.direction.normalize();

        //Return the new ray.
        return Ray(pos, newDir);

    }

    Ray convexRefract(Ray ray) {
        //Find point of intersection on lens and find the normal at that point.
        double p;
        lens[1].intersect(ray, p);
        Vector3 pos = ray.origin + ray.direction * p;
        Vector3 norm = (pos - lens[1].center).normalize();
        printf("Norm = ");
        norm.println();
        printf("Point of intersection on first sphere: ");
        pos.println();

        //Find the angle between the incoming ray and the normal.
        double theta1 = (-ray.direction).getAngleBetween(norm);
        printf("theta1 = %lf (in rad)\n", theta1);

        //Find the angle between the refracted ray and the normal.
        double theta2 = asin(1.0*sin(theta1) / refracIdx);
        printf("theta2 = %lf (in rad. is the angle from snell's law)\n", theta2);

        (ray.direction.cross(norm)).println();
        //Find the new direction.
        Vector3 rotAxis = (ray.direction.cross(norm)).normalize();
        printf("rotation axis to find newDir ");
        rotAxis.println();
        Vector3 newDir = rotAroundAxis(rotAxis, -norm, theta2);
        printf("newDir = ");
        newDir.println();

        //Find new origin of the ray coming out of the lens.
        lens[0].intersect(Ray(pos, newDir), p);

        //Find the normal at the new position.
        pos = pos + newDir * p;
        norm = (pos - lens[0].center).normalize();
        printf("Position on back of sphere: ");
        pos.println();

        //Find angle between newDir and new normal.
        theta1 = (-newDir).getAngleBetween(-norm);
        printf("New theta1 = %lf (in rad)\n", theta1);

        //Find the angle between the refracted ray and the normal.
        theta2 = asin(refracIdx*sin(theta1) / 1.0);
        printf("new theta2 = %lf (in rad. is the angle from snell's law)\n", theta2);

        //Find the new direction.
        rotAxis = (newDir.cross(norm)).normalize();
        printf("rotation axis to find newDir ");
        rotAxis.println();
        newDir = rotAroundAxis(rotAxis, norm, theta2);
        printf("newDir = ");
        newDir.println();

        printf("New ray with origin: ");
        pos.print();
        printf(" and direction: ");
        newDir.print();
        printf("\n\n");

        //Return the new ray.
        return Ray(pos, newDir);
    }
};

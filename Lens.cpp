#include "Lens.h"

Lens::Lens(Sphere frontLens, Sphere backLens, Plane pl, double refractionIndex)
{
     lens[0] =  frontLens;
     lens[1] = backLens;
     plane = pl;
     refracIdx = refractionIndex; 
}
void Lens::changePos(double offset) {
    lens[0].center.x = lens[0].center.x + offset;
    lens[1].center.x = lens[1].center.x + offset;
    plane.center.x = plane.center.x + offset;
} 

Ray Lens::refract(Ray ray, int refracType) {

    //Find point of intersection on lens and find the normal at that point.
    double p;
    Vector3 pos, norm;
    switch(refracType) {
        case BICONCAVE: lens[0].intersectFar(ray, p);
                        pos = ray.origin + ray.direction * p;
                        norm = (lens[0].center - pos).normalize();
                        break;
        case PCONCAVE:  lens[0].intersectFar(ray, p);
                        pos = ray.origin + ray.direction * p;
                        norm = (lens[0].center - pos).normalize();
                        break;
        case BICONVEX:  lens[1].intersect(ray, p);
                        pos = ray.origin + ray.direction * p;
                        norm = (pos - lens[1].center).normalize();
                        break;
        case PCONVEX:   lens[1].intersect(ray, p);
                        pos = ray.origin + ray.direction * p;
                        norm = (pos - lens[1].center).normalize();
                        break;
    }
    
    //Find the angle between the incoming ray and the normal.
    double theta1 = (-ray.direction).getAngleBetween(norm);

    //Find the angle between the refracted ray and the normal.
    double theta2 = asin(1.0*sin(theta1) / refracIdx);

    Vector3 newDir;

    if(std::abs(theta1) < 0.0001 || std::abs(theta2) < 0.0001) {
        newDir = ray.direction;
    } else {
        //Find the new direction.
        Vector3 rotAxis = (ray.direction.cross(norm)).normalize();
        newDir = rotAroundAxis(rotAxis, -norm, rad_to_deg(theta2));
    }

    //Find new origin of the ray coming out of the lens and the normal at the new position.
    switch(refracType) {
        case BICONCAVE: lens[1].intersect(Ray(pos, newDir), p);
                        pos = pos + newDir * p;
                        norm = (lens[1].center - pos).normalize();
                        break;
        case PCONCAVE:  plane.intersect(Ray(pos, newDir), pos);
                        norm = plane.N;
                        break;
        case BICONVEX:  lens[0].intersect(Ray(pos, newDir), p);
                        pos = pos + newDir * p;
                        norm = (pos - lens[0].center).normalize();
                        break;
        case PCONVEX:   plane.intersect(Ray(pos, newDir), pos);
                        norm = plane.N;
                        break;
    }

    //Find angle between newDir and new normal.
    theta1 = (-newDir).getAngleBetween(-norm);

    //Find the angle between the refracted ray and the normal.
    theta2 = asin(refracIdx*sin(theta1) / 1.0);

    if(std::abs(theta1) < 0.0001 || std::abs(theta2) < 0.0001) {
        //leave newDir alone
    } else {
        //Find the new direction.
        Vector3 rotAxis = (newDir.cross(norm)).normalize();
        newDir = rotAroundAxis(rotAxis, norm, -rad_to_deg(theta2));
    }

    //Return the new ray.
    return Ray(pos, newDir);
}

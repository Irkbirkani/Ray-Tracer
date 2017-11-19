/* Tester file for refraction functions */
#include <stdlib.h>

#include "Shape.h"

int main(void) {

    printf("--------- Testing .normalize() ---------\n");

    //Set up test Vectors3s.
    Vector3 normTest1 = Vector3(2, 4, 4);
    Vector3 normTest2 = Vector3(1, 0, 0);
    Vector3 normTest3 = Vector3(6, 6, 3);

    //What the vectors should normalize to.
    Vector3 normTest1Ans = Vector3(1/3.0, 2/3.0, 2/3.0);
    Vector3 normTest2Ans = Vector3(1,    0,    0);
    Vector3 normTest3Ans = Vector3(2/3.0, 2/3.0, 1/3.0);


    if(normTest1.normalize() == normTest1Ans){
        printf("***SUCCESS***\n");
        printf("normTest1    = Vector(%lf, %lf, %lf)\nnormTest1Ans = Vector(%lf, %lf, %lf)\n\n",
                normTest1.x, normTest1.y, normTest1.z,
                normTest1Ans.x, normTest1Ans.y, normTest1Ans.z);
    } else {
        printf("***FAILED***\nnormalize on Vector(%lf, %lf, %lf) failed.\n\n",
		       	normTest1.x, normTest1.y, normTest1.z);
    }

    if(normTest2.normalize() == normTest2Ans){
        printf("***SUCCESS***\n");
        printf("normTest2    = Vector(%lf, %lf, %lf)\nnormTest2Ans = Vector(%lf, %lf, %lf)\n\n",
                normTest2.x, normTest2.y, normTest2.z,
                normTest2Ans.x, normTest2Ans.y, normTest2Ans.z);
    } else {
        printf("***FAILED***\nnormalize on Vector(%lf, %lf, %lf) failed.\n\n",
		       	normTest2.x, normTest2.y, normTest2.z);
    }


    if(normTest3.normalize() == normTest3Ans){
        printf("***SUCCESS***\n");
        printf("normTest3    = Vector(%lf, %lf, %lf)\nnormTest3Ans = Vector(%lf, %lf, %lf)\n\n",
                normTest3.x, normTest3.y, normTest3.z,
                normTest3Ans.x, normTest3Ans.y, normTest3Ans.z);
    } else {
        printf("***FAILED***\nnormalize on Vector(%lf, %lf, %lf) failed.\n\n",
		       	normTest3.x, normTest3.y, normTest3.z);
    }

    printf("--------- Testing Cross Product ---------\n");

    //Set up test Vector3s.
    Vector3 xy = Vector3(1,1,0);
    Vector3 xz = Vector3(1,0,1);
    Vector3 yz = Vector3(0,1,1);

    //Set up result Vector3s.
    Vector3 xyCrossxz = xy.cross(xz);
    Vector3 xzCrossyz = xz.cross(yz);
    Vector3 yzCrossxy = yz.cross(xy);

    //Set up answer Vector3s.
    Vector3 xyCrossxzAns = Vector3( 1,-1,-1);
    Vector3 yzCrossxyAns = Vector3(-1, 1,-1);
    Vector3 xzCrossyzAns = Vector3(-1,-1, 1);

    if(xyCrossxz == xyCrossxzAns){
        printf("***SUCCESS***\n");
        printf("xy.cross(xz) = Vector(%lf,%lf,%lf)\n\n", xyCrossxz.x, xyCrossxz.y, xyCrossxz.z);
    } else {
        printf("***FAILED***\n");
        printf("Cross product failed over Vector(%lf, %lf, %lf) x Vector(%lf, %lf, %lf)\n\n",
                xy.x, xy.y, xy.z, xz.x, xz.y, xz.x);
    }


    if(yzCrossxy == yzCrossxyAns){
        printf("***SUCCESS***\n");
        printf("yz.cross(xy) = Vector(%lf,%lf,%lf)\n\n", yzCrossxy.x, yzCrossxy.y, yzCrossxy.z);
    } else {
        printf("***FAILED***\n");
        printf("Cross product failed over Vector(%lf, %lf, %lf) x Vector(%lf, %lf, %lf)\n\n",
                yz.x, yz.y, yz.z, xy.x, xy.y, xy.x);
    }

    if(xzCrossyz == xzCrossyzAns){
        printf("***SUCCESS***\n");
        printf("xz.cross(yz) = Vector(%lf,%lf,%lf)\n\n", xzCrossyz.x, xzCrossyz.y, xzCrossyz.z);
    } else {
        printf("***FAILED***\n");
        printf("Cross product failed over Vector(%lf, %lf, %lf) x Vector(%lf, %lf, %lf)\n\n",
                xz.x, xz.y, xz.z, yz.x, yz.y, yz.x);
    }

    printf("--------- Testing rotAroundAxis() ---------\n");

    //What the vectors should rotate to.
    Vector3 xyRotated = Vector3(-1,  1,  0);
    Vector3 yzRotated = Vector3( 0, -1,  1);
    Vector3 xzRotated = Vector3( 1,  0, -1);

    //Rotation angle.
    double theta = 90;

    //Test rotAroundAxis where axis = X_AXIS, vector = yz, theta = 90.
    printf("Test rotAroundAxis where axis = X_AXIS, vector = yz, theta = 90\n");

    Vector3 yzResult = rotAroundAxis(X_AXIS, yz, theta);

    if(yzResult == yzRotated){
        printf("***SUCCESS***\n");
    } else {
        printf("***FAILED***\n rotAroundAxis failed with inputs: X_AXIS, Vector3(0, 1, 1), 90\n");
        printf("Is ");
        yzResult.print();
        printf("\nShould have been Vector(%lf, %lf, %lf)\n\n",
		       	yzRotated.x, yzRotated.y, yzRotated.z);
    }

    //Test rotAroundAxis where axis = Y_AXIS, vector = xz, theta = 90.
    printf("Test rotAroundAxis where axis = Y_AXIS, vector = xz, theta = 90\n");

    Vector3 xzResult = rotAroundAxis(Y_AXIS, xz, theta);

    if(xzResult == xzRotated){
        printf("***SUCCESS***\n");
    } else {
        printf("***FAILED***\n rotAroundAxis failed with inputs: Y_AXIS, Vector3(1, 0, 1), 90)\n");
        printf("Is ");
        xzResult.print();
        printf("\nShould have been Vector(%lf, %lf, %lf)\n\n",
                 xzRotated.x, xzRotated.y, xzRotated.z);
    }

    //Test rotAroundAxis where axis = Z_AXIS, vector = xy, theta = 90.
    printf("Test rotAroundAxis where axis = xy, vector = Z_AXIS, theta = 90\n");

    Vector3 zResult = rotAroundAxis(xy, Z_AXIS, theta);
    Vector3 zRotated = Vector3(0.707107, -0.707107, 0);

    if(zResult == zRotated){
        printf("***SUCCESS***\n");
    } else {
        printf("***FAILED***\n rotAroundAxis failed with inputs: Vector3(1, 1, 0), Z_AXIS, 90\n");
        printf("Is ");
        zResult.print();
        printf("\nShould have been Vector(%lf, %lf, %lf)\n\n",
                 zRotated.x, zRotated.y, zRotated.z);
    }

    printf("Test rotAroundAxis where axis = Vector(0.75, 1, 0.5), vector = Vector(0.5, 0.25, 1), theta = 45\n");

	theta = 45;
    Vector3 testAxis    = Vector3(0.75, 1, 0.5);
    Vector3 testVector  = Vector3(0.5, 0.25, 1);
    Vector3 testResult  = rotAroundAxis(testAxis, testVector, theta);
    Vector3 testRotated = Vector3(0.949473, 0.09596, 0.633872);

    if(testResult == testRotated){
        printf("***SUCCESS***\n");
    } else {
        printf("***FAILED***\n rotAroundAxis failed with inputs: Vector3(1, 1, 0), Z_AXIS, 45\n");
        printf("Is ");
        testResult.println();
        printf("Should have been ");
		testRotated.println();
    }

    printf("Test rotAroundAxis where axis = Vector(1278,328,153), vector = Vector(321,452,7632), theta = 21\n");

	theta = 21;
    testAxis    = Vector3(1278,328,153);
    testVector  = Vector3(321,452,7632);
    testResult  = rotAroundAxis(testAxis, testVector, theta);
    testRotated = Vector3(1039.467826,-2175.025918,7262.47464);

    if(testResult == testRotated){
        printf("***SUCCESS***\n");
    } else {
        printf("***FAILED***\n rotAroundAxis failed with inputs: Vector3(1, 1, 0), Z_AXIS, 45\n");
        printf("Is ");
        testResult.println();
        printf("Should have been ");
		testRotated.println();
    }
/*
    printf("--------- Testing convexRefract() ---------\n");

    //Set up Lens.
    Lens testLens(Sphere(Vector3(), 10.0, WHITE, false),
                  Sphere(Vector3(), 10.0, WHITE, false),
                  1.0);
    printf("Lens testLens with the first sphere at ");
    testLens.lens[0].center.print();
    printf(" and radius %lf.\nThe second sphere is at ", testLens.lens[0].radius);
    testLens.lens[1].center.print();
    printf(" and radius %lf.\nThe index of refraction is %lf\n\n",
		    testLens.lens[1].radius, testLens.refracIdx);

    Ray ray(Vector3(0,0,-100), Vector3(0,0,1));

    printf("Testing convexRefract on ray with origin: ");
    ray.origin.print();
    printf(" and direction: ");
    ray.direction.print();
    printf("\n\n");
    Ray retRay = testLens.convexRefract(ray);

    if(ray.direction == retRay.direction) {
        printf("***SUCCESS***\n");
    } else {
        printf("***FAILED***\n");
        printf("new ray direction: ");
        retRay.direction.println();
        printf("Should have been: ");
        ray.direction.println();
    }

    ray = Ray(Vector3(0,0,-20), Vector3(0,5,20).normalize());

    printf("Testing convexRefract on ray with origin: ");
    ray.origin.print();
    printf(" and direction: ");
    ray.direction.print();
    printf("\n\n");
    retRay = testLens.convexRefract(ray);

    if(ray.direction == retRay.direction) {
        printf("***SUCCESS***\n");
    } else {
        printf("***FAILED***\n");
        printf("new ray direction: ");
        retRay.direction.println();
        printf("Should have been: ");
        ray.direction.println();
    }

    ray = Ray(Vector3(0,10,-10), (Vector3(0,0.1,0) - Vector3(0,10,-10)).normalize());

    printf("Testing convexRefract on ray with origin: ");
    ray.origin.print();
    printf(" and direction: ");
    ray.direction.print();
    printf("\n\n");
    retRay = testLens.convexRefract(ray);

    if(ray.direction == retRay.direction) {
        printf("***SUCCESS***\n");
    } else {
        printf("***FAILED***\n");
        printf("new ray direction: ");
        retRay.direction.println();
        printf("Should have been: ");
        ray.direction.println();
    }
*/
}

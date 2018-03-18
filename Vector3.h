#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3
{
public:
    double x, y, z;
    Vector3();
    Vector3(double v);
    Vector3(double _x, double _y, double _z);

    //Returns the dot product of two vectors.
    double dot(Vector3 v);

    //Returns the sum of two vectors.
    Vector3 operator + (const Vector3& v) const;    

    //Returns the difference between two vectors.
    Vector3 operator - (const Vector3& v) const;      

    //Returns the negative of a vector.
    Vector3 operator - () const;
    
    //Return the product of a vector and a scalar.
    Vector3 operator * (double d) const;

    //Returns the quotient of a vector and a scalar.
    Vector3 operator / (double d) const;
    
    //Returns true if the x, y, and z coordinates are equal
    bool operator == (const Vector3& v) const;

    //Returns true if one of the x, y, or z coordinates does not equal the others.
    bool operator != (const Vector3& v) const;

    //Returns the magnitude of a vector.
    double magnitude();

    //Normalize the vector.
    Vector3 normalize(); 

    //Returns the distance between two vectors.
    double distance(Vector3 v);

    //Returns the cross product of two vectors.
    Vector3 cross(Vector3 v);

    //Rotates the Vector around the X axis by a given amount.
    Vector3 rotAroundX(double theta);

    //Rotates the Vector around the Y axis by a given amount.
    Vector3 rotAroundY(double theta);

    //Rotates the Vector around the Z axis by a given amount.
    Vector3 rotAroundZ(double theta);

    //Retuns the angle between two vectors.
    double getAngleBetween(Vector3 v);

    //Prints the Vector.
    void print(char* str);
    void print();

    //Prints the Vector with a new line.
    void println(char* str);
    void println();    
};
#endif

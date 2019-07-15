#ifndef GEOMETRY_H_GUARD
#define GEOMETRY_H_GUARD

//Struct for vectors of 3 floats

struct Vector3f{
  float x;
  float y;
  float z;
  Vector3f():x(0),y(0),z(0){}
  Vector3f(float x, float y, float z):x(x),y(y),z(z){}

  Vector3f operator+(const Vector3f other){
    return Vector3f(x+other.x, y+other.y, z+other.z);  
  }
  Vector3f operator+=(const Vector3f other){
    x+=other.x;
    y+=other.y;
    z+=other.z;  
  }
  Vector3f operator*(const float scalar){
    return Vector3f(x*scalar,y*scalar,z*scalar);  
  }
};


void printVector(const Vector3f& v);

float getR(const Vector3f& theta);
float getX(Vector3f theta);
float getY(Vector3f theta);
float getZ(Vector3f theta);
Vector3f getCartesian(Vector3f theta);
  
float getRDot(Vector3f theta, Vector3f thetaDot);
float getZDot(Vector3f theta, Vector3f thetaDot);

Vector3f getThetaDot(const Vector3f& theta, const Vector3f& rDot);

Vector3f vectorToRadians(const Vector3f& v);
Vector3f vectorToDegrees(const Vector3f& v);

#endif


#include <Arduino.h>
#include "geometry.h"

//Geometric lengths for arm

float a = 8.1;
float b = 8.1;
float c = 6.3;
float r_base = 1.8;
float z_base = 3;

//Function for printing a vector

void printVector(const Vector3f& v){
  Serial.print("[");
  Serial.print(v.x);
  Serial.print(", ");
  Serial.print(v.y);
  Serial.print(", ");
  Serial.print(v.z);
  Serial.print("]");
  Serial.println("");
}

//Functions for calculating position

float getR(const Vector3f& theta){
  return r_base + a*sin(theta.y) - b*cos(theta.z) + c;
}
float getX(Vector3f theta){
  return getR(theta)*cos(theta.x);  
}
float getY(Vector3f theta){
  return getR(theta)*sin(theta.x);  
}
float getZ(Vector3f theta){
  return z_base + a*cos(theta.y) - b*sin(theta.z);  
}
Vector3f getCartesian(Vector3f theta){
  return Vector3f(getX(theta), getY(theta), getZ(theta));  
}

//Functions for calculating rDot and zDot, given theta and thetaDot

float getRDot(Vector3f theta, Vector3f thetaDot){
  return a*cos(theta.y)*thetaDot.y + b*sin(theta.z)*thetaDot.z;  
}
float getZDot(Vector3f theta, Vector3f thetaDot){
  return -a*sin(theta.y)*thetaDot.y - b*cos(theta.z)*thetaDot.z;  
}

//Function for calculating the necessary thetaDot for a given rDot

Vector3f getThetaDot(const Vector3f& theta, const Vector3f& rDot){
  float r = getR(theta);
  
  //xDot = A11*thetaDot1 + A12*thetaDot2 + A13*thetaDot3
  float A11 = -sin(theta.x)*r;
  float A12 = a*cos(theta.x)*cos(theta.y);
  float A13 = b*cos(theta.x)*sin(theta.z);
  
  //yDot = A21*thetaDot1 + A22*thetaDot2 + A23*thetaDot3
  float A21 = cos(theta.x)*r;
  float A22 = a*sin(theta.x)*cos(theta.y);
  float A23 = b*sin(theta.x)*sin(theta.z);

  //zDot = A31*thetaDot1 + A32*thetaDot2 + A33*thetaDot3
  float A31 = 0;
  float A32 = -a*sin(theta.y);
  float A33 = -b*cos(theta.z);

  float detA = A11*(A22*A33 - A32*A23) - A12*(A21*A33 - A31*A23) + A13*(A21*A32 - A31*A22);  

  //B = inverse of A, without dividing by the determinant
  //Dividing by determinant later
  float b11 = +(A22*A33 - A32*A23);
  float b12 = -(A12*A33 - A32*A13);
  float b13 = +(A12*A23 - A22*A13);
  float b21 = -(A21*A33 - A31*A23);
  float b22 = +(A11*A33 - A31*A13);
  float b23 = -(A11*A23 - A21*A13);
  float b31 = +(A21*A32 - A31*A22);
  float b32 = -(A11*A32 - A31*A12);
  float b33 = +(A11*A22 - A21*A12);

  //Calculate thetaDot elements
  float thetaDot1 = (b11*rDot.x + b12*rDot.y + b13*rDot.z)/detA;
  float thetaDot2 = (b21*rDot.x + b22*rDot.y + b23*rDot.z)/detA;
  float thetaDot3 = (b31*rDot.x + b32*rDot.y + b33*rDot.z)/detA;

  return Vector3f(thetaDot1, thetaDot2, thetaDot3);
}

//Utility functions for working with vectors of angles

Vector3f vectorToRadians(const Vector3f& v){
  return Vector3f(radians(v.x),radians(v.y),radians(v.z));    
}
Vector3f vectorToDegrees(const Vector3f& v){
  return Vector3f(degrees(v.x),degrees(v.y),degrees(v.z));
}

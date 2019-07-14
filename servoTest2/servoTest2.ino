
#include <Servo.h>

#define TEST 2

struct Vector3f{
  float x;
  float y;
  float z;
  Vector3f():x(0),y(0),z(0){}
  Vector3f(float x, float y, float z):x(x),y(y),z(z){}
  
  void setRadX(float x){ this->x=x; }
  void setRadY(float y){ this->y=y; }
  void setRadZ(float z){ this->z=z; }
  void setDegX(float x){ this->x=radians(x); }
  void setDegY(float y){ this->y=radians(y); }
  void setDegZ(float z){ this->z=radians(z); }

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

Servo servo1;
Servo servo2;
Servo servo3;

Vector3f theta;
Vector3f thetaDot;

float a = 8.1;
float b = 8.1;
float c = 6.3;
float r_base = 1.8;
float z_base = 3;

void waitForInput(){
  while(Serial.available() < 1){}
  while(Serial.available() > 0){
    Serial.read();  
  }
}

Vector3f getThetaVector(float x, float y, float z){
  return Vector3f(radians(x), radians(y), radians(z));  
}
void writeServos(Vector3f theta){
  int angle1 = degrees(theta.x);
  if(angle1<0)angle1=0;
  if(angle1>120)angle1=120;

  int angle2 = degrees(theta.y)+10;
  if(angle2<10)angle2=10;
  if(angle2>90)angle2=90;

  int angle3 = degrees(theta.z)-20;
  if(angle3<50)angle3=50;
  if(angle3>180)angle3=180;
  
  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);  
}
void printVector(Vector3f theta){
  Serial.print("[");
  Serial.print(theta.x);
  Serial.print(", ");
  Serial.print(theta.y);
  Serial.print(", ");
  Serial.print(theta.z);
  Serial.print("]");
  Serial.println("");
}
void printVectorDegrees(Vector3f theta){
  Serial.print("[");
  Serial.print(degrees(theta.x));
  Serial.print(", ");
  Serial.print(degrees(theta.y));
  Serial.print(", ");
  Serial.print(degrees(theta.z));
  Serial.print("]");
  Serial.println("");
}

float getR(Vector3f theta){
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

float getRDot(Vector3f theta, Vector3f thetaDot){
  return a*cos(theta.y)*thetaDot.y + b*sin(theta.z)*thetaDot.z;  
}
float getZDot(Vector3f theta, Vector3f thetaDot){
  return -a*sin(theta.y)*thetaDot.y - b*cos(theta.z)*thetaDot.z;  
}

void printCartesian(Vector3f theta){
  Serial.print("[");
  Serial.print(getX(theta));
  Serial.print(", ");
  Serial.print(getY(theta));
  Serial.print(", ");
  Serial.print(getZ(theta));
  Serial.print("]");
  Serial.println("");
}

Vector3f getThetaDot(Vector3f theta, float xDot, float yDot, float zDot){

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
  float thetaDot1 = (b11*xDot + b12*yDot + b13*zDot)/detA;
  float thetaDot2 = (b21*xDot + b22*yDot + b23*zDot)/detA;
  float thetaDot3 = (b31*xDot + b32*zDot + b33*zDot)/detA;

  return Vector3f(thetaDot1, thetaDot2, thetaDot3);
}

void setup() {
    
  Serial.begin(9600);

  // put your setup code here, to run once:
  servo1.attach(5);
  servo2.attach(6);
  servo3.attach(9);

}

void test1(){
  theta = getThetaVector(87, 60-10, 90+20);
  writeServos(theta);
  printCartesian(theta);
  Serial.println(getR(theta));
  
  waitForInput();
  
  theta+=getThetaVector(-20,0,0);
  writeServos(theta);
  printCartesian(theta);
  Serial.println(getR(theta));
  
  waitForInput();
  
  theta+=getThetaVector(-20,20,0);
  writeServos(theta);
  printCartesian(theta);
  Serial.println(getR(theta));
  
  waitForInput();
  
  theta+=getThetaVector(0,-30,-20);
  writeServos(theta);
  printCartesian(theta);
  Serial.println(getR(theta));
  
  waitForInput();
  
  theta+=getThetaVector(0,50,50);
  writeServos(theta);
  printCartesian(theta);
  Serial.println(getR(theta));

  waitForInput();
}

void test2(){

  theta = getThetaVector(87, 60-10, 90+20);
  writeServos(theta);
  printCartesian(theta);
  
  waitForInput();
  
  float xDot = -4;
  float yDot = 4;
  float zDot = 4;

  thetaDot = getThetaDot(theta, xDot, yDot, zDot);
  
  long prevMillis = millis();
  long elapsedTime = 0;

  while(elapsedTime < 1200){
    long currentMillis = millis();
    long elapsedMillis = currentMillis - prevMillis;
    prevMillis = currentMillis;
    elapsedTime += elapsedMillis;
    float seconds = ((float)elapsedMillis) / 1000.0;

    Vector3f deltaTheta = thetaDot*seconds;
    theta += deltaTheta;
    writeServos(theta);
    printCartesian(theta);

    thetaDot = getThetaDot(theta, xDot, yDot, zDot);
  }

  waitForInput();
}
  
void loop() {

  if(TEST==1){
    test1();  
  }else if(TEST==2){
    test2();  
  }
  
}

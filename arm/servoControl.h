#ifndef SERVO_CONTROL_H_GUARD
#define SERVO_CONTROL_H_GUARD

#include <Servo.h>
#include <Arduino.h>
#include "geometry.h"

class SmartServo{
  
public:
  SmartServo(const int MIN, const int MAX):MIN(MIN),MAX(MAX){}
  
  void attach(const int PIN){ servo.attach(PIN); }
  bool inRange(const int angle){
    return (angle>=MIN and angle<=MAX);
  } 
  void write(const int angle){
    if(inRange(angle)) servo.write(angle);
  }
  const int read()const{
    return servo.read();
  }
  
private:
  Servo servo;
  const int MIN;
  const int MAX;

};


class ServoControl{

public:

  ServoControl();

  void attach(const int ARM_1_PIN, const int ARM_2_PIN, const int ARM_3_PIN, const int HAND_PIN);
  bool update();
  void setVelocity(float xDot, float yDot, float zDot);
  void setHandVelocity(float velocity){ handAngleDot = velocity; }

  void printAngles();
  void printPosition();
  
private:

  bool writeServos();
  
  Vector3f theta; //Arm angles
  Vector3f thetaDot; //Arm angle velocities
  Vector3f r; //Cartesian position
  Vector3f rDot; //Cartesian velocity

  float handAngle;
  float handAngleDot;
  
  long prevMillis;
  
  SmartServo arm1;
  SmartServo arm2;
  SmartServo arm3;
  SmartServo hand;

};

#endif

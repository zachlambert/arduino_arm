
#include <Arduino.h>
#include "servoControl.h"

//To convert from arm angle to servo angle, add the offset
const int ANGLE_1_OFFSET = -3;
const int ANGLE_2_OFFSET = 10;
const int ANGLE_3_OFFSET = -20;

//Arm angles for home position
const Vector3f HOME_THETA = vectorToRadians(Vector3f(90, 50, 110));
//const int ANGLE_1_HOME = 90;
//const int ANGLE_2_HOME = 50;
//const int ANGLE_3_HOME = 110;

//Servo angle for home position for hand
const int HAND_HOME = 30;

//Ranges of motion for each servo (servo angles)
const int ARM_1_MIN = 0;
const int ARM_1_MAX = 120;
const int ARM_2_MIN = 10;
const int ARM_2_MAX = 90;
const int ARM_3_MIN = 50;
const int ARM_3_MAX = 180;
const int HAND_MIN = 10;
const int HAND_MAX = 180; 

ServoControl::ServoControl():
      arm1(ARM_1_MIN, ARM_1_MAX),
      arm2(ARM_2_MIN, ARM_2_MAX),
      arm3(ARM_3_MIN, ARM_3_MAX),
      hand(HAND_MIN, HAND_MAX){

  theta = HOME_THETA;
  handAngle = HAND_HOME;
  
  writeMotionServos(theta);
  writeHandServo(handAngle);
}

void ServoControl::attach(const int ARM_1_PIN, const int ARM_2_PIN, const int ARM_3_PIN, const int HAND_PIN){
  arm1.attach(ARM_1_PIN);
  arm2.attach(ARM_2_PIN);
  arm3.attach(ARM_3_PIN);
  hand.attach(HAND_PIN);

  prevMillis = millis();
}

void ServoControl::update(){
  
  //Calculate elapsed time
  
  long currentMillis = millis();
  long elapsedMillis = currentMillis - prevMillis;
  prevMillis = currentMillis;
  
  float seconds = ((float)elapsedMillis) / 1000.0;

  //Update hand servo

  float newHandAngle = handAngle + handAngleDot*seconds;
  
  if(writeHandServo(newHandAngle)){
    handAngle = newHandAngle;
  }

  //Update motion servos
  
  Vector3f newTheta = theta + thetaDot*seconds;
  
  if(writeMotionServos(newTheta)){
    theta = newTheta;
  }
}

void ServoControl::setCartesianVelocity(float xDot, float yDot, float zDot){
  rDot.x = xDot;
  rDot.y = yDot;
  rDot.z = zDot;
  thetaDot = getThetaDotForCartesian(theta, rDot);
}

void ServoControl::setPolarVelocity(float rScalarDot, float thetaScalarDot, float zDot){
  rDot.x = rScalarDot;
  rDot.y = thetaScalarDot;
  rDot.z = zDot;
  thetaDot = getThetaDotForPolar(theta, rDot);
}

void ServoControl::printAngles(){
  Vector3f degs = vectorToDegrees(theta);
  printVector(degs);  
}
void ServoControl::printPosition(){
  printVector(getCartesian(theta));  
}
bool ServoControl::writeMotionServos(const Vector3f& theta){

  //The arm servos are only written to if all movement is valid (within range)
  
  float servoAngle1 = degrees(theta.x) + ANGLE_1_OFFSET;
  float servoAngle2 = degrees(theta.y) + ANGLE_2_OFFSET;
  float servoAngle3 = degrees(theta.z) + ANGLE_3_OFFSET;

  if(not arm1.inRange(servoAngle1))return false;
  if(not arm2.inRange(servoAngle2))return false;
  if(not arm3.inRange(servoAngle3))return false;
  
  arm1.write(servoAngle1);
  arm2.write(servoAngle2);
  arm3.write(servoAngle3);

  return true;
}
bool ServoControl::writeHandServo(const float angle){

  if(hand.inRange(angle)){
    hand.write(angle);  
    return true;
  }else{
    return false;  
  }
  
}

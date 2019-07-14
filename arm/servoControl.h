#ifndef SERVO_CONTROL_H_GUARD
#define SERVO_CONTROL_H_GUARD

#include <Servo.h>
#include <Arduino.h>

class SmartServo{

public:
  SmartServo(const int PIN, const int HOME, const int MIN, const int MAX):
    servo(), PIN(PIN), speed(0), pos(90),
    HOME(HOME), MIN(MIN), MAX(MAX), prevMillis(0){}
  
  void init();

  void goHome(){ servo.write(HOME); }
  void setSpeed(float speed){ this->speed = speed; }
  float getSpeed()const{ return speed; }
  
  float getPos()const{ return pos; }
  bool atLimit()const{ return pos<MIN+1 or pos>MAX-1; }
  
  void update();
    
private:
  Servo servo;
  const int PIN;
  float speed; //Radians per second
  float pos;
  
  const int HOME;
  const int MIN;
  const int MAX;
  
  long prevMillis;

};

class ServoControl{

public:

  ServoControl(const int SERVO_PIN_1, const int SERVO_PIN_2, const int SERVO_PIN_3, const int SERVO_PIN_4):
    servo1(SERVO_PIN_1, 87, 0, 120), servo2(SERVO_PIN_2, 60, 10, 90),
    servo3(SERVO_PIN_3, 90, 50, 180), servo4(SERVO_PIN_4, 30, 10, 180){}

  void init();
  void update();

  SmartServo servo1;
  SmartServo servo2;
  SmartServo servo3;
  SmartServo servo4;
  
};

#endif

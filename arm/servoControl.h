#ifndef SERVO_CONTROL_H_GUARD
#define SERVO_CONTROL_H_GUARD

#include <Servo.h>

class SmartServo{

public:
  SmartServo(const int PIN, float defaultSpeed, const int HOME, const int MIN, const int MAX):
    servo(), PIN(PIN), speed(defaultSpeed), pos(90), target(90),
    HOME(HOME), MIN(MIN), MAX(MAX), prevMillis(0){}
  
  void init();
  
  void setTarget(int target){ this->target = target; }
  void move(int delta){ target = pos+delta; }

  void goHome(){ target = HOME; }
  void setSpeed(float speed){ this->speed = speed; }

  float getPos()const{ return pos; }
  
  void update();
    
private:
  Servo servo;
  const int PIN;
  float speed; //Radians per second
  float pos;
  int target;
  
  const int HOME;
  const int MIN;
  const int MAX;
  
  long prevMillis;

};

class ServoControl{

public:

  ServoControl(const int SERVO_PIN_1, const int SERVO_PIN_2, const int SERVO_PIN_3,
               const int SERVO_PIN_4, float defaultSpeed):
    servo1(SERVO_PIN_1, defaultSpeed, 87, 0, 120), servo2(SERVO_PIN_2, defaultSpeed, 60, 10, 90),
    servo3(SERVO_PIN_3, defaultSpeed, 90, 50, 180), servo4(SERVO_PIN_4, defaultSpeed, 30, 10, 180){}

  void init();
  void update();

  SmartServo servo1;
  SmartServo servo2;
  SmartServo servo3;
  SmartServo servo4;
  
};

#endif

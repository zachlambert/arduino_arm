#ifndef SERVO_CONTROL_H_GUARD
#define SERVO_CONTROL_H_GUARD

#include <Servo.h>

class ServoControl{

public:

  ServoControl(const int SERVO_PIN_1, const int SERVO_PIN_2, const int SERVO_PIN_3, const int SERVO_PIN_4):
    SERVO_PIN_1(SERVO_PIN_1), SERVO_PIN_2(SERVO_PIN_2), SERVO_PIN_3(SERVO_PIN_3), SERVO_PIN_4(SERVO_PIN_4){}

  void init();
 
private:

  const int SERVO_PIN_1;
  const int SERVO_PIN_2;
  const int SERVO_PIN_3;
  const int SERVO_PIN_4;

  Servo servo1;
  Servo servo2;
  Servo servo3;
  Servo servo4;
  
};

#endif


#include "servoControl.h"

void ServoControl::init(){

  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);
  servo3.attach(SERVO_PIN_3);
  servo4.attach(SERVO_PIN_4);

  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  
}


#include "servoControl.h"
#include <Arduino.h>

void SmartServo::init(){
  servo.attach(PIN, MIN, MAX);
  pos = HOME;
  servo.write(pos);
}

void SmartServo::update(){

  long currentMillis = millis();
  long elapsedMillis = currentMillis - prevMillis;
  prevMillis = currentMillis;
  
  float speedDegrees = speed * (180.0/3.14159);
  
  float deltaPos = (speedDegrees*elapsedMillis)/1000.0;
    
  pos+=deltaPos;
  if(pos<MIN) pos=MIN;
  if(pos>MAX) pos=MAX;
  
  servo.write((int)pos);
  
}

void ServoControl::init(){
  servo1.init();
  servo2.init();
  servo3.init();
  servo4.init();
}

void ServoControl::update(){
  servo1.update();
  servo2.update();
  servo3.update();
  servo4.update();
}

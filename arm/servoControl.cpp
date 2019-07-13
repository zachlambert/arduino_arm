
#include "servoControl.h"
#include <Arduino.h>

void SmartServo::init(){
  servo.attach(PIN, MIN, MAX);
  target = HOME;
  pos = servo.read();
}

void SmartServo::update(){

  int posDif = target - pos;
  if(abs(posDif)<1) return;
  
  long currentMillis = millis();
  long elapsedMillis = currentMillis - prevMillis;
  prevMillis = currentMillis;
  Serial.println(elapsedMillis);
  
  int dir = (posDif>0?1:-1);
  Serial.println(dir);
  
  float speedDegrees = speed * (180.0/3.14159);
  Serial.println(speedDegrees);
  
  float deltaPos = dir*((speedDegrees*elapsedMillis)/1000.0);
  Serial.println(deltaPos);
  
  if(abs(deltaPos)>abs(posDif)){
    pos = target;  
  }else{
    pos += deltaPos;  
  }
  Serial.println(pos);
  Serial.println(target);
  Serial.println("");

  servo.write((int)pos);
  
}

void ServoControl::init(){
  servo1.init();
  servo2.init();
  servo3.init();
  servo4.init();

  servo4.setTarget(0);
  servo2.setTarget(145);
}

void ServoControl::update(){
  servo1.update();
  servo2.update();
  servo3.update();
  servo4.update();
}

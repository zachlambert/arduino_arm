
#include "buttons.h"
#include <Arduino.h>

void Buttons::init(){
  pinMode(movePin, INPUT_PULLUP);
  pinMode(closePin, INPUT_PULLUP);
  pinMode(openPin, INPUT_PULLUP);  
}

void Buttons::update(){
  movePrev = moveValue;
  moveValue = digitalRead(movePin);
  closeValue = digitalRead(closePin);
  openValue = digitalRead(openPin);
}
  
bool Buttons::moveButtonDown(){
  return moveValue==0;
}
bool Buttons::moveButtonUp(){
  return moveValue==1;
}
bool Buttons::moveButtonJustDown(){
  return moveValue==0 and movePrev==1;
}
bool Buttons::closeButtonDown(){
  return closeValue==0;
}
bool Buttons::closeButtonUp(){
  return closeValue==1;
}
bool Buttons::openButtonDown(){
  return openValue==0;
}
bool Buttons::openButtonUp(){
  return openValue==1;
}

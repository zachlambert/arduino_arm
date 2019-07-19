
#include "buttons.h"
#include <Arduino.h>

void Button::init(){
  pinMode(pin, INPUT_PULLUP);  
}
void Button::update(){
  prev = value;
  value = digitalRead(pin);
}
bool Button::isDown()const{
  return value==0; //Active low  
}
bool Button::justDown()const{
  return value==0 and prev==1;  
}

void Buttons::init(){
  buttonA.init();
  buttonB.init();
  buttonC.init();
  buttonD.init();
  buttonE.init(); 
}

void Buttons::update(){
  buttonA.update();
  buttonB.update();
  buttonC.update();
  buttonD.update();
  buttonE.update(); 
}

int16_t Buttons::getStatus()const{

  int16_t status = 0;
  status|=buttonA.isDown();
  status|=buttonB.isDown()<<1;
  status|=buttonC.isDown()<<2;
  status|=buttonD.isDown()<<3;
  status|=buttonE.isDown()<<4;

  return status;
}

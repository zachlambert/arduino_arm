//Include Libraries

#include "comms.h"
#include "servoControl.h"

//Communication

Comms comms(8, 7, "00001");

//Servos

ServoControl servoControl(5, 6, 9, 10, 1.5);

void setup(){
  
  Serial.begin(9600);
  
  comms.init();
  servoControl.init();
}

void loop(){
  //Read the data if available in buffer
  /*
  if (comms.available()){
    int16_t data[5] = {0,0,0,0,0};
    comms.readInts(data, 5);
    Serial.print(data[0]);
    Serial.print(" | ");
    Serial.print(data[1]);
    Serial.print(" | ");
    Serial.print(data[2]);
    Serial.print(" | ");
    Serial.print(data[3]);
    Serial.print(" | ");
    Serial.print(data[4]);
    Serial.println("");
  }
  */

  servoControl.update();
  
}

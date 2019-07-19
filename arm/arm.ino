//Include Libraries

#include "comms.h"
#include "servoControl.h"

//Communication

Comms comms(8, 7, "00001");

//Servos

ServoControl servoControl;
  
void setup(){
  
  Serial.begin(9600);
  
  comms.init();
  
  servoControl.attach(5, 6, 9, 10);

}


void loop(){
  //Read the data if available in buffer
  
  if (comms.available()){
    int16_t data[2] = {0,0};
    comms.readInts(data, 2);

    int isV1 = data[1]>>0 & 0x1;
    int isV2 = data[1]>>1 & 0x1;
    int isV3 = data[1]>>2 & 0x1;
    int isVHand = data[1]>>3 & 0x1;
    int coordSystemSelect = data[1]>>4 & 0x1;

    float v1 = 0;
    float v2 = 0;
    float v3 = 0;
    float vHand = 0;
    
    if(isV1){
      v1 = data[0] / 250;    
    }else if(isV2){
      v2 = data[0] / 250;  
    }else if(isV3){
      v3 = data[0] / 250;  
    }else if(isVHand){
      vHand = data[0] / 25;  
    }

    Serial.print(v1);
    Serial.print(" | ");
    Serial.print(v2);
    Serial.print(" | ");
    Serial.print(v3);
    Serial.print(" | ");
    Serial.print(vHand);
    Serial.println("");

    if(coordSystemSelect==1){
      servoControl.setCartesianVelocity(v1,v2,v3);
    }else{
      servoControl.setPolarVelocity(v1,v2,v3);  
    }
    
    servoControl.setHandVelocity(vHand);
  }
  

  servoControl.update();
  //servoControl.printAngles();
  
}

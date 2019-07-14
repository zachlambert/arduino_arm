//Include Libraries

#include "comms.h"
#include "servoControl.h"

//Communication

Comms comms(8, 7, "00001");

//Servos

ServoControl servoControl(5, 6, 9, 10);

void setup(){
  
  Serial.begin(9600);
  
  comms.init();
  servoControl.init();

  servoControl.servo2.setSpeed(1.2);
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

  float angle1 = radians(servoControl.servo1.getPos());
  float angle2 = radians(servoControl.servo2.getPos()-20);
  float angle3 = radians(servoControl.servo3.getPos()+10);
  
  float speed2 = servoControl.servo2.getSpeed();
  float speed3 = - speed2 * (sin(angle2)/cos(angle1));
  
  servoControl.servo3.setSpeed(speed3);  

  //float rSpeed = 8.1*cos(angle2)*speed2 + 8.1*sin(angle3)*speed3;
  float r = 1.8 + 8.1*sin(angle2) - 8.1*cos(angle3) + 6.3;

  //float speed1 = - (sin(angle1)/(r*cos(angle1)))*rSpeed;
  //servoControl.servo1.setSpeed(speed1);

  Serial.println(speed3);
  
  if(servoControl.servo2.atLimit() or servoControl.servo3.atLimit()){
      servoControl.servo2.setSpeed(-servoControl.servo2.getSpeed());
      servoControl.servo3.setSpeed(-servoControl.servo3.getSpeed());
      delay(100);
  }
    
  /*
  if(servoControl.servo3.atLimit() and servoControl.servo2.atLimit()){
    servoControl.servo3.setSpeed(-servoControl.servo3.getSpeed());  
    servoControl.servo2.setSpeed(-servoControl.servo2.getSpeed());  
  }

  float angle2 = radians(servoControl.servo2.getPos()-20);
  float angle3 = radians(servoControl.servo3.getPos()+10);
  float r = 1.8 + 8.1*sin(angle2) - 8.1*cos(angle3) + 6.3;
  Serial.println(r);
  */
  
  servoControl.update();
  
}

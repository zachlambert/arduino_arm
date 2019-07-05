

#include "motion.h"
#include "Comms.h"

Motion motion;
Comms comms(9, 8, "00001");

void setup() {
  
  Serial.begin(9600);
  
  motion.init();
  comms.init();

}

void loop() {

  //Read from MPU

  motion.update();

  //Print to Serial

  Serial.print("X = ");
  Serial.print(motion.getAcX());
  Serial.print(", Y = ");
  Serial.print(motion.getAcY());
  Serial.print(", Z = ");
  Serial.print(motion.getAcZ());
  Serial.println("");

  //Write to radio

  const char my_string[] = "Hello World v2!";
  //comms.sendString(my_string,sizeof(my_string));

  comms.sendInt(motion.getAcX());
  
  delay(200);
}

//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//create an RF24 object
RF24 radio(8, 7);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

//Servos

#define SERVO_4_PIN 10 //Claw servo
#define SERVO_3_PIN 9 //Smallest range of motion pivot
#define SERVO_2_PIN 6 //Medium range of motion pivot
#define SERVO_1_PIN 5 //Largest range of motion pivot

/*
Servo servo4;
Servo servo3;
Servo servo2;
Servo servo1;
*/

void setup(){
  
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  //Set module as receiver
  radio.startListening();

  /*
  servo4.attach(SERVO_4_PIN);
  servo3.attach(SERVO_3_PIN);
  servo2.attach(SERVO_2_PIN);
  servo1.attach(SERVO_1_PIN);

  servo4.write(90);
  servo3.write(90);
  servo2.write(90);
  servo1.write(70);
  */
}

void loop(){
  //Read the data if available in buffer
  if (radio.available()){
    int16_t data[5] = {0,0,0,0,0};
    radio.read(data, 2*5);
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
}

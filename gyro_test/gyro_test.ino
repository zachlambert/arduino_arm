
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//MPU

const int MPU_ADDR = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

//Radio

RF24 radio(9, 8); //CE, CSN

const byte address[6] = "00001";

void setup() {
  //MPU
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  //Radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  //Serial
  Serial.begin(9600);
}

void loop() {

  //Read from MPU

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 12, true);

  AcX = Wire.read()<<8 | Wire.read();
  AcY = Wire.read()<<8 | Wire.read();  
  AcZ = Wire.read()<<8 | Wire.read();  
  GyX = Wire.read()<<8 | Wire.read();  
  GyY = Wire.read()<<8 | Wire.read();  
  GyZ = Wire.read()<<8 | Wire.read(); 

  //Print to Serial

  Serial.print("Accelerometer: ");
  Serial.print("X = ");
  Serial.print(AcX);
  Serial.print(", Y = ");
  Serial.print(AcY);
  Serial.print(", Z = ");
  Serial.print(AcZ);
  Serial.println("");

  Serial.print("Gyroscope: ");
  Serial.print("X = ");
  Serial.print(GyX);
  Serial.print(", Y = ");
  Serial.print(GyY);
  Serial.print(", Z = ");
  Serial.print(GyZ);
  Serial.println("");

  //Write to radio

  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  
  delay(200);
}

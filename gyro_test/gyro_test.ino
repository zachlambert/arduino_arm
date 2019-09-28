
#include <Wire.h>
#include <SPI.h>

//MPU

const int MPU_ADDR = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() {
  //MPU
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  //Serial
  Serial.begin(9600);
}

void loop() {
  //Read from MPU

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 12, true);

  AcX = (Wire.read()<<8 | Wire.read()) / 1;
  AcY = (Wire.read()<<8 | Wire.read()) / 1;  
  AcZ = (Wire.read()<<8 | Wire.read()) / 1;  
  GyX = (Wire.read()<<8 | Wire.read()) / 131;  
  GyY = (Wire.read()<<8 | Wire.read()) / 131;  
  GyZ = (Wire.read()<<8 | Wire.read()) / 131; 


  
  //Print to Serial 
  
  Serial.print(AcX);
  Serial.print(", ");
  Serial.print(AcY);
  Serial.print(", ");
  Serial.print(AcZ);
  Serial.println("");

  delay(200);
}

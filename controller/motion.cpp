
#include "motion.h"

void Motion::init(){
  Wire.begin();
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void Motion::update(){
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(I2C_ADDR, 2*3, true);

  AcX = Wire.read()<<8 | Wire.read();
  AcY = Wire.read()<<8 | Wire.read();  
  AcZ = Wire.read()<<8 | Wire.read();  
}

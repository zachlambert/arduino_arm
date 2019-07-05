#ifndef MOTION_H_GUARD
#define MOTION_H_GUARD

#include <Wire.h>

struct MotionData{
  
  int16_t dx;
  int16_t dy;
  int16_t dz;
  MotionData():dx(0),dy(0),dz(0){}
  MotionData(int16_t dx, int16_t dy, int16_t dz):dx(dx),dy(dy),dz(dz){}  
  
};


class Motion{

public:
  Motion(const int I2C_ADDR=0x68):I2C_ADDR(I2C_ADDR),AcX(0),AcY(0),AcZ(0),motionData(){}
  void init();
  void update();
  MotionData getMotionData()const{ return motionData; }

  int16_t getAcX()const{ return AcX; }
  int16_t getAcY()const{ return AcY; }
  int16_t getAcZ()const{ return AcZ; }
  
private:
  const int I2C_ADDR;
  int16_t AcX, AcY, AcZ;
  MotionData motionData;
  
};

#endif

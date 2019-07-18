#ifndef MOTION_H_GUARD
#define MOTION_H_GUARD

#include <Arduino.h>

const int SMOOTH_LENGTH = 6;

class Motion{

public:
  Motion(){}

  void updateAcceleration(int16_t ax, int16_t ay, int16_t az);
  void updateRoll(int16_t rawRoll);
  void updateVelocity();
   
  
  void resetVelocity(){ vx=0; vy=0; vz=0; }
  void resetVelocityX(){ vx=0; }
  void resetVelocityY(){ vy=0; }
  void resetVelocityZ(){ vz=0; }
  
  int16_t getVelocityX()const{ return vx; }
  int16_t getVelocityY()const{ return vy; }
  int16_t getVelocityZ()const{ return vz; }
  int16_t getRoll()const{ return roll; }

private:

  int16_t axs[SMOOTH_LENGTH];
  int16_t ays[SMOOTH_LENGTH];
  int16_t azs[SMOOTH_LENGTH];

  int16_t vx;
  int16_t vy;
  int16_t vz;
  int16_t roll;
  
  long prevMillis;
  
};


#endif

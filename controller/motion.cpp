
#include "motion.h"

void updateArray(int16_t newValue, int16_t* start, int16_t len){
  for(int i=len-2;i>=0;i--){
    start[i+1] = start[i];
  }  
  start[0] = newValue;  
}

int16_t averageArray(int16_t* start, int16_t len){
  int16_t sum = 0;
  for(int16_t i=0;i<len;i++){
    sum += start[i];  
  }
  return sum/len;  
}


void Motion::updateAcceleration(int16_t ax, int16_t ay, int16_t az){
  updateArray(ax, axs, SMOOTH_LENGTH);
  updateArray(ay, ays, SMOOTH_LENGTH);
  updateArray(az, azs, SMOOTH_LENGTH);
}

void Motion::updateVelocity(){
  long currentMillis = millis();
  long elapsedMillis = currentMillis - prevMillis;
  prevMillis = currentMillis;

  vx += (averageArray(axs, SMOOTH_LENGTH) * elapsedMillis) / 1000;
  vy += (averageArray(ays, SMOOTH_LENGTH) * elapsedMillis) / 1000;
  vz += (averageArray(azs, SMOOTH_LENGTH) * elapsedMillis) / 1000;
  
}

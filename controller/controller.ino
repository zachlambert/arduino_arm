
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "comms.h"
#include "mpuData.h"

//MPU

MPU6050 mpu;
MPUData mpuData;

#define INTERRUPT_PIN 2
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

//Smoothed acceleration x (test)

const int SMOOTH_LENGTH = 3;
int16_t accelStore[SMOOTH_LENGTH];

void updateAccelStore(int16_t newInt){
  for(int i=SMOOTH_LENGTH-2;i>=0;i--){
    accelStore[i+1] = accelStore[i];
  }  
  accelStore[0] = newInt;
}
int16_t getSmoothedAccel(){
  int16_t sum = 0;
  for(int i=0;i<SMOOTH_LENGTH;i++){
    sum += accelStore[i];  
  }
  return sum/SMOOTH_LENGTH;
}

long prevMillis = 0;
int16_t vx = 0;

//Radio

Comms comms(9, 8, "00001");

void setup_mpu(){

    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    mpuData.devStatus = mpu.dmpInitialize();

    mpu.setXGyroOffset(109);
    mpu.setYGyroOffset(-76);
    mpu.setZGyroOffset(-67);
    mpu.setZAccelOffset(9227);

    if (mpuData.devStatus == 0) {
        mpu.setDMPEnabled(true);

        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuData.mpuIntStatus = mpu.getIntStatus();
        mpuData.dmpReady = true;

        mpuData.packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(mpuData.devStatus);
        Serial.println(F(")"));
    }
}

void update_mpu(){
  
  if (!mpuData.dmpReady) return;
  
  while (!mpuInterrupt && mpuData.fifoCount < mpuData.packetSize) {
    if (mpuInterrupt && mpuData.fifoCount < mpuData.packetSize) {
      mpuData.fifoCount = mpu.getFIFOCount();
    }  
  }

  mpuInterrupt = false;
  mpuData.mpuIntStatus = mpu.getIntStatus();
  
  // get current FIFO count
  mpuData.fifoCount = mpu.getFIFOCount();

  if ((mpuData.mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || mpuData.fifoCount >= 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      mpuData.fifoCount = mpu.getFIFOCount();
      Serial.println(F("FIFO overflow!"));
  
  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuData.mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {

      // wait for correct available data length, should be a VERY short wait
      while (mpuData.fifoCount < mpuData.packetSize) mpuData.fifoCount = mpu.getFIFOCount();
  
      // read a packet from FIFO
      mpu.getFIFOBytes(mpuData.fifoBuffer, mpuData.packetSize);
      mpu.resetFIFO();

      
      // track FIFO count here in case there is > 1 packet available
      // (this lets us immediately read more without waiting for an interrupt)
      mpuData.fifoCount -= mpuData.packetSize;

      //Extract relevant information

      // display real acceleration, adjusted to remove gravity
      mpu.dmpGetQuaternion(&mpuData.q, mpuData.fifoBuffer);
      mpu.dmpGetAccel(&mpuData.aa, mpuData.fifoBuffer);
      mpu.dmpGetGravity(&mpuData.gravity, &mpuData.q);
      mpu.dmpGetYawPitchRoll(mpuData.ypr, &mpuData.q, &mpuData.gravity);
      mpu.dmpGetLinearAccel(&mpuData.aaReal, &mpuData.aa, &mpuData.gravity);
      mpu.dmpGetLinearAccelInWorld(&mpuData.aaWorld, &mpuData.aaReal, &mpuData.q);
       
      updateAccelStore(mpuData.aaWorld.x);

      //Serial.println(getSmoothedAccel());
      //Serial.println(mpuData.aaWorld.x);

      long elapsedMillis = prevMillis - millis();
      prevMillis = millis();

      vx += ( getSmoothedAccel() * elapsedMillis ) / 1000;
      
      Serial.println(vx);

  }  
}

void setup() {
  
  Serial.begin(9600);

  setup_mpu();
  comms.init();

}

void loop() {

  //Read from MPU

  update_mpu();
  
  //Write to radio

  //const char my_string[] = "Hello World v2!";
  //comms.sendString(my_string,sizeof(my_string));

  //comms.sendInt(-37);
}

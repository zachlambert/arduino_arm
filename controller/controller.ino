
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
      
      // display Euler angles in degrees
      mpu.dmpGetQuaternion(&mpuData.q, mpuData.fifoBuffer);
      mpu.dmpGetGravity(&mpuData.gravity, &mpuData.q);
      mpu.dmpGetYawPitchRoll(mpuData.ypr, &mpuData.q, &mpuData.gravity);
      /*
      Serial.print("ypr\t");
      Serial.print(mpuData.ypr[0] * 180/M_PI);
      Serial.print("\t");
      Serial.print(mpuData.ypr[1] * 180/M_PI);
      Serial.print("\t");
      Serial.println(mpuData.ypr[2] * 180/M_PI);
      */

      // display real acceleration, adjusted to remove gravity
      mpu.dmpGetQuaternion(&mpuData.q, mpuData.fifoBuffer);
      mpu.dmpGetAccel(&mpuData.aa, mpuData.fifoBuffer);
      mpu.dmpGetGravity(&mpuData.gravity, &mpuData.q);
      mpu.dmpGetLinearAccel(&mpuData.aaReal, &mpuData.aa, &mpuData.gravity);

      Serial.println(mpuData.aaReal.y);

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

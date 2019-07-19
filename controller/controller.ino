
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "comms.h"
#include "mpuData.h"
#include "buttons.h"

//MPU

MPU6050 mpu;
MPUData mpuData;

#define INTERRUPT_PIN 3
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

//Communication

Comms comms(9, 8, "00001");

//Buttons

Buttons buttons(4,5,6,7,10);

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

    mpu.setXGyroOffset(111);
    mpu.setYGyroOffset(-74);
    mpu.setZGyroOffset(-74);
    mpu.setXAccelOffset(7267);
    mpu.setYAccelOffset(-5873);
    mpu.setZAccelOffset(9213);

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

      mpu.dmpGetQuaternion(&mpuData.q, mpuData.fifoBuffer);
      mpu.dmpGetGravity(&mpuData.gravity, &mpuData.q);
      mpu.dmpGetYawPitchRoll(mpuData.ypr, &mpuData.q, &mpuData.gravity);

  }  
}

void setup() {
  
  Serial.begin(9600);

  setup_mpu();
  comms.init();
  buttons.init();
  
}

void loop() {

  //Read from MPU

  update_mpu();

  //Update buttons and get status
  
  buttons.update();
  int16_t status = buttons.getStatus();
    
  //Extract roll, convert to a relative value

  int16_t rawRoll = degrees(mpuData.ypr[2]);
  const int16_t rollOffset = 90;
  int16_t roll = rawRoll - rollOffset;
  if(roll>180){
    roll-=360;  
  }
  if(roll<-180){
    roll+=360;  
  }

  //Measure sensitivity

  //int potValue = analogRead(0); TODO
  int potValue = 500;
  int sensitivity = map(potValue,0,1024,0,100);

  //Calculate velocity

  //(Magnitude should be large to increase resolution, arm will divide the value)
  //Velocity is in the range [-18,000,18,000]
  int16_t velocity = roll * sensitivity;
  
  //Write to radio
  
  int16_t data[2] = {0, 0};

  data[0] = velocity;
  data[1] = status;

  Serial.print(data[0]);
  Serial.print(" | ");
  Serial.print(data[1], BIN);
  Serial.println("");
  
  comms.sendInts(data, 2);
}


#include <Servo.h>

int servoIndex = 0;
const int numServos = 4;
Servo servos[numServos];
int servoValues[numServos];

int buttonValue;

int prevServoValue = 0;

void setup() {
  // put your setup code here, to run once:
  servos[0].attach(5);
  servos[1].attach(6);
  servos[2].attach(9);
  servos[3].attach(10);

  servoValues[0] = servos[0].read();
  servoValues[1] = servos[1].read();
  servoValues[2] = servos[2].read();
  servoValues[3] = servos[3].read();

  prevServoValue = map(analogRead(A0),0,1024,0,180);
  
  pinMode(4, INPUT_PULLUP);
  buttonValue = digitalRead(4);
  
  Serial.begin(9600);
}

void loop() {
  int servoValue = map(analogRead(A0),0,1024,0,360); //Use 360, so there is more room to rotate the pot before reaching the limit
  int dif = servoValue - prevServoValue;
  prevServoValue = servoValue;
  
  servoValues[servoIndex] += dif;
  if(servoValues[servoIndex]<0) servoValues[servoIndex] = 0;
  if(servoValues[servoIndex]>180) servoValues[servoIndex] = 180;
  
  servos[servoIndex].write(servoValues[servoIndex]);
  Serial.println(servoValues[servoIndex]);

  int newButtonValue = digitalRead(4);
  if(newButtonValue==0 and buttonValue==1){
    servoIndex++;
    servoIndex%=numServos;
  }
  buttonValue = newButtonValue;
}

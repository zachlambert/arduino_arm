
#include "comms.h"

void Comms::init(){
  
  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, pipeAddress);
  radio.setPALevel(RF24_PA_MIN);
  //Set module as receiver
  radio.startListening();

}

void Comms::readInts(int16_t* start, int16_t numInts){
  radio.read(start, 2*numInts);
}

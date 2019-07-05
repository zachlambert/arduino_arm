
#include "comms.h"

void Comms::init(){
  radio.begin();
  radio.openWritingPipe(pipe_address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();  
}

void Comms::sendString(const char* data, int length){
  radio.write(data, length);  
}

void Comms::sendInt(const int16_t data){
  radio.write(&data,2);  
}

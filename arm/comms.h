#ifndef COMMS_H_GUARD
#define COMMS_H_GUARD

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

class Comms{

public:
  Comms(int CE, int CSN, const byte* pipeAddress):radio(CE, CSN),
    pipeAddress(pipeAddress){}
  void init();
  void readInts(int16_t* start, int16_t length);
  bool available(){ return radio.available(); }
  
private:
  RF24 radio;
  const byte* pipeAddress;
  
};

#endif

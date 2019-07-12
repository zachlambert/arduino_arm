#ifndef COMMS_H_GUARD
#define COMMS_H_GUARD

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

class Comms{

public:
  Comms(int CE, int CSN, const byte* pipe_address):radio(CE, CSN),
    pipe_address(pipe_address){}
  void init();
  void sendString(const char* data, int16_t length);
  void sendInt(const int16_t data);
  void sendInts(const int16_t* start, int16_t length);
  
private:
  RF24 radio;
  const byte* pipe_address;
  
};

#endif

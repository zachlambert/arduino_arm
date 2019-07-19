#ifndef BUTTONS_H_GUARD
#define BUTTONS_H_GUARD

class Button{

public:
  Button(int pin):pin(pin), value(0), prev(prev){}
  void init();
  void update();

  bool isDown()const;
  bool justDown()const;

  int getValue()const{ return value; }
  
private:
  int pin;
  int value;
  int prev;
};

class Buttons{

public:
  
  Buttons(int buttonAPin, int buttonBPin, int buttonCPin, int buttonDPin, int buttonEPin):
    buttonA(buttonAPin), buttonB(buttonBPin), buttonC(buttonCPin), buttonD(buttonDPin),
    buttonE(buttonEPin) {}

  void init();
  void update();
  int getStatus()const;
  
private:
  
  Button buttonA;
  Button buttonB;
  Button buttonC;
  Button buttonD;
  Button buttonE;
  
};

#endif

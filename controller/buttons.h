#ifndef BUTTONS_H_GUARD
#define BUTTONS_H_GUARD

class Buttons{

public:
  
  Buttons(int movePin, int closePin, int openPin):
    movePin(movePin), moveValue(0), movePrev(0),
    closePin(closePin), closeValue(0),
    openPin(openPin), openValue(0)
    {}

  void init();
  void update();
  
  bool moveButtonDown();
  bool moveButtonUp();
  bool moveButtonJustDown();
  
  bool closeButtonDown();
  bool closeButtonUp();
  
  bool openButtonDown();
  bool openButtonUp();
  
private:
  
  int movePin;
  int moveValue;
  int movePrev;
  
  int closePin;
  int closeValue;
  
  int openPin;
  int openValue;
  
};

#endif

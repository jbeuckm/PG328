#include "Arduino.h"

#define pinA 2
#define pinB 3


class RotaryEncoder {

private:
  int BUTTON_PIN; // will be pulled high and activated by connecting to 0V
  boolean buttonState;


  volatile byte aFlag; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
  volatile byte bFlag; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
  volatile byte encoderPos; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
  volatile byte oldEncPos; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
  volatile byte reading; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
  
  void (*mRotateCallback)(int direction);
  void (*mButtonDownCallback)(void);
  void (*mButtonUpCallback)(void);
    
public:

  void PinA();
  void PinB();

  RotaryEncoder(int buttonPin);

  byte getPosition();
  boolean checkButton();

  void setHandleRotate(void (*fptr)(int direction));
  void setHandleButtonDown(void (*fptr)(void));        
  void setHandleButtonUp(void (*fptr)(void));        
};



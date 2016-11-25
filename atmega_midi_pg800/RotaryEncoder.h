#include "Arduino.h"

class RotaryEncoder {

  int pinA; // Our first hardware interrupt pin is digital pin 2
  int pinB; // Our second hardware interrupt pin is digital pin 3
  volatile byte aFlag; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
  volatile byte bFlag; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
  volatile byte encoderPos; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
  volatile byte oldEncPos; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
  volatile byte reading; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

public:

  void PinA();
  void PinB();

  RotaryEncoder();

  void setup();
  byte getPosition();

};



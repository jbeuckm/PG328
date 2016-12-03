#include "RotaryEncoder.h"


void RotaryEncoder::PinA(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    
    if (mRotateCallback != 0) mRotateCallback(-1);
    
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void RotaryEncoder::PinB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    
    if (mRotateCallback != 0) mRotateCallback(1);
    
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

RotaryEncoder::RotaryEncoder(int buttonPin) {

  buttonState = false;
  BUTTON_PIN = buttonPin;

  mRotateCallback = 0;
  mButtonDownCallback = 0;
  mButtonUpCallback = 0;

  aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
  bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
  reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
}


boolean RotaryEncoder::checkButton() {
  boolean newState = digitalRead(BUTTON_PIN);

  if (!newState && buttonState) {
    if (mButtonDownCallback != 0) mButtonDownCallback();
    buttonState = newState;
  }
  else if (newState && !buttonState) {
    if (mButtonUpCallback != 0) mButtonUpCallback();
    buttonState = newState;
  }

  return newState;
}


void RotaryEncoder::setHandleRotate(void (*fptr)(int direction)) { mRotateCallback = fptr; }
void RotaryEncoder::setHandleButtonDown(void (*fptr)(void)) { mButtonDownCallback = fptr; }
void RotaryEncoder::setHandleButtonUp(void (*fptr)(void)) { mButtonUpCallback = fptr; }



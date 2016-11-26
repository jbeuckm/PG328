#include <MIDI.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <gfxfont.h>

#include "RotaryEncoder.h"
#include "PG800.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

RotaryEncoder wheel(4);

PG800 pg800(10, 8, 9);

MIDI_CREATE_DEFAULT_INSTANCE();

void handleSystemExclusive(byte *message, unsigned size) {

  if (message[1] == 0x41) // manufacturer ID
  if (message[2] == 0x36) // IPR mode
  if (message[3] == 0)    // device ID
  if (message[4] == 0x24) // JX-10
  {  
    MIDI.begin();
    
  }
}



void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  attachInterrupt(0, interruptA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, interruptB, RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  wheel.setHandleButtonDown(buttonDown);
  wheel.setHandleRotate(rotateWheel);
  
  MIDI.setHandleSystemExclusive(handleSystemExclusive);
  
  MIDI.begin();
}

void interruptA() {
  wheel.PinA();
}
void interruptB() {
  wheel.PinB();
}

void loop() {
  MIDI.read();
  wheel.checkButton();
  updateDisplay();
}


boolean controlMode = false;
void buttonDown() {
  controlMode = !controlMode;
}


void rotateWheel(int direction) {
  if (controlMode) {
    if (direction == 1) {
      pg800.nextParam();
    } else {
      pg800.prevParam();
    }
  } else {
    if (direction == 1) {
      pg800.incValue();
    } else {
      pg800.decValue();
    }
  }
}

void updateDisplay() {
  display.clearDisplay();
  
  display.setTextColor(WHITE);

  display.setCursor(3,3);
  display.setTextSize(1);
  display.print(pg800.paramName());

  if (controlMode)
  display.drawRect(0,0,128,13,WHITE);
    

  display.setCursor(4,16);
  display.setTextSize(2);
  display.print(String(pg800.paramValue()));

  if (!controlMode)
  display.drawRect(0,13,128,19,WHITE);

  display.display();
}



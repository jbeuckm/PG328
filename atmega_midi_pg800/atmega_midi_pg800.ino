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

byte muxAddress;
byte potValueIndex;
int potValues[48];
int oldValue, newValue;
#define MOMENTUM .9
#define FORCE .1

// mapping pots to PG800 params
byte potMap[48] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
  30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
  40, 41, 42, 43, 44, 45, 46, 47
};


MIDI_CREATE_DEFAULT_INSTANCE();

void handleSystemExclusive(byte *message, unsigned size) {

  if (message[1] == 0x41) // manufacturer ID
  if (message[2] == 0x36) // IPR mode
  if (message[3] == 0x00) // device ID
  if (message[4] == 0x24) // ???
  if (message[5] == 0x20) // ???
  if (message[6] == 0x01) // Tone
  {  
    pg800.setParam(message[7]);
    pg800.setValue(message[8]);
  }
}



void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  attachInterrupt(0, interruptA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, interruptB, RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  wheel.setHandleButtonDown(buttonDown);
  wheel.setHandleButtonUp(buttonUp);
  wheel.setHandleRotate(rotateWheel);

  DDRB = B1111; // mux address lines as outputs
  initPotValues();
  
  MIDI.setHandleSystemExclusive(handleSystemExclusive);
  
  MIDI.begin();
}

void interruptA() {
  wheel.PinA();
}
void interruptB() {
  wheel.PinB();
}


void initPotValues() {
  for (muxAddress=0; muxAddress<16; muxAddress++) {
    PORTB = muxAddress;
    analogRead(A0);
    potValues[muxAddress] = analogRead(A0);
    analogRead(A1);
    potValues[16 + muxAddress] = analogRead(A1);
    analogRead(A2);
    potValues[32 + muxAddress] = analogRead(A2);
  }
}


void loop() {

  potValueIndex = 0;

  for (muxAddress=0; muxAddress<16; muxAddress++) {
    PORTB = muxAddress;

    oldValue = potValues[potValueIndex];
    analogRead(A0); // toss the initial reading
    // average over previous readings
    newValue = MOMENTUM * oldValue + FORCE * analogRead(A0);
    if ((oldValue >> 3) != (newValue >> 3)) {
      // scaled value change happened
      byte paramIndex = potMap[potValueIndex];
      pg800.setParam(paramIndex);
      pg800.setValue(newValue >> 3);
    }
    potValues[potValueIndex] = newValue;
    potValueIndex++;
    
    potValues[potValueIndex] = analogRead(A1);
    potValueIndex++;
    
    potValues[potValueIndex] = analogRead(A2);
    potValueIndex++;
  }
  
  MIDI.read();
  
  wheel.checkButton();
  updateDisplay();
}


boolean changeParam = false;
void buttonDown() {
  changeParam = true;
}
void buttonUp() {
  changeParam = false;
}


void rotateWheel(int direction) {
  if (changeParam) {
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

  if (changeParam)
  display.drawRect(0,0,128,13,WHITE);
    

  display.setCursor(4,16);
  display.setTextSize(2);
  display.print(String(pg800.paramValue()));

  if (!changeParam)
  display.drawRect(0,13,128,19,WHITE);

  display.display();
}



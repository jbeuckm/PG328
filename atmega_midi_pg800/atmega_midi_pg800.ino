#include <MIDI.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "RotaryEncoder.h"
#include "PG800.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
volatile boolean displayNeedsUpdate = true;

RotaryEncoder wheel(4);
boolean changeParamMode = false;

byte potAssigned = 255;
volatile unsigned long potAssignHoldStartTime;

byte muxAddress;
byte potValueIndex;
boolean ignorePot[48];
byte potValues[48];

// mapping pots to PG800 params
byte potAssignMap[48] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
  30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
  40, 41, 42, 43, 44, 45, 46, 47
};


PG800 pg800(6, 5, 7);

MIDI_CREATE_DEFAULT_INSTANCE();

void handleSystemExclusive(byte *message, unsigned size) {

  if (message[1] == 0x41) // manufacturer ID
  if (message[2] == 0x36) // IPR mode
//  if (message[3] == 0x01) // device ID
  if (message[4] == 0x24) // ???
  if (message[5] == 0x20) // ???
  if (message[6] == 0x01) // Tone
  {  
    pg800.setParam(message[7] - PG800_SYSEX_OFFSET);
    pg800.setValue(message[8]);
    displayNeedsUpdate = true;
  }
}



void setup() {

  for (int i=0; i<48; i++) {
    ignorePot[i] = true;
  }
  
  DDRB = B1111; // mux address lines as outputs
  initPotValues();
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  attachInterrupt(0, interruptA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, interruptB, RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  wheel.setHandleButtonDown(buttonDown);
  wheel.setHandleButtonUp(buttonUp);
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


void initPotValues() {

  potValueIndex = 0;

  for (muxAddress=0; muxAddress<16; muxAddress++) {
    PORTB = muxAddress;
    analogRead(A0);
    potValues[potValueIndex] = analogRead(A0) >> 3;
    potValueIndex++;
    analogRead(A1);
    potValues[potValueIndex] = analogRead(A1) >> 3;
    potValueIndex++;
    analogRead(A2);
    potValues[potValueIndex] = analogRead(A2) >> 3;
    potValueIndex++;
  }
}


int findGreatestValuePot() {
  
  unsigned int greatestPotValue = 0;
  byte greatestPotValueIndex = 0;
  unsigned int readingValue;
  
  potValueIndex = 0;

  for (muxAddress=0; muxAddress<16; muxAddress++) {
    PORTB = muxAddress;

    readingValue = analogRead(A0);
    if (readingValue > greatestPotValue) {
      greatestPotValue = readingValue;
      greatestPotValueIndex = potValueIndex;
    }
    potValueIndex++;

    readingValue = analogRead(A1);
    if (readingValue > greatestPotValue) {
      greatestPotValue = readingValue;
      greatestPotValueIndex = potValueIndex;
    }
    potValueIndex++;

    readingValue = analogRead(A2);
    if (readingValue > greatestPotValue) {
      greatestPotValue = readingValue;
      greatestPotValueIndex = potValueIndex;
    }
    potValueIndex++;
  }

  return greatestPotValueIndex;
}

unsigned long syncTimeout = 0;
byte oldValue, newValue;
unsigned int reading;
byte readingFraction;

void loop() {

  potValueIndex = 0;

  for (muxAddress=0; muxAddress<16; muxAddress++) {
    PORTB = muxAddress;

    if (!ignorePot[potValueIndex]) {

      analogRead(A0); // toss the initial reading
      
      reading = analogRead(A0);
      readingFraction = reading & B111;
      
      // round down
      if (readingFraction < 3) {
        newValue = reading >> 3;

        if (potValues[potValueIndex] != newValue) {
          byte paramIndex = potAssignMap[potValueIndex];
          pg800.setParam(paramIndex);
          pg800.setValue(newValue);
          potValues[potValueIndex] = newValue;
          displayNeedsUpdate = true;
        }
      }
      // dead zone - keep existing value
      else if (readingFraction < 6) {
      }
      // round up
      else {
        newValue = reading >> 3;
        newValue++;

        if (potValues[potValueIndex] != newValue) {
          byte paramIndex = potAssignMap[potValueIndex];
          pg800.setParam(paramIndex);
          pg800.setValue(newValue);
          potValues[potValueIndex] = newValue;
          displayNeedsUpdate = true;
        }
      }
          
    }
    potValueIndex++;
    
    potValues[potValueIndex] = analogRead(A1);
    potValueIndex++;
    
    potValues[potValueIndex] = analogRead(A2);
    potValueIndex++;
  }
  
  MIDI.read();
  
  wheel.checkButton();

  if (changeParamMode && ((millis() - potAssignHoldStartTime) > 3000)) {
    int greatestPotValueIndex = findGreatestValuePot();
    potAssignMap[greatestPotValueIndex] = pg800.getParamIndex();
    potAssigned = greatestPotValueIndex;
    ignorePot[greatestPotValueIndex] = false;
    displayNeedsUpdate = true;
    potAssignHoldStartTime = millis();
  }

  if (displayNeedsUpdate) {

    detachInterrupt(0);
    detachInterrupt(1);
    updateDisplay();
    attachInterrupt(0, interruptA, RISING);
    attachInterrupt(1, interruptB, RISING);
    
    displayNeedsUpdate = false;
  }

  if ((millis() - syncTimeout) > 5) {
    cli();
    pg800.sync();
    sei();
    syncTimeout = millis();
  }
}


void buttonDown() {
  changeParamMode = true;
  displayNeedsUpdate = true;
  potAssignHoldStartTime = millis();
}
void buttonUp() {
  changeParamMode = false;
  displayNeedsUpdate = true;
}


void rotateWheel(int direction) {
  if (changeParamMode) {
    potAssignHoldStartTime = millis();
    if (direction == -1) {
      pg800.nextParam();
    } else {
      pg800.prevParam();
    }
  } else {
    if (direction == -1) {
      pg800.incValue();
    } else {
      pg800.decValue();
    }
  }
  displayNeedsUpdate = true;
}

void updateDisplay() {
  
  display.clearDisplay();
  display.setTextColor(WHITE);

  if (potAssigned != 255) {
    display.setCursor(3,3);
    display.setTextSize(1);
    display.print(potAssigned);
    display.print(" --> ");
    display.print(pg800.paramName());
    display.display();
    potAssigned = 255;
    return;
  }

  display.setCursor(3,3);
  display.setTextSize(1);
  display.print(pg800.paramName());

  if (changeParamMode)
  display.drawRect(0,0,128,13,WHITE);
    

  display.setCursor(4,16);
  display.setTextSize(2);
  display.print(String(pg800.paramValue()));

  if (!changeParamMode)
  display.drawRect(0,13,128,19,WHITE);

  display.display();
}



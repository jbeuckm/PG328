#include <EEPROM.h>
#include <MIDI.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "RotaryEncoder.h"
#include "PG800.h"

#define OLED_RESET 4
Adafruit_SSD1306 *display = new Adafruit_SSD1306(OLED_RESET);
volatile boolean displayNeedsUpdate = true;

PG800 pg800(6, 5, 7);

RotaryEncoder wheel(4);
volatile boolean changeParamMode = false;
volatile unsigned long potAssignHoldStartTime;

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

#define NO_ASSIGNMENT 255
byte potAssigned = NO_ASSIGNMENT;

byte muxAddress;
byte potIndex;
boolean potEnabled[48];
unsigned int potReadings[48];
byte potValues[48];

// mapping pots to PG800 params
byte potAssignMap[48] = {
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
//  if (message[3] == 0x01) // device ID
  if (message[4] == 0x24) // ???
  if (message[5] == 0x20) // ???
  if (message[6] == 0x01) // Tone
  {  
    pg800.setParam(message[7] - PG800_SYSEX_OFFSET);
    pg800.setValue(message[8]);
    displayNeedsUpdate = true;
  }
  else if (message[6] == 0x0f) {  // clear EEPROM
    for (int i=0; i<48; i++) {
      EEPROM.write(i, NO_ASSIGNMENT);
      potEnabled[i] = false;
    }
  }
}



void setup() {

  for (int i=0; i<48; i++) {
    byte fromROM = EEPROM.read(i);
    if (fromROM != NO_ASSIGNMENT) {
      potAssignMap[i] = fromROM;
      potEnabled[i] = true;
    }
  }
  
  DDRB = B1111; // mux address lines as outputs
  initPotValues();
  
  display->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

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

  potIndex = 0;

  for (muxAddress=0; muxAddress<16; muxAddress++) {
    PORTB = muxAddress;

    potReadings[potIndex] = analogRead(A0);
    potValues[potIndex] = analogRead(A0) >> 3;
    potIndex++;

    potReadings[potIndex] = analogRead(A1);
    potValues[potIndex] = analogRead(A1) >> 3;
    potIndex++;

    potReadings[potIndex] = analogRead(A2);
    potValues[potIndex] = analogRead(A2) >> 3;
    potIndex++;
  }
}


int findGreatestValuePot() {
  
  unsigned int greatestPotValue = 0;
  byte greatestpotIndex = 0;
  unsigned int readingValue;
  
  potIndex = 0;

  for (muxAddress=0; muxAddress<16; muxAddress++) {
    PORTB = muxAddress;

    readingValue = analogRead(A0);
    if (readingValue > greatestPotValue) {
      greatestPotValue = readingValue;
      greatestpotIndex = potIndex;
    }
    potIndex++;

    readingValue = analogRead(A1);
    if (readingValue > greatestPotValue) {
      greatestPotValue = readingValue;
      greatestpotIndex = potIndex;
    }
    potIndex++;

    readingValue = analogRead(A2);
    if (readingValue > greatestPotValue) {
      greatestPotValue = readingValue;
      greatestpotIndex = potIndex;
    }
    potIndex++;
  }

  return greatestpotIndex;
}

unsigned long syncTimeout = 0;
byte oldValue, newValue;
unsigned int analogIn;
unsigned int reading;
byte readingFraction;
byte readingWholePart;

#define UPDATE_IF_ENABLED() if (potEnabled[potIndex]) {\
  if (potValues[potIndex] != newValue) {\
    byte paramIndex = potAssignMap[potIndex];\
    pg800.setParam(paramIndex);\
    pg800.setValue(newValue);\
    potValues[potIndex] = newValue;\
    displayNeedsUpdate = true;\
  }\
}


#define PROCESS_INPUT(analogInput) {\
  analogIn = analogRead(analogInput);\
  if (analogIn < 4) {\
    potReadings[potIndex] = 0;\
    newValue = 0;\
    UPDATE_IF_ENABLED()\
  } else {\
    reading = ( (potValues[potIndex] << 4) + potReadings[potIndex] + analogIn ) >> 2;\
    potReadings[potIndex] = analogIn;\
    readingFraction = reading & B111;\
    readingWholePart = reading >> 3;\
    if (readingFraction < 3) {\
      newValue = readingWholePart;\
      UPDATE_IF_ENABLED()\
    }\
    else if (readingFraction < 6) {\
      if (potValues[potIndex] != readingWholePart) {\
        if (potValues[potIndex] != (readingWholePart + 1)) {\
          newValue = readingWholePart;\
          UPDATE_IF_ENABLED()\
        }\
      }\
    }\
    else {\
      newValue = readingWholePart + 1;\
      UPDATE_IF_ENABLED()\
    }\
  }\
}



void loop() {

  potIndex = 0;

  for (muxAddress=0; muxAddress<16; muxAddress++) {
    PORTB = muxAddress;

    PROCESS_INPUT(A0)
    potIndex++;
    
    PROCESS_INPUT(A1)
    potIndex++;
    
    PROCESS_INPUT(A2)
    potIndex++;
  }
  
  MIDI.read();
  
  wheel.checkButton();

  if (changeParamMode && ((millis() - potAssignHoldStartTime) > 3000)) {
    int greatestpotIndex = findGreatestValuePot();
    potAssignMap[greatestpotIndex] = pg800.getParamIndex();
    EEPROM.write(greatestpotIndex, pg800.getParamIndex());
    potAssigned = greatestpotIndex;
    potEnabled[greatestpotIndex] = true;
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


void updateDisplay() {
  
  display->clearDisplay();
  display->setTextColor(WHITE);

  if (potAssigned != NO_ASSIGNMENT) {
    display->setCursor(3,3);
    display->setTextSize(1);
    display->print(potAssigned);
    display->print(" --> ");
    display->print(pg800.paramName());
    display->display();
    potAssigned = NO_ASSIGNMENT;
    delay(2000);
    return;
  }

  display->setCursor(3,3);
  display->setTextSize(1);
  display->print(pg800.paramName());

  if (changeParamMode) {
    display->drawRect(0,0,128,13,WHITE);
  } else {
    display->drawRect(0,13,128,19,WHITE);
  }

  pg800.drawParamValue(display);

  display->display();
}



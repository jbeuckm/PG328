#include <EEPROM.h>
#include <MIDI.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <gfxfont.h>

#include "RotaryEncoder.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

RotaryEncoder wheel;


#define CTRL_RESET 121

#define READY 10
#define CLOCK_IN 8
#define DATA_OUT 9

#define MIDI_CHANNEL_ADDRESS 0

MIDI_CREATE_DEFAULT_INSTANCE();

byte selectedChannel = 3;


void sendByte(byte data) {
  
  digitalWrite(READY, HIGH);
  
  byte mask = B1000000;
  
  for (int i=0; i<8; i++) {
    while (digitalRead(CLOCK_IN) == LOW) {
      delayMicroseconds(1);
    }
    if (data & mask) {
      digitalWrite(DATA_OUT, HIGH);
    } else {
      digitalWrite(DATA_OUT, LOW);
    }
    while (digitalRead(CLOCK_IN) == HIGH) {
      delayMicroseconds(1);
    }
    mask >>= 1;
  }
  
  digitalWrite(READY, LOW);
  
}

void handleSystemExclusive(byte *message, unsigned size) {

  if (message[1] == 0x77) // manufacturer ID
  if (message[2] == 0x10)    // model ID
  if (message[3] == 0) {  // device ID
    selectedChannel = message[4] % 17;
    EEPROM.write(MIDI_CHANNEL_ADDRESS, selectedChannel);
    MIDI.begin(selectedChannel);
    
  }
}


void handleControlChange(byte channel, byte number, byte value)
{
  switch (number) {

    case CTRL_RESET:
      break;
      
  }   
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  wheel.setup();
  attachInterrupt(0, interruptA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, interruptB, RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)

  pinMode(READY, OUTPUT);
  digitalWrite(READY, LOW);

  pinMode(DATA_OUT, OUTPUT);
  digitalWrite(DATA_OUT, LOW);

  pinMode(CLOCK_IN, INPUT);

  selectedChannel = EEPROM.read(MIDI_CHANNEL_ADDRESS);
  if (selectedChannel > 16) {
    selectedChannel = 0;
    EEPROM.write(MIDI_CHANNEL_ADDRESS, selectedChannel);
  }

  MIDI.setHandleSystemExclusive(handleSystemExclusive);
  MIDI.setHandleControlChange(handleControlChange);
  
  MIDI.begin(selectedChannel);
}

void interruptA() {
  wheel.PinA();
}
void interruptB() {
  wheel.PinB();
}

void loop() {
  MIDI.read();
  updateDisplay();
}


void updateDisplay() {
  display.clearDisplay();
  
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
    
  display.print("VCF Freq\n\n");
  display.setTextSize(2);
  display.print(String(wheel.getPosition()));
  
  display.display();
}



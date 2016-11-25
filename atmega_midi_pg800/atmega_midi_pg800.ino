#include <EEPROM.h>
#include <MIDI.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <gfxfont.h>

#include "RotaryEncoder.h"
#include "PG800.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

RotaryEncoder wheel;

PG800 pg800(10, 8, 9);

#define CTRL_RESET 121
#define MIDI_CHANNEL_ADDRESS 0

MIDI_CREATE_DEFAULT_INSTANCE();

byte selectedChannel = 3;


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
  
  display.setTextColor(WHITE);

  display.setCursor(3,3);
  display.setTextSize(1);
  display.print("DCO2 Fine Tune");

  display.drawRect(0,0,128,13,WHITE);
    

  display.setCursor(4,16);
  display.setTextSize(2);
  display.print(String(wheel.getPosition()));

  display.drawRect(0,13,128,19,WHITE);

  display.display();
}



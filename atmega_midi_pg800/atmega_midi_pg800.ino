#include <EEPROM.h>
#include <MIDI.h>

#define CTRL_RESET 121

#define READY 0
#define CLOCK_IN 1
#define DATA_OUT 2

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

void loop() {
  MIDI.read();
}


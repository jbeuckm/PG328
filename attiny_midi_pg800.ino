#include <EEPROM.h>
#include <SoftwareSerialIn.h>
#include <MIDI.h>

#define CTRL_RESET 121

#define READY 0
#define CLOCK_IN 1
#define DATA_OUT 2

#define MIDI_CHANNEL_ADDRESS 0

SoftwareSerialIn mSerial(5);
MIDI_CREATE_INSTANCE(SoftwareSerialIn, mSerial, midiIn);

byte selectedChannel = 3;


void handleSystemExclusive(byte *message, unsigned size) {

  if (message[1] == 0x77) // manufacturer ID
  if (message[2] == 0x10)    // model ID
  if (message[3] == 0) {  // device ID
    selectedChannel = message[4] % 17;
    EEPROM.write(MIDI_CHANNEL_ADDRESS, selectedChannel);
    midiIn.begin(selectedChannel);
    
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

  midiIn.setHandleSystemExclusive(handleSystemExclusive);
  midiIn.setHandleControlChange(handleControlChange);
  
  midiIn.begin(selectedChannel);
}

void loop() {
  midiIn.read();  
}


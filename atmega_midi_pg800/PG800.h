#include "Arduino.h"
#include "CompactBoolArray.h"
#include "CircularByteBuffer.h"

#define PG800_SYSEX_OFFSET 0x0b
#define PG800_PARAM_OFFSET 0x80

extern volatile byte paramValues[];
extern volatile byte paramIndex;

class PG800 {

  CompactBoolArray paramChanged;
  CircularByteBuffer outBuffer;

  byte sendQueue[10];
  byte sendQueueLength = 0;

  byte READY_PIN;
  byte CLOCK_IN_PIN;
  byte DATA_OUT_PIN;


public:
  PG800(int ready_pin, int clock_in_pin, int data_out_pin);

  void sendByte(byte data);

  char *paramName();

  byte paramValue();
  void nextParam();
  void prevParam();
  void setParam(byte param);
  int getParamIndex();

  void incValue();
  void decValue();
  void setValue(byte value);

  void queueByte(byte newByte);
  void sync();
};


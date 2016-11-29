#include "PG800.h"
#include <avr/pgmspace.h>

const char s0[] PROGMEM = "DCO1 Range";
const char s1[] PROGMEM = "DCO1 Wave";
const char s2[] PROGMEM = "DCO1 Tune";
const char s3[] PROGMEM = "DCO1 LFO";
const char s4[] PROGMEM = "DCO1 Env";
const char s5[] PROGMEM = "DCO2 Range";
const char s6[] PROGMEM = "DCO2 Wave";
const char s7[] PROGMEM = "XMOD";
const char s8[] PROGMEM = "DCO2 Tune";
const char s9[] PROGMEM = "DCO2 Fine Tune";
const char s10[] PROGMEM = "DCO2 LFO";
const char s11[] PROGMEM = "DCO2 Env";
const char s12[] PROGMEM = "";
const char s13[] PROGMEM = "";
const char s14[] PROGMEM = "";
const char s15[] PROGMEM = "DCO Dynamics";
const char s16[] PROGMEM = "DCO Env Mode";
const char s17[] PROGMEM = "DCO1 Level";
const char s18[] PROGMEM = "DCO2 Level";
const char s19[] PROGMEM = "Mix Env";
const char s20[] PROGMEM = "Mix Dynamics";
const char s21[] PROGMEM = "Mix Env Mode";
const char s22[] PROGMEM = "VCF HPF";
const char s23[] PROGMEM = "VCF Freq";
const char s24[] PROGMEM = "VCF Resonance";
const char s25[] PROGMEM = "VCF LFO";
const char s26[] PROGMEM = "VCF Env";
const char s27[] PROGMEM = "VCF Key";
const char s28[] PROGMEM = "VCF Dynamics";
const char s29[] PROGMEM = "VCF Env Mode";
const char s30[] PROGMEM = "VCA Level";
const char s31[] PROGMEM = "VCA Dynamics";
const char s32[] PROGMEM = "Chorus";
const char s33[] PROGMEM = "LFO Wave";
const char s34[] PROGMEM = "LFO Delay";
const char s35[] PROGMEM = "LFO Rate";
const char s36[] PROGMEM = "Env1 Attack";
const char s37[] PROGMEM = "EG1 Decay";
const char s38[] PROGMEM = "EG1 Sustain";
const char s39[] PROGMEM = "EG1 Release";
const char s40[] PROGMEM = "EG1 Key";
const char s41[] PROGMEM = "EG2 Attack";
const char s42[] PROGMEM = "EG2 Decay";
const char s43[] PROGMEM = "EG2 Sustain";
const char s44[] PROGMEM = "EG2 Release";
const char s45[] PROGMEM = "EG2 Key";
const char s46[] PROGMEM = "";
const char s47[] PROGMEM = "VCA Env Mode";

const char* const string_table[] PROGMEM = {
  s0, s1, s2, s3, s4, s5, s6, s7, s8, s9,
  s10, s11, s12, s13, s14, s15, s16, s17, s18, s19,
  s20, s21, s22, s23, s24, s25, s26, s27, s28, s29,
  s30, s31, s32, s33, s34, s35, s36, s37, s38, s39,
  s40, s41, s42, s43, s44, s45, s46, s47
};

byte param_values[48] = {
  63,63,63,63,63,63,63,63,
  63,63,63,63,63,63,63,63,
  63,63,63,63,63,63,63,63,
  63,63,63,63,63,63,63,63,
  63,63,63,63,63,63,63,63,
  63,63,63,63,63,63,63,63
};


#define CONTROL_BYTE_OFFSET 0x80

char buffer[20];

char *PG800::paramName() {
  strcpy_P(buffer, (char*)pgm_read_word(&(string_table[paramIndex])));
  return buffer;
}
byte PG800::paramValue() {
  return param_values[paramIndex];
}

void PG800::nextParam() {
  if (paramIndex == 47) return;
  do {
    paramIndex++;
  } while (strcmp(paramName(), "") == 0);
}
void PG800::prevParam() {
  if (paramIndex == 0) return;
  do {
    paramIndex--;
  } while (strcmp(paramName(), "") == 0);
}
void PG800::setParam(byte param) {
  if (param < 0) return;
  if (param >= 48) return;
  paramIndex = param;
}

void PG800::incValue() {
  if (param_values[paramIndex] == 127) return;
  param_values[paramIndex]++;

  sendByte(CONTROL_BYTE_OFFSET + paramIndex);
  sendByte(param_values[paramIndex] % 128);
}
void PG800::decValue() {
  if (param_values[paramIndex] == 0) return;
  param_values[paramIndex]--;

  sendByte(CONTROL_BYTE_OFFSET + paramIndex);
  sendByte(param_values[paramIndex] % 128);
}
void PG800::setValue(byte value) {
  if (value < 0) return;
  if (value >= 128) return;
  param_values[paramIndex] = value;

  sendByte(CONTROL_BYTE_OFFSET + paramIndex);
  sendByte(param_values[paramIndex] % 128);
}



PG800::PG800(int ready_pin, int clock_in_pin, int data_out_pin) {
  READY_PIN = ready_pin;
  CLOCK_IN_PIN = clock_in_pin;
  DATA_OUT_PIN = data_out_pin;

  pinMode(READY_PIN, OUTPUT);
  digitalWrite(READY_PIN, LOW);

  pinMode(DATA_OUT_PIN, OUTPUT);
  digitalWrite(DATA_OUT_PIN, LOW);

  pinMode(CLOCK_IN_PIN, INPUT);

  paramIndex = 0;
}


void PG800::sendByte(byte data) {
  
  byte mask = B10000000;
  
  digitalWrite(READY_PIN, HIGH);
  
  for (int i=0; i<8; i++) {
    while (digitalRead(CLOCK_IN_PIN) == LOW) {
      delayMicroseconds(1);
    }
    if (data & mask) {
      digitalWrite(DATA_OUT_PIN, LOW);
    } else {
      digitalWrite(DATA_OUT_PIN, HIGH);
    }
    while (digitalRead(CLOCK_IN_PIN) == HIGH) {
      delayMicroseconds(1);
    }
    mask >>= 1;
  }
  
  digitalWrite(READY_PIN, LOW);
}


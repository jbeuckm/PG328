#include "PG800.h"

char *param_names[] = {
"DCO1 Rng",
"DCO1 Wav",
"DCO1 Tune",
"DCO1 LFO",
"DCO1 Env",
"DCO2 Rng",
"DCO2 Wav",
"XMOD",
"DCO2 Tune",
"DCO2 Fine",
"DCO2 LFO",
"DCO2 Env",
"",
"",
"",
"DCO Dyn",
"DCO Env Mode",
"DCO1 Lev",
"DCO2 Lev",
"Mix Env",
"Mix Dyn",
"Mix Env Mode",
"VCF HPF",
"VCF F",
"VCF Q",
"VCF LFO",
"VCF Env",
"VCF Key",
"VCF Dyn",
"VCF Env Mode",
"VCA Lev",
"VCA Dyn",
"Chorus",
"LFO Wav",
"LFO Del",
"LFO Rate",
"EG1 Att",
"EG1 Dec",
"EG1 Sus",
"EG1 Rel",
"EG1 Key",
"EG2 Att",
"EG2 Dec",
"EG2 Sus",
"EG2 Rel",
"EG2 Key",
"",
"VCA Env Mode"
};

int param_values[48] = {
  63,63,63,63,63,63,63,63,
  63,63,63,63,63,63,63,63,
  63,63,63,63,63,63,63,63,
  63,63,63,63,63,63,63,63,
  63,63,63,63,63,63,63,63,
  63,63,63,63,63,63,63,63
};

char *PG800::controlName() {
  return param_names[controlIndex];
}
int PG800::controlValue() {
  return param_values[controlIndex];
}

void PG800::nextControl() {
  if (controlIndex == 47) return;
  do {
    controlIndex++;
  } while (strcmp(param_names[controlIndex], "") == 0);
}
void PG800::prevControl() {
  if (controlIndex == 0) return;
  do {
    controlIndex--;
  } while (strcmp(param_names[controlIndex], "") == 0);
}

void PG800::incValue() {
  if (param_values[controlIndex] == 127) return;
  param_values[controlIndex]++;
}
void PG800::decValue() {
  if (param_values[controlIndex] == 0) return;
  param_values[controlIndex]--;
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

  controlIndex = 0;
}


void PG800::sendByte(byte data) {
  
  digitalWrite(READY_PIN, HIGH);
  
  byte mask = B1000000;
  
  for (int i=0; i<8; i++) {
    while (digitalRead(CLOCK_IN_PIN) == LOW) {
      delayMicroseconds(1);
    }
    if (data & mask) {
      digitalWrite(DATA_OUT_PIN, HIGH);
    } else {
      digitalWrite(DATA_OUT_PIN, LOW);
    }
    while (digitalRead(CLOCK_IN_PIN) == HIGH) {
      delayMicroseconds(1);
    }
    mask >>= 1;
  }
  
  digitalWrite(READY_PIN, LOW);
}


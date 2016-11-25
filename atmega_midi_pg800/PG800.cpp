#include "PG800.h"

PG800::PG800(int ready_pin, int clock_in_pin, int data_out_pin) {
  READY_PIN = ready_pin;
  CLOCK_IN_PIN = clock_in_pin;
  DATA_OUT_PIN = data_out_pin;

  pinMode(READY_PIN, OUTPUT);
  digitalWrite(READY_PIN, LOW);

  pinMode(DATA_OUT_PIN, OUTPUT);
  digitalWrite(DATA_OUT_PIN, LOW);

  pinMode(CLOCK_IN_PIN, INPUT);
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


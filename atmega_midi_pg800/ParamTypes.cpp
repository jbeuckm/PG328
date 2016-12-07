#include "ParamTypes.h"
#include "PG800.h"

void inc_value_numeric() {
  if (paramValues[paramIndex] == 127) return;
  paramValues[paramIndex]++;  
}
void dec_value_numeric() {
  if (paramValues[paramIndex] == 0) return;
  paramValues[paramIndex]--;
}
void set_value_numeric(unsigned char value) {
  if (value < 0) return;
  if (value >= 128) return;
  paramValues[paramIndex] = value;  
}
void draw_value_numeric(Adafruit_SSD1306 *display) {
  display->setCursor(4,16);
  display->setTextSize(2);
  display->print(String(paramValues[paramIndex]));  
}


void inc_value_4val() {
  if (paramValues[paramIndex] < 32) {
    paramValues[paramIndex] = 32;
  }
  else if (paramValues[paramIndex] < 64) {
    paramValues[paramIndex] = 64;
  }
  else if (paramValues[paramIndex] < 96) {
    paramValues[paramIndex] = 96;
  }
}
void dec_value_4val() {
  if (paramValues[paramIndex] >= 96) {
    paramValues[paramIndex] = 95;
  }
  else if (paramValues[paramIndex] >= 64) {
    paramValues[paramIndex] = 63;
  }
  else if (paramValues[paramIndex] >= 32) {
    paramValues[paramIndex] = 31;
  }
}
void draw_value_4val(Adafruit_SSD1306 *display) {
  display->setCursor(4,16);
  display->setTextSize(2);
  if (paramValues[paramIndex] < 32) {
    display->print(String("Off"));  
  }
  else if (paramValues[paramIndex] < 64) {
    display->print(String("1"));  
  }
  else if (paramValues[paramIndex] < 96) {
    display->print(String("2"));  
  }
  else {
    display->print(String("3"));  
  }
}


void draw_value_wave(Adafruit_SSD1306 *display) {
  display->setCursor(4,16);
  display->setTextSize(2);
  if (paramValues[paramIndex] < 32) {
    display->print(String("Noise"));  
  }
  else if (paramValues[paramIndex] < 64) {
    display->print(String("Square"));  
  }
  else if (paramValues[paramIndex] < 96) {
    display->print(String("Pulse"));  
  }
  else {
    display->print(String("Sawtooth"));  
  }
}


void draw_value_range(Adafruit_SSD1306 *display) {
  display->setCursor(4,16);
  display->setTextSize(2);
  if (paramValues[paramIndex] < 32) {
    display->print(String("16'"));  
  }
  else if (paramValues[paramIndex] < 64) {
    display->print(String("8'"));  
  }
  else if (paramValues[paramIndex] < 96) {
    display->print(String("4'"));  
  }
  else {
    display->print(String("2'"));  
  }
}




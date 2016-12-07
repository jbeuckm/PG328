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
  if (paramValues[paramIndex] == 127) return;
  paramValues[paramIndex]++;  
}
void dec_value_4val() {
  if (paramValues[paramIndex] == 0) return;
  paramValues[paramIndex]--;
}
void set_value_4val(unsigned char value) {
  if (value < 0) return;
  if (value >= 128) return;
  paramValues[paramIndex] = value;  
}
void draw_value_4val(Adafruit_SSD1306 *display) {
  display->setCursor(4,16);
  display->setTextSize(2);
  display->print(String(paramValues[paramIndex]));  
}




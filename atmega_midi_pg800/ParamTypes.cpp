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

char value_string_buffer[10];

void draw_value_string_from_table(Adafruit_SSD1306 *display, const char * const*stringTable) {
  
  byte index = paramValues[paramIndex] / 32;

  strcpy_P(value_string_buffer, (char*)pgm_read_word(&(stringTable[index])));

  display->setCursor(4,16);
  display->setTextSize(2);
  display->print(String(value_string_buffer));  
}


const char _4val_string_0[] PROGMEM = "Off";
const char _4val_string_1[] PROGMEM = "1";
const char _4val_string_2[] PROGMEM = "2";
const char _4val_string_3[] PROGMEM = "3";
const char* const _4val_string_table[] PROGMEM = {
  _4val_string_0, _4val_string_1, _4val_string_2, _4val_string_3
};

void draw_value_4val(Adafruit_SSD1306 *display) {
  draw_value_string_from_table(display, _4val_string_table);
}


const char wave_string_0[] PROGMEM = "Noise";
const char wave_string_1[] PROGMEM = "Square";
const char wave_string_2[] PROGMEM = "Pulse";
const char wave_string_3[] PROGMEM = "Sawtooth";
const char* const wave_string_table[] PROGMEM = {
  wave_string_0, wave_string_1, wave_string_2, wave_string_3
};
void draw_value_wave(Adafruit_SSD1306 *display) {
  draw_value_string_from_table(display, wave_string_table);
}


const char range_string_0[] PROGMEM = "16'";
const char range_string_1[] PROGMEM = "8'";
const char range_string_2[] PROGMEM = "4'";
const char range_string_3[] PROGMEM = "2'";
const char* const range_string_table[] PROGMEM = {
  range_string_0, range_string_1, range_string_2, range_string_3
};
void draw_value_range(Adafruit_SSD1306 *display) {
  draw_value_string_from_table(display, range_string_table);
}

const char xmod_string_0[] PROGMEM = "Off";
const char xmod_string_1[] PROGMEM = "Sync 1";
const char xmod_string_2[] PROGMEM = "Sync 2";
const char xmod_string_3[] PROGMEM = "XMOD";
const char* const xmod_string_table[] PROGMEM = {
  xmod_string_0, xmod_string_1, xmod_string_2, xmod_string_3
};

void draw_value_xmod(Adafruit_SSD1306 *display) {
  draw_value_string_from_table(display, xmod_string_table);
}



const char eg_mode_string_0[] PROGMEM = "-EG2";
const char eg_mode_string_1[] PROGMEM = "EG2";
const char eg_mode_string_2[] PROGMEM = "-EG1";
const char eg_mode_string_3[] PROGMEM = "EG1";
const char* const eg_mode_string_table[] PROGMEM = {
  eg_mode_string_0, eg_mode_string_1, eg_mode_string_2, eg_mode_string_3
};
void draw_value_eg_mode(Adafruit_SSD1306 *display) {

  draw_value_string_from_table(display, eg_mode_string_table);

  if ((paramValues[paramIndex] / 32) & B00000001) {
    byte baseX = 57, baseY = 29;
      
    display->drawLine(baseX+0, baseY+0, baseX+14, baseY-14, WHITE);
    display->drawLine(baseX+14, baseY-14, baseX+21, baseY-7, WHITE);
    display->drawLine(baseX+21, baseY-7, baseX+51, baseY-7, WHITE);
    display->drawLine(baseX+51, baseY-7, baseX+58, baseY+0, WHITE);
  } else {
    byte baseX = 57, baseY = 15;
      
    display->drawLine(baseX+0, baseY+0, baseX+14, baseY+14, WHITE);
    display->drawLine(baseX+14, baseY+14, baseX+21, baseY+7, WHITE);
    display->drawLine(baseX+21, baseY+7, baseX+51, baseY+7, WHITE);
    display->drawLine(baseX+51, baseY+7, baseX+58, baseY+0, WHITE);
  }
}


void draw_value_tune(Adafruit_SSD1306 *display) {
  int mappedValue = map(paramValues[paramIndex], 0, 127, -12, 12);
  display->setCursor(4,16);
  display->setTextSize(2);
  display->print(mappedValue);  
}
void draw_value_fine(Adafruit_SSD1306 *display) {
  int mappedValue = map(paramValues[paramIndex], 0, 127, -50, 50);
  display->setCursor(4,16);
  display->setTextSize(2);
  display->print(mappedValue);  
}


const char lfo_wave_string_0[] PROGMEM = "Noise";
const char lfo_wave_string_1[] PROGMEM = "Square";
const char lfo_wave_string_2[] PROGMEM = "Sine";
const char lfo_wave_string_3[] PROGMEM = "Sine";
const char* const lfo_wave_string_table[] PROGMEM = {
  lfo_wave_string_0, lfo_wave_string_1, lfo_wave_string_2, lfo_wave_string_3
};

void draw_value_lfo_wave(Adafruit_SSD1306 *display) {
  draw_value_string_from_table(display, lfo_wave_string_table);
}



const char vca_eg_mode_string_0[] PROGMEM = "Gate";
const char vca_eg_mode_string_1[] PROGMEM = "Gate";
const char vca_eg_mode_string_2[] PROGMEM = "EG2";
const char vca_eg_mode_string_3[] PROGMEM = "EG2";
const char* const vca_eg_mode_string_table[] PROGMEM = {
  vca_eg_mode_string_0, vca_eg_mode_string_1, vca_eg_mode_string_2, vca_eg_mode_string_3
};

void draw_value_vca_eg_mode(Adafruit_SSD1306 *display) {
  draw_value_string_from_table(display, vca_eg_mode_string_table);
}



const char chorus_string_0[] PROGMEM = "Off";
const char chorus_string_1[] PROGMEM = "Chorus 1";
const char chorus_string_2[] PROGMEM = "Chorus 2";
const char chorus_string_3[] PROGMEM = "Chorus 2";
const char* const chorus_string_table[] PROGMEM = {
  chorus_string_0, chorus_string_1, chorus_string_2, chorus_string_3
};

void draw_value_chorus(Adafruit_SSD1306 *display) {
  draw_value_string_from_table(display, chorus_string_table);
}




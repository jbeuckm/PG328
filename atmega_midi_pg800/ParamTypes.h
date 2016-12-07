#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

void inc_value_numeric();
void dec_value_numeric();
void set_value_numeric(unsigned char value);
void draw_value_numeric(Adafruit_SSD1306 *display);

void draw_value_string_from_table(Adafruit_SSD1306 *display, const char * const*stringTable);

void inc_value_4val();
void dec_value_4val();
void draw_value_4val(Adafruit_SSD1306 *display);

void draw_value_wave(Adafruit_SSD1306 *display);

void draw_value_range(Adafruit_SSD1306 *display);

void draw_value_xmod(Adafruit_SSD1306 *display);

void draw_value_eg_mode(Adafruit_SSD1306 *display);

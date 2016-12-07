#include "ParamTypes.h"
#include "PG800.h"

void inc_value_numeric() {
  if (paramValues[paramIndex] == 127) return;
  paramValues[paramIndex]++;  
}
void dec_value_numeric();
void set_value_numeric(unsigned char value);
void draw_value_numeric(void *display);



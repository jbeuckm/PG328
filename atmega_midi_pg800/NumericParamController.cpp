#include "NumericParamController.h"

NumericParamController::NumericParamController(unsigned char _value) : ParamController(_value) {
}

void NumericParamController::incValue(){
  if (value == 127) return;
  value++;
}
void NumericParamController::decValue(){
  if (value == 0) return;
  value--;
}
void NumericParamController::setValue(unsigned char _value){
  if (_value < 0) return;
  if (_value >= 128) return;
  value = _value;
}

void NumericParamController::drawValue(void *display) {
}


#include "ParamController.h"

ParamController::ParamController(unsigned char _value) {
  value = _value;
}

unsigned char ParamController::getValue() {
  return value;
}


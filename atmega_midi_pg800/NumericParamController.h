#include "ParamController.h"

class NumericParamController : public ParamController {
  
  public:
    NumericParamController(unsigned char _value);
    
    virtual void incValue();
    virtual void decValue();
    virtual void setValue(unsigned char _value);
    
    virtual void drawValue(void *display);
  
};

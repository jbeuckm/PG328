#ifndef __ParamController__
#define __ParamController__

class ParamController {

  protected: 
    unsigned char value;

  public:
    ParamController(unsigned char _value);
    unsigned char getValue();
  
    virtual void incValue() = 0;
    virtual void decValue() = 0;
    virtual void setValue(unsigned char _value) = 0;
    
    virtual void drawValue(void *display) = 0;
};

#endif


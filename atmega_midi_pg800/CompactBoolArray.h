#ifndef __CompactBoolArray__
#define __CompactBoolArray__

class CompactBoolArray {
  private:
  
  volatile unsigned char *data;
  volatile unsigned int __size;
  
  public: 
    CompactBoolArray(unsigned int _size);
    void setBit(unsigned int address, bool value);
    bool getBit(unsigned int address);
    
    int addressOfFirstSet();
};

#endif


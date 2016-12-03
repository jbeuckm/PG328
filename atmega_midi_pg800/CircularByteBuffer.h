#ifndef __CircularByteBuffer__
#define __CircularByteBuffer__

class CircularByteBuffer {
  private:
  
  char *data;
  unsigned int size;
  unsigned int position;
  unsigned int length;
  
  public: 
    CircularByteBuffer(unsigned int _size);
    void push(char newByte);
    unsigned int getLength();
    char shift();
};

#endif


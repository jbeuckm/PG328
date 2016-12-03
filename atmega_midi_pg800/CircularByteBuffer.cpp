#include "CircularByteBuffer.h"
#include <stdlib.h>

CircularByteBuffer::CircularByteBuffer(unsigned int _size) {
  
  size = _size;
  data = (char *)calloc(size, 1);
  position = 0;
  length = 0;
}


void CircularByteBuffer::push(char newByte) {
  unsigned int newPosition = (position + length) % size;
  data[newPosition] = newByte;
  length++;
}

unsigned int CircularByteBuffer::getLength() {
  return length;
}



char CircularByteBuffer::shift() {
  if (length == 0) return 0;
  
  char outByte = data[position];
  position = (position + 1) % size;
  length--;
  
  return outByte;
}
    

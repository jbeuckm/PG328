#include "CompactBoolArray.h"
#include <stdlib.h>


CompactBoolArray::CompactBoolArray(unsigned int _size) {
  __size = _size;
  
  int byteCount = _size >> 3;
  if (_size & 0x7) {
    byteCount++;
  }
  
  data = (unsigned char *)calloc(byteCount, 1);
}


void CompactBoolArray::setBit(unsigned int address, bool value) {
  
  unsigned char mask = 1 << (address & 0x7);
  
  if (value) {
    data[address >> 3] |= mask;  
  } else {
    data[address >> 3] &= ~mask;  
  }
}


bool CompactBoolArray::getBit(unsigned int address) {

  unsigned char mask = 1 << (address & 0x7);
  
  return data[address >> 3] & mask; 
}


/*
 * Returns -1 if all zero
 */
int CompactBoolArray::addressOfFirstSet() {
  unsigned char compareByte;
  for (unsigned int i=0; i<__size; i++) {
    compareByte = data[i];
    if (compareByte) {    // found a byte with a 1 in it
      if (compareByte & 0x01) return (i << 3);
      if (compareByte & 0x02) return (i << 3) + 1;
      if (compareByte & 0x04) return (i << 3) + 2;
      if (compareByte & 0x08) return (i << 3) + 3;
      if (compareByte & 0x10) return (i << 3) + 4;
      if (compareByte & 0x20) return (i << 3) + 5;
      if (compareByte & 0x40) return (i << 3) + 6;
      if (compareByte & 0x80) return (i << 3) + 7;
      // error - byte true but no bits :(
    }
  }
  return -1;
}


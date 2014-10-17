#include "tokenizer.h"
#include <math.h>

UNIT toStandardSize(SIZE_TYPE type, UNIT num) {
  switch (type) {
  case TB:
    return num << 30;
  case GB:
    return num << 20;
  case MB:
    return num << 10;
  case KB:
    return num;
  default:
    return -1;
  }
}

int sizeToLevel(UNIT size) {
  return ceil(log2(size)) + 1;
}

UNIT nextPower2(UNIT size) {
  return pow(2, (ceil(log2(size))));  
}

bool isSlabSize(UNIT size, UNIT slabSize) {
  return (size > slabSize / 2) && (size <= slabSize);
}

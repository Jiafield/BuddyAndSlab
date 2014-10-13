#include "tokenizer.h"

UNIT sizeToLevel(SIZE_TYPE type, UNIT num, UNIT total) {
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
    return 0;
  }
}

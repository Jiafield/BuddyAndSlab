#include "tokenizer.h"
#include <string>
#include <iostream>
#include <boost/regex.hpp>
#include <math.h>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;

UNIT getMemorySize(string str) {
  // get rid of the whitespaces
  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
  // Parse the string
  boost::regex expr("memorySize\\(([0-9]*)([a-zA-Z]*)\\);");
  boost::cmatch results;
  if (boost::regex_match(str.c_str(), results, expr)) {
    cout << results[1].str() << endl;
    cout << results[2].str() << endl;
  }
}

UNIT getSlabInfo(int *portion, string str) {
  // get rid of the whitespaces
  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
  // Parse the string
  boost::regex expr("slabSize\\(([0-9]*)([a-zA-Z]*),1/([0-9]*)\\);");
  boost::cmatch results;
  if (boost::regex_match(str.c_str(), results, expr)) {
    cout << results[1].str() << endl;
    cout << results[2].str() << endl;
    cout << results[3].str() << endl;
  }
  
}

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

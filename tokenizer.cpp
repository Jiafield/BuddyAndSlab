#include "tokenizer.h"
#include <string>
#include <iostream>
#include <stdlib.h>
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
    return toStandardSize(results[2].str(), atoi(results[1].str().c_str()));
  }
  return 0;
}

UNIT getSlabInfo(int *portion, string str) {
  // get rid of the whitespaces
  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
  // Parse the string
  boost::regex expr("slabSize\\(([0-9]*)([a-zA-Z]*),1/([0-9]*)\\);");
  boost::cmatch results;
  if (boost::regex_match(str.c_str(), results, expr)) {
    *portion = atoi(results[3].str().c_str());
    return toStandardSize(results[2].str(), atoi(results[1].str().c_str()));
  }
  return 0;
}

UNIT toStandardSize(string type, UNIT num) {
  if (type.compare("TB") == 0)
    return num << 30;
  else if (type.compare("GB") == 0)
    return num << 20;
  else if (type.compare("MB") == 0)
    return num << 10;
  else if (type.compare("KB") == 0)
    return num;
  else
    return 0;
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

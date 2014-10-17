#ifndef tokenizer
#define tokenizer

#include <string>

//typedef enum {TB, GB, MB, KB, OTHER} SIZE_TYPE;
typedef unsigned long long int UNIT;

using std::string;
UNIT getMemorySize(string str);
UNIT getSlabInfo(int *portion, string str);
UNIT toStandardSize(string type, UNIT num);
int sizeToLevel(UNIT size);
UNIT nextPower2(UNIT size);
bool isSlabSize(UNIT size, UNIT slabSize);

#endif

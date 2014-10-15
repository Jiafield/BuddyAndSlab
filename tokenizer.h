#ifndef tokenizer
#define tokenizer

typedef enum {TB, GB, MB, KB, OTHER} SIZE_TYPE;
typedef unsigned long long int UNIT;

UNIT toStandardSize(SIZE_TYPE type, UNIT num);
int sizeToLevel(UNIT size);
UNIT nextPower2(UNIT size);
bool isSlabSize(UNIT size, UNIT slabSize);

#endif

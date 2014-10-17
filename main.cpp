#include "treeObject.h"
#include "tokenizer.h"
#include <map>
#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>

using std::map;
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::string;

int main() {
  // Read input
  vector<string> data;
  string line;
  while (getline(cin, line)) {
    data.push_back(line);
  }
  
  // Parse first 2 lines to get memory description
  int portion;
  UNIT total = getMemorySize(data[0]);
  if (!total) {
    cout << "Read total memory size error!" << endl;
    exit(EXIT_FAILURE);
  }
  UNIT slabSize = getSlabInfo(&portion, data[1]);
  if (!slabSize) {
    cout << "Read slab size error!" << endl;
    exit(EXIT_FAILURE);
  }

  // Build memory manager
  MemoryManager memManager(total, portion, slabSize);
  memManager.dump();
  return 0;
}

#include "treeObject.h"
#include "tokenizer.h"
#include <map>
#include <vector>
#include <iostream>
#include <math.h>
#include <string>

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
  MemoryManager memManager(1<<10, 8, 4);
  
  return 0;
}

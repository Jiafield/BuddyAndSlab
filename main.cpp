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

void execute(string line, MemoryManager &memManager) {
  string funcName = parseFunction(line);
  if (funcName.compare("alloc") == 0) {
    int pid;
    UNIT size = getAllocParam(line, &pid);
    if (!size) {
      cout << "Parse parameters error: " << line << endl;
    } else {
      if (!memManager.alloc(pid, size)) {
	cout << "No space for process " << pid << endl; 
      }
    }      
  } else if (funcName.compare("realloc") == 0) {
    int pid;
    UNIT size = getReallocParam(line, &pid);
    if (!size) {
      cout << "Parse parameters error: " << line << endl;
    } else {
      if (!memManager.realloc(pid, size)) {
	cout << "No space for process " << pid << endl; 
      }
    }
  } else if (funcName.compare("free") == 0) {
    int pid = getFreeParam(line);
    if (pid == -1) {
      cout << "Parse parameters error: " << line << endl;
    } else {
      if (!memManager.free(pid)) {
	cout << "Free: couldn't find process " << pid << endl;
      }
    }
  } else if (funcName.compare("dump") == 0) {
    memManager.dump();
  } else {
    cout << "Can not parse command: " << line << endl;
  }
}

int main() {
  // Read input
  vector<string> data;
  string line;
  while (getline(cin, line)) {
    data.push_back(line);
  }
  
  // Parse first 2 lines to get memory and slab description
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

  // Initialize memory manager
  MemoryManager memManager(total, portion, slabSize);

  // Execute the instructions
  for (size_t i = 2; i < data.size(); i++) {
    execute(data[i], memManager);
  }
  return 0;
}

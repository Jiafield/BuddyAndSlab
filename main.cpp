#include "treeObject.h"
#include "tokenizer.h"
#include <map>
#include <vector>
#include <iostream>
#include <math.h>

using std::map;
using std::vector;
using std::cout;
using std::endl;

int main() {
  MemoryManager memManager(4<<20, 8, 512);
  /*
  memManager.alloc(365, 500);
  memManager.alloc(1758, 1<<20);
  memManager.alloc(3, 300);
  memManager.realloc(5, 600);
  memManager.realloc(3, 400);
  memManager.dump();
  memManager.realloc(3, 1<<20);
  memManager.alloc(4, 1<<20);
  memManager.alloc(5, 1<<19);
  memManager.alloc(6, 1<<10);
  memManager.dump();
  memManager.free(3);
  memManager.free(4);
  memManager.free(1758);
  memManager.free(365);
  memManager.dump();
  */
  for (int i = 0; i < 1024; i++)
    memManager.alloc(i, 512);
  memManager.alloc(1025, 500);
  memManager.dump();
  
  return 0;
}

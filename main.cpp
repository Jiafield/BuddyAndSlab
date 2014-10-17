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
  MemoryManager memManager(1<<10, 8, 4);
  
  //for (int i = 0; i < 32; i++)


  //  memManager.alloc(365, 500);
  //  memManager.alloc(3, 300);
  //memManager.realloc(5, 600);
  //memManager.realloc(3, 400);
  //memManager.dump();
  memManager.alloc(1026, 1<<7);
  memManager.alloc(1027, 1<<7);
  memManager.alloc(1028, 1<<7);
  memManager.free(1027);
  memManager.free(1028);
  memManager.dump();
  memManager.alloc(1029, 1<<7);
  memManager.dump();

  memManager.alloc(1030, 1<<7);
  memManager.dump();
  memManager.alloc(1031, 1<<7);
  memManager.alloc(1032, 1<<7);
  memManager.free(1026);
  //memManager.free(1028);
  //  memManager.free(1030);
  //  memManager.dump();
  memManager.alloc(1025, 1<<8);
  //memManager.dump();
  //  memManager.dump();
  //memManager.free(3);
  //memManager.free(4);
  //memManager.free(1758);
  //memManager.free(365);
  //memManager.dump();
  // memManager.dump();
  
  return 0;
}

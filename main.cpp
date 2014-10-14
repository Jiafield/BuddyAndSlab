#include "treeObject.h"
#include "tokenizer.h"
#include <map>
#include <vector>
#include <iostream>

using std::map;
using std::vector;
using std::cout;
using std::endl;

Node *initSystem(int portion, UNIT slabsize) {
  Node *root = new Node(BUDDY);

  return root;  
}

bool freeNode(map<int, Node *> &memLocation, int pid) {
  if (memLocation.find(pid) == memLocation.end()) {
    cout << "couldn't find process " << pid << endl;
    return false;
  }
  Node *n = memLocation[pid];
  vector<Node *> toBeDeleted;
  n->free(toBeDeleted, n->getLevel());
  
  return true;
}

int main() {
  Node *root = initSystem(8, 512);
  //cout << "Successfully initialized" << endl;
  vector<int> stack;
  map<int, Node *> memLocation;

  // root->alloc(1, 0, memLocation);
  root->alloc(365, 13, memLocation); 
  root->alloc(1758, 2, memLocation); 
  root->alloc(3, 13, memLocation);
  root->alloc(1, 13, memLocation);
  root->alloc(2, 14, memLocation);
  root->printTree(stack);
  freeNode(memLocation, 5);
  freeNode(memLocation, 365);
  root->printTree(stack);
  freeNode(memLocation, 3);
  root->printTree(stack);
  freeNode(memLocation, 2);
  root->printTree(stack);
  freeNode(memLocation, 1);
  root->printTree(stack);
  freeNode(memLocation, 1758);
  root->printTree(stack);

  return 0;
}
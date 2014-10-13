#include <iostream>
#include <vector>
#include "treeObject.h"
#include "tokenizer.h"

using std::cout;
using std::endl;
using std::vector;

// Constructor only use to initialize root
Node::Node(MemType t) {
  parent = NULL;
  left = NULL;
  right = NULL;
  type = t;
  pid = -1;
  level = 0;
  status = FREE;
  subtreeStatus[0] = 1;
}

// Contructor for split nodes
Node::Node(MemType t, int l) {
  parent = NULL;
  left = NULL;
  right = NULL;
  type = t;
  pid = -1;
  level = l;
  status = FREE;
}

Node::Node(int pid, int level, MemType t) {
  parent = NULL;
  left = NULL;
  right = NULL;
  type = t;
  this->pid = pid;
  this->level = level;
  status = ALLOCATED;
}


bool Node::isFree() {return status == FREE;}

int Node::getPID() {return pid;}

int Node::getLevel() {return level;}

MemType Node::getType() {return type;}


void Node::printTree(vector<int> &stack) {
  // Base case, print leaf node
  if (!left && !right) {
    for (vector<int>::iterator it = stack.begin(); it != stack.end(); it++)
      cout << *it;
    if (this->isFree())
      cout << " free" << endl;
    else
      cout << " " << this->getPID() << endl;
    return;
  }
  // print left subtree
  stack.push_back(0);
  (this->left)->printTree(stack);
  stack.pop_back();
  // print right subtree
  stack.push_back(1);
  (this->right)->printTree(stack);
  stack.pop_back();
}

Node::~Node() {}

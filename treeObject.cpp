#include <iostream>
#include <vector>
#include "treeObject.h"
#include "tokenizer.h"

using std::cout;
using std::endl;
using std::vector;

// Constructor only use to initialize root
Node::Node(MemType t) : subtreeStatus(MAX_LEVEL, 0){
  parent = NULL;
  left = NULL;
  right = NULL;
  type = t;
  pid = -1;
  level = 0;
  status = FREE;
  incrementStatus(level);
}

// Contructor for split nodes
Node::Node(Node *p, MemType t, int l) : subtreeStatus(MAX_LEVEL, 0){
  parent = p;
  left = NULL;
  right = NULL;
  type = t;
  pid = -1;
  level = l;
  status = FREE;
  incrementStatus(level);
}

int Node::getPID() {return pid;}

int Node::getLevel() {return level;}

MemType Node::getType() {return type;}


bool Node::hasLevel(int l) {
  return subtreeStatus[l] > 0;
}

void Node::decrementStatus(int l) {
  subtreeStatus[l]--;
}

void Node::incrementStatus(int l) {
  subtreeStatus[l]++;
}

bool Node::alloc(int p, int l, map<int, Node *> &memLocation) {
  // Base case:
  if (level == l && status == FREE) {
    cout << "Find free node for " << p << " level = " << level <<  endl;
    decrementStatus(l);
    status = ALLOCATED;
    pid = p;
    memLocation[p] = this;
    return true;
  }

  if (hasLevel(l)) {
    // Case 1: there are free node in the level already
    decrementStatus(l);
    if (left->hasLevel(l)) {
      return left->alloc(p, l, memLocation);
    } else {
      return right->alloc(p, l, memLocation);
    }
  } else {
    // Case 2: need to split a free node to get the target level node
    cout << "Need to split for " << p << endl;
    // Find the lowest level that could split
    int splitLevel = -1;
    for (int i = l - 1; i >= 0; i--) {
      if (hasLevel(i)) {
	splitLevel = i;
	break;
      }
    }
    // No level has free space to split, return false
    if (splitLevel == -1) {
      cout << "No space for alloc " << p << endl;
      return false;
    }
    // Split
    Node *n = split(splitLevel, l);
    return n->alloc(p, l, memLocation);
  }
}

Node * Node::split(int splitLevel, int targetLevel) {
  // Base case: already arrived at the target level   
  if (level == targetLevel) {
    return this;
  }
  if (level < splitLevel) {
    // Case1: not arrived the split Level yet
    // Update subtree status
    decrementStatus(splitLevel);
    for (int i = splitLevel + 1; i <= targetLevel; i++)
      incrementStatus(i);
    // continue to find the split node
    if (left->hasLevel(splitLevel)) {
      return left->split(splitLevel, targetLevel);
    } else {
      return right->split(splitLevel, targetLevel);
    }
  } else {
    // Case2: split the current node
    // update the subtree status
    status = BRANCH;
    decrementStatus(level);
    for (int i = level + 1; i <= targetLevel; i++)
      incrementStatus(i);
    // split the node
    left = new Node(this, type, level + 1);
    right = new Node(this, type, level + 1);
    // Continue with left node
    return left->split(splitLevel, targetLevel);
  }
}

void Node::free(vector<Node *> &toBeDeleted, int l) {
  if (parent && parent->hasLevel(level)) {
    cout << "Start free " << pid << endl;
    // Case 1: the node's sibling is also free, merge up
    cout << "sibling is free " << endl;
    toBeDeleted.push_back(parent->left);
    toBeDeleted.push_back(parent->right);
    parent->left = NULL;
    parent->right = NULL;
    parent->status = FREE;
    parent->free(toBeDeleted, l);
  } else {
    // Case 2: the node's sibling is not free
    cout << "sibling is not free " << endl;
    this->status = FREE;
    // Update subtree status
    Node *p = this;
    while (p) {
      for (int i = l; i > level; i--)
	p->decrementStatus(i);
      p->incrementStatus(level);
      p = p->parent;
    }
  }
}

void Node::printTree(vector<int> &stack) {
  // Base case, print leaf node
  if (!left && !right) {
    for (vector<int>::iterator it = stack.begin(); it != stack.end(); it++)
      cout << *it;
    if (status == FREE)
      cout << " free" << endl;
    else
      cout << " " << this->getPID() << endl;
    //    for (map<int, int>::iterator it)
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

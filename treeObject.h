#ifndef treeObject
#define treeObject

#include <vector>
#include <iostream>
#include <map>
#include "tokenizer.h"

typedef enum {FREE, ALLOCATED, BRANCH} NodeStatus;
typedef enum {BUDDY, SLAB} MemType;
#define MAX_LEVEL 31

using std::vector;
using std::map;
using std::cout;
using std::endl;

class Node {
 private:
  Node *parent;
  Node *left;
  Node *right;

  int pid;
  int level;
  NodeStatus status;    // True means the node is free, false means this node has been allocated to some process.
  vector<int> subtreeStatus; //Store the subtree status below the current node

public:
  // Constructor only use to initialize root
  Node(int totalLevel);

  // Contructor for split nodes
  Node(Node *p, int l, int totalLevel);

  bool hasLevel(int l);

  void decrementStatus(int l);

  void incrementStatus(int l);

  int getPID();

  int getLevel();

  Node *alloc(int p, int l, int totalLevel);
  
  Node *split(int splitLevel, int targetLevel, int totalLevel);

  void free(vector<Node *> &toBeDeleted, int l);

  void printTree(vector<int> &);

  ~Node();

};

class MemoryManager {
 private:
  Node *root;
  Node *slabRoot;

  UNIT totalSpace;
  UNIT buddyFreeSpace;
  UNIT slabFreeSpace;
  UNIT slabSize;

  int totalLevel;
  int slabTotalLevel;
  int slabLevel;
  int slabHeadLevel;
  // Store the pid and node mapping
  map<int, Node *> pidToPointer;
  
 public:
  MemoryManager(UNIT total, int portion, UNIT slabSize);
  Node *alloc(int pid, UNIT size);
  Node *buddyAlloc(int pid, UNIT size);
  Node *slabAlloc(int pid, UNIT size);
  Node *realloc(int pid, UNIT size);
  bool free(int pid);
  void dump();
  ~MemoryManager();
};

#endif

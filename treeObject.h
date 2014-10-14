#ifndef treeObject
#define treeObject

#include <vector>
#include <map>
#include <iostream>

typedef enum {SLAB, BUDDY} MemType;
typedef enum {FREE, ALLOCATED, BRANCH} NodeStatus;

using std::vector;
using std::map;
using std::cout;
using std::endl;

class Node {
private:
  Node *parent;
  Node *left;
  Node *right;

  MemType type;
  int pid;
  int level;
  NodeStatus status;    // True means the node is free, false means this node has been allocated to some process.
  map<int, int> subtreeStatus; //Store the subtree status below the current node

public:
  // Constructor only use to initialize root
  Node(MemType t);

  // Contructor for split nodes
  Node(Node *p, MemType t, int l);

  Node(int pid, int level, MemType t);

  Node *getParent();
  
  Node *getLeft();
  
  Node *getRight();

  bool hasLevel(int l);

  void initStatus(int l);

  void eraseStatus(int l);

  void decrementStatus(int l);

  void incrementStatus(int l);

  bool isFree();

  int getPID();

  int getLevel();

  MemType getType();

  bool alloc(int p, int l, map<int, Node *> &memLocation);
  
  Node *split(int splitLevel, int targetLevel);

  void free(vector<Node *> &toBeDeleted, int l);

  void printTree(vector<int> &);

  ~Node();

};

#endif

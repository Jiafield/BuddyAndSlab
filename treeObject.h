#ifndef treeObject
#define treeObject

#include <vector>
#include <iostream>
#include <map>

typedef enum {SLAB, BUDDY} MemType;
typedef enum {FREE, ALLOCATED, BRANCH} NodeStatus;
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

  MemType type;
  int pid;
  int level;
  NodeStatus status;    // True means the node is free, false means this node has been allocated to some process.
  vector<int> subtreeStatus; //Store the subtree status below the current node

public:
  // Constructor only use to initialize root
  Node(MemType t);

  // Contructor for split nodes
  Node(Node *p, MemType t, int l);

  bool hasLevel(int l);

  void decrementStatus(int l);

  void incrementStatus(int l);

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

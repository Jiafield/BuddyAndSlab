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
  Node(MemType t);

  Node(MemType t, int l);

  Node(int pid, int level, MemType t);

  Node *getParent();
  
  Node *getLeft();
  
  Node *getRight();

  bool isFree();

  int getPID();

  int getLevel();

  MemType getType();

  void free(vector<Node *> &toBeDeleted, int l) {
    if ((parent->subtreeStatus)[level] == 1) {
      // Case 1: the node's sibling is also free, merge up
      toBeDeleted.push_back(parent->left);
      toBeDeleted.push_back(parent->right);
      parent->left = NULL;
      parent->right = NULL;
      parent->status = FREE;
      // Update subtree status
      //      for (int i = l; i >= level; i--)
      //	(parent->subtreeStatus).erase(i);
      parent->free(toBeDeleted, l);
    } else {
      // Case 2: the node's sibling is allocated
      this->status = FREE;
      // Update subtree status
      Node *p = this;
      while (p) {
	for (int i = l; i > p->level; i--)
	  (p->subtreeStatus).erase(i);
	p->subtreeStatus[level]++;
	p = p->parent;
      }
    }
  }

  bool alloc(int p, int l, map<int, Node *> memLocation) {
    // Base case:
    if (level == l && status == FREE) {
      cout << "find free node for " << p << " level = " << level <<  endl;
      subtreeStatus[level] = 0;
      status = ALLOCATED;
      pid = p;
      memLocation[p] = this;
      return true;
    }
    if (subtreeStatus.find(l) != subtreeStatus.end()
	&& subtreeStatus[l] > 0) {
      // Case 1: there are free node in the level already
      subtreeStatus[l]--;
      if ((left->subtreeStatus).find(l) != (left->subtreeStatus).end() 
	  && (left->subtreeStatus)[l] != 0) {
	return left->alloc(p, l, memLocation);
      } else {
	return right->alloc(p, l, memLocation);
      }
    } else {
      // Case 2: need to split a free node to get the target level node
      cout << "need to split for " << p << endl;
      int splitLevel = -1;
      cout << "need to split for " << p << endl;
      // Find the lowest level that could split
      for (int i = l - 1; i >= 0; i--) {
	if (subtreeStatus.find(i) != subtreeStatus.end() && subtreeStatus[i] > 0) {
	  splitLevel = i;
	  break;
	}
      }
      if (splitLevel == -1) {
	cout << "No space for alloc " << p << endl;
	return false;
      }
      // Split
      Node *n = split(splitLevel, l);
      return n->alloc(p, l, memLocation);
    }
  }
  
  Node *split(int splitLevel, int targetLevel) {
    // Base case: already arrived at the target level   
    if (level == targetLevel) {
      return this;
    }
    if (level < splitLevel) {
      // Case1: not arrived the split Level yet
      // Update subtree status
      subtreeStatus[splitLevel]--;
      for (int i = splitLevel + 1; i <= targetLevel; i++)
	subtreeStatus[i] += 1;
      // continue to find the split node
      if ((left->subtreeStatus)[splitLevel] > 0) {
	return left->split(splitLevel, targetLevel);
      } else {
	return right->split(splitLevel, targetLevel);
      }
    } else {
      // Case2: split the current node
      // update the subtree status
      subtreeStatus[level]--;
      status = BRANCH;
      for (int i = level + 1; i <= targetLevel; i++)
	subtreeStatus[i] = 1;
      // split the node
      left = new Node(type, level + 1);
      (left->subtreeStatus)[level + 1] = 1;
      right = new Node(type, level + 1);
      (right->subtreeStatus)[level + 1] = 1;
      // Continue with left node
      return left->split(splitLevel, targetLevel);
    }
  }

  void printTree(vector<int> &);

  ~Node();

};

#endif

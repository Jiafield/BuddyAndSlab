#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include "treeObject.h"
#include "tokenizer.h"

using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::queue;

MemoryManager::MemoryManager(UNIT total, int portion, UNIT slabSize) {
  totalSpace = total;
  slabFreeSpace = total / portion;
  buddyFreeSpace = total - slabFreeSpace;
  this->slabSize = slabSize;

  totalLevel = sizeToLevel(total);
  slabTotalLevel = sizeToLevel(slabFreeSpace);
  slabHeadLevel = totalLevel - sizeToLevel(slabFreeSpace);
  slabLevel = slabTotalLevel - sizeToLevel(slabSize);
 
  root = new Node(totalLevel);
  slabRoot = new Node(slabTotalLevel);

  // Mark the slab head in the buddy tree;
  root->alloc(-1, slabHeadLevel, totalLevel); 
}

Node *MemoryManager::alloc(int pid, UNIT size) {
  size = nextPower2(size);
  // Case 1: size is a slab size
  if (size == slabSize) {
    return slabAlloc(pid, size);
  }
  // Case 2: size is not slab size
  return buddyAlloc(pid, size);
}

Node *MemoryManager::buddyAlloc(int pid, UNIT size) {
  int targetLevel = totalLevel - sizeToLevel(size);
  Node *newNode = root->alloc(pid, targetLevel, totalLevel);
  if (!newNode) {
    // Try compaction
    cout << "Compact buddy for " << pid << endl;
    compaction(root);
    newNode = root->alloc(pid, targetLevel, totalLevel);
    if (!newNode) {
      //borrow from slab
      cout << "Alloc: No space in buddy for " << pid << endl;
      cout << "Borrow from slab" << endl;
      newNode = borrowSlabAlloc(pid, size);
      if (!newNode)
	return NULL;
    }
  }
  pidToPointer[pid] = newNode;
  buddyFreeSpace -= size;
  return newNode;
}


Node *MemoryManager::slabAlloc(int pid, UNIT size) {
  Node *newNode = slabRoot->alloc(pid, slabLevel, slabTotalLevel);
  if (!newNode) {
    // Compaction
    cout << "Compact slab for " << pid << endl;
    compaction(slabRoot);
    newNode = slabRoot->alloc(pid, slabLevel, totalLevel);    
    if (!newNode) {
      // Borrow from buddy
      cout << "Alloc: No space in slab for " << pid << endl;
      cout << "Borrow from buddy" << endl;
      newNode = borrowBuddyAlloc(pid, size);
      if (!newNode)
	return NULL;
    }
  }
  slabFreeSpace -= slabSize;
  pidToPointer[pid] = newNode;
  return newNode;
}

Node *MemoryManager::borrowBuddyAlloc(int pid, UNIT size) {
  int targetLevel = totalLevel - sizeToLevel(size);
  Node *newNode = root->alloc(pid, targetLevel, totalLevel);
  if (!newNode) {
    // Try compaction
    cout << "Compact buddy" << endl;
    compaction(root);
    newNode = root->alloc(pid, targetLevel, totalLevel);
    if (!newNode) {
      return NULL;
    }
  }
  pidToPointer[pid] = newNode;
  buddyFreeSpace -= size;
  return newNode;
}

Node *MemoryManager::borrowSlabAlloc(int pid, UNIT size) {
  int targetLevel = slabTotalLevel - sizeToLevel(size);
  Node *newNode = slabRoot->alloc(pid, targetLevel, slabTotalLevel);
  if (!newNode) {
    compaction(slabRoot);
    newNode = slabRoot->alloc(pid, targetLevel, slabTotalLevel);
    if (!newNode)
      return NULL;
  }
  pidToPointer[pid] = newNode;
  slabFreeSpace -= size;
  return newNode;
}

Node *MemoryManager::realloc(int pid, UNIT size) {
  // Look up the pid in the pid to pointer map
  if (pidToPointer.find(pid) == pidToPointer.end()) {
    cout << "Realloc: never allocated memory for " << pid << " before." << endl;
    return NULL;
  }
  // Check if the original size fit for the new size
  Node *originNode = pidToPointer[pid];
  size = nextPower2(size);
  int targetLevel;
  if (size == slabSize)
    targetLevel = slabLevel;
  else 
    targetLevel = totalLevel - sizeToLevel(size);

  if (targetLevel == originNode->getLevel()) {
    // Case 1: the original size fit the new size
    return originNode;
  } else {
    // Case 2: alloc new size for pid
    free(pid);
    return alloc(pid, size);
  }
}

void MemoryManager::compaction(Node *r) {
  queue<Node *> freeQueues[totalLevel];
  vector<Node *> toBeDeleted;
  // DFS traversal
  r->DFSTravesal(freeQueues, r, totalLevel, pidToPointer);
  r->DFSFree(toBeDeleted);
  for (vector<Node *>::iterator n = toBeDeleted.begin(); n != toBeDeleted.end(); n++) {
    pidToPointer.erase((*n)->getPID());
    delete *n;
  }
}

bool MemoryManager::free(int pid) {
  if (pidToPointer.find(pid) != pidToPointer.end()) {
    // Case 1: the pid in tree
    Node *n = pidToPointer[pid];
    vector<Node *> toBeDeleted;
    n->free(toBeDeleted, n->getLevel());
    for (vector<Node *>::iterator n = toBeDeleted.begin(); n != toBeDeleted.end(); n++) {
      pidToPointer.erase((*n)->getPID());
      delete *n;
    }
    return true;
  } else {
    // Case 2: no where to find pid
    return false;
  }
}

void MemoryManager::dump() {
  vector<int> stack(slabHeadLevel, 0);
  cout << endl << "Slab:" << endl;
  slabRoot->printTree(stack);
  stack.clear();
  cout << endl << "Buddy:" << endl;
  root->printTree(stack);
}

MemoryManager::~MemoryManager() {
  delete root;
  delete slabRoot;
}

// Constructor only use to initialize root
Node::Node(int totalLevel) : subtreeStatus(totalLevel + 1, 0){
  parent = NULL;
  left = NULL;
  right = NULL;
  pid = -1;
  level = 0;
  status = FREE;
  incrementStatus(level);
}

// Contructor for split nodes
Node::Node(Node *p, int l, int totalLevel) : subtreeStatus(totalLevel + 1, 0){
  parent = p;
  left = NULL;
  right = NULL;
  pid = -1;
  level = l;
  status = FREE;
  incrementStatus(level);
}

int Node::getPID() {return pid;}

int Node::getLevel() {return level;}

bool Node::hasLevel(int l) {
  return subtreeStatus[l] > 0;
}

void Node::decrementStatus(int l) {
  if (parent)
    parent->decrementStatus(l);
  subtreeStatus[l]--;
}

void Node::incrementStatus(int l) {
  if (parent)
    parent->incrementStatus(l);
  subtreeStatus[l]++;
}

Node* Node::alloc(int p, int l, int totalLevel) {
  // Base case:
  if (level == l && status == FREE) {
    decrementStatus(l);
    status = ALLOCATED;
    pid = p;
    return this;
  }

  if (hasLevel(l)) {
    // Case 1: there are free node in the level already
    if (left->hasLevel(l)) {
      return left->alloc(p, l, totalLevel);
    } else {
      return right->alloc(p, l, totalLevel);
    }
  } else {
    // Case 2: need to split a free node to get the target level node
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
      return NULL;
    }
    // Split
    Node *n = split(splitLevel, l, totalLevel);
    return n->alloc(p, l, totalLevel);
  }
}

Node *Node::split(int splitLevel, int targetLevel, int totalLevel) {
  // Base case: already arrived at the target level   
  if (level == targetLevel) {
    return this;
  }
  if (level < splitLevel) {
    // Case1: not arrived the split Level yet
    if (left->hasLevel(splitLevel)) {
      return left->split(splitLevel, targetLevel, totalLevel);
    } else {
      return right->split(splitLevel, targetLevel, totalLevel);
    }
  } else {
    // Case2: split the current node
    // update the subtree status
    status = BRANCH;
    decrementStatus(level);
    left = new Node(this, level + 1, totalLevel);
    right = new Node(this, level + 1, totalLevel);
    // Continue with left node
    return left->split(splitLevel, targetLevel, totalLevel);
  }
}

void Node::DFSTravesal(queue<Node *> freeQueues[], Node *root, int totalLevel, map<int, Node *>pidToPointer) {
  // Base case: arrive at leaf of tree
  if (status == FREE) {
    freeQueues[level].push(this);
    return;
  } else if (status == ALLOCATED) {
    // Move the allocated node to the left most free node, if there is any
    if (!freeQueues[level].empty()) {
      root->alloc(pid, level, totalLevel);	
      status = FREE;
      incrementStatus(level);
      freeQueues[level].pop();
      freeQueues[level].push(this);
    }
    return;
  }
  // Check left subtree
  left->DFSTravesal(freeQueues, root, totalLevel, pidToPointer);
  // Check right subtree
  right->DFSTravesal(freeQueues, root, totalLevel, pidToPointer);
}

void Node::DFSFree(vector<Node *> toBeDeleted) {
  if (!left && !right)
    return;
  left->DFSFree(toBeDeleted);
  right->DFSFree(toBeDeleted);
  if (left->status == FREE && right->status == FREE) {
    toBeDeleted.push_back(left);
    decrementStatus(level + 1);
    toBeDeleted.push_back(right);
    decrementStatus(level + 1);
    status = FREE;
    left = NULL;
    right = NULL;
    incrementStatus(level);
  }
}

void Node::free(vector<Node *> &toBeDeleted, int l) {
  if (parent && parent->hasLevel(level)) {
    // Case 1: the node's sibling is also free, merge up
    toBeDeleted.push_back(parent->left);
    toBeDeleted.push_back(parent->right);
    parent->left = NULL;
    parent->right = NULL;
    parent->status = FREE;
    parent->free(toBeDeleted, l);
  } else {
    // Case 2: the node's sibling is not free
    this->status = FREE;
    // Update subtree status
    incrementStatus(level);
    for (int i = l; i > level; i--)
      decrementStatus(i);
  }
}

void Node::printTree(vector<int> &stack) {
  // Base case, print leaf node
  if (!left && !right) {
    for (vector<int>::iterator it = stack.begin(); it != stack.end(); it++)
      cout << *it;
    if (status == FREE)
      cout << " free" << endl;
    else if (pid == -1)
      cout << " slab head" << endl;
    else 
      cout << " " << pid << endl;
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

Node::~Node() {
  if (left)
    delete left;
  if (right)
    delete right;
  //decrementStatus(level);
}

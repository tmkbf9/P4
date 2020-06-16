#ifndef __NODE_H__
#define __NODE_H__

#include <string>
#include <vector>

#include "token.h"

class Node {
 public:    
  Node(const std::string & nodeName) : nodeName(nodeName) {}
  Node(const std::string & nodeName, token & token) : nodeName(nodeName), tk(token) {}

  void addSubtree(Node * subTree, int count) {
    subTrees.push_back (subTree);
  }

 //private:
  std::string nodeName;
  token tk;
  std::vector<Node *> subTrees;
};

#endif

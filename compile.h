#ifndef __COMPILE_H__
#define __COMPILE_H__

#include <iosfwd>
#include <vector>
class Node;

class Compile {
 public:
  Compile();

  void compile(Node * rootNode, std::ostream & os);
  void traversePreOrder(Node* rootNode, std::ostream& os, int depth);

  //private
  std::vector<token> printTable;
};

#endif

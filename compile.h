#ifndef __COMPILE_H__
#define __COMPILE_H__

#include <iosfwd>
#include <vector>
#include <string>

class Node;

class Compile {
 public:
  Compile() : tempVariableCount(0) {}

  void compile(Node * rootNode, std::ostream & os);
  void traversePreOrder(Node* rootNode, std::ostream& os, int depth);

 private:
  std::string createTemporaryVariable();
  std::string processANode(Node* rootNode, std::string output);
  std::string processWNode(Node* rootNode, std::string output);
  std::string processHRnodes(token& tk, bool ampersandFlag);
  std::string processMNode(Node * rootNode, std::string output);
  std::vector<token> symbolTable;
  int tempVariableCount;
};

#endif

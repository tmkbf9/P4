#ifndef __COMPILE_H__
#define __COMPILE_H__

#include <iosfwd>
#include <vector>
#include <string>

class Node;

class Compile {
 public:
     Compile() : tempVariableCount(0), conditionalCount(1) {}

  void compile(Node * rootNode, std::ostream & os);
  void traversePreOrder(Node* rootNode, std::ostream& os, int depth);

 private:
  std::string createTemporaryVariable();
  std::string processQNode(Node* rootNode, std::string output);
  std::string processTNode(Node* rootNode, std::string output);
  std::string processANode(Node* rootNode, std::string output);
  std::string processWNode(Node* rootNode, std::string output);
  std::string processINode(Node* rootNode, std::string output, std::string gotoString);
  std::string processGNode(Node* rootNode, std::string output, std::string gotoString);
  std::string processHRnodes(Node* rootNode, bool ampersandFlag, std::string assemblyCommand);
  std::string processMNode(Node * rootNode, std::string output, std::string assemblyCommand);
  std::vector<token> symbolTable;
  int tempVariableCount;
  int conditionalCount;
};

#endif

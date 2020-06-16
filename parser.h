#ifndef __PARSER_H__
#define __PARSER_H__

#include "available_tokens.h"

class Scanner;
class Node;

class Parser {
 public:
  Parser() : errTk(), validParse(true) {}

  Node * parse(Scanner & scanner);

  Node * createSNode(AvailableTokens& token);
  Node * createBNode(AvailableTokens& token);
  Node * createVNode(AvailableTokens& token);
  Node * createMNode(AvailableTokens& token);
  Node * createHNode(AvailableTokens& token);
  Node * createRNode(AvailableTokens& token);
  Node * createQNode(AvailableTokens& token);
  Node * createTNode(AvailableTokens& token);
  Node * createANode(AvailableTokens& token);
  Node * createWNode(AvailableTokens& token);
  Node * createINode(AvailableTokens& token);
  Node * createGNode(AvailableTokens& token);
  Node * createENode(AvailableTokens& token);
  Node * createZNode(AvailableTokens& token);

 private:
  bool validParse;
  token errTk;
};

#endif

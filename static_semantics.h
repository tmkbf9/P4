#ifndef __STATIC_SEMANTICS_H__
#define __STATIC_SEMANTICS_H__

#include <iosfwd>
#include <vector>

class Node;

class StaticSemantics {
public:
    void semantics(Node* rootNode, std::ostream& os);
    void traversePreOrder(Node* rootNode, std::ostream& os, int depth);
    bool insertSymbol(token & tk);
    bool verify(token& tk);
    void printSymbolTable(token & tk, std::ostream& os);


//private:
    std::vector<token> symbolTable;
    token errTk;
    bool redefiningVariableError;
    bool undefinedVariableError;
};

#endif
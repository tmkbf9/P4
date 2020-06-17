#include <iostream>
#include <string>

#include "node.h"
#include "token.h"
#include "compile.h"


using namespace std;

Compile::Compile() {}

void Compile::compile(Node * rootNode, ostream & os) {

    traversePreOrder(rootNode, os, 0);
    os << "STOP" << endl;
    for (int i = 0; i < symbolTable.size(); i++) {
        os << symbolTable[i].tokenLiteral << " 0" << endl;
    }
}

void Compile::traversePreOrder(Node* rootNode, ostream& os, int depth) {
    if (rootNode == NULL) return;

    if (rootNode->nodeName == "<V>") {
        os << "READ " << rootNode->subTrees[1]->tk.tokenLiteral << endl;
        symbolTable.push_back(rootNode->subTrees[1]->tk);
    }

    for (int subTreeIndex = 0; subTreeIndex < rootNode->subTrees.size(); subTreeIndex++) {
        traversePreOrder(rootNode->subTrees[subTreeIndex], os, depth + 1);
    }
}

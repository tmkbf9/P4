#include <iostream>
#include <string>

#include "node.h"
#include "token.h"
#include "compile.h"


using namespace std;

Compile::Compile() {}

void Compile::compile(Node* rootNode, ostream& os) {
    int tCount = 0;

    traversePreOrder(rootNode, os, 0);

    os << "STOP" << endl;

    for (int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i].tokenID == "IDTK") {
            os << symbolTable[i].tokenLiteral << " 0" << endl;
        }
        else if (symbolTable[i].tokenID == "NUMTK") {
            os << "T" << tCount << " " << symbolTable[i].tokenLiteral << endl;
            tCount++;
        }
    }
}

void Compile::traversePreOrder(Node* rootNode, ostream& os, int depth) {
    if (rootNode == NULL) return;

    if (rootNode->nodeName == "<V>") {
        symbolTable.push_back(rootNode->subTrees[1]->tk);
    }
    if (rootNode->nodeName == "<W>") {
        os << "WRITE ";
    }
    if (rootNode->nodeName == "<E>") {
        os << "STORE ";
    }
    if (rootNode->nodeName == "<A>") {
        os << "READ ";
    
        if (rootNode->subTrees[1]->tk.tokenID == "IDTK") {
            os << rootNode->subTrees[1]->tk.tokenLiteral << endl;
        }
        else if(rootNode->subTrees[1]->tk.tokenID == "NUMTK") {
            os << rootNode->subTrees[1]->tk.tokenLiteral << endl;
            symbolTable.push_back(rootNode->subTrees[1]->tk);
        }
    }

    for (int subTreeIndex = 0; subTreeIndex < rootNode->subTrees.size(); subTreeIndex++) {
        traversePreOrder(rootNode->subTrees[subTreeIndex], os, depth + 1);
    }
}

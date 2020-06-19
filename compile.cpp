#include <iostream>
#include <sstream>
#include <string>

#include "node.h"
#include "token.h"
#include "compile.h"


using namespace std;

void Compile::compile(Node* rootNode, ostream& os) {
    traversePreOrder(rootNode, os, 0);

    os << "STOP" << endl;

    for (int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i].tokenID == "IDTK") {
            os << symbolTable[i].tokenLiteral << " 0" << endl;
        }
    }
}

void Compile::traversePreOrder(Node* rootNode, ostream& os, int depth) {
    if (rootNode == NULL) return;

    if (rootNode->nodeName == "<V>") {
        symbolTable.push_back(rootNode->subTrees[1]->tk);
    }
    else if (rootNode->nodeName == "<W>") {
        os << "WRITE ";
    }
    else if (rootNode->nodeName == "<E>") {
        os << "STORE ";
    }
    else if (rootNode->nodeName == "<A>") {
        os << "READ ";
    
        if (rootNode->subTrees[1]->tk.tokenID == "IDTK") {
            os << rootNode->subTrees[1]->tk.tokenLiteral << endl;
        }
        else if(rootNode->subTrees[1]->tk.tokenID == "NUMTK") {
	  token token = token::ID_Token(createTemporaryVariable(), rootNode->subTrees[1]->tk.linenumber);
	  symbolTable.push_back(token);

	  os << token.tokenLiteral << endl;
        }
    }
    else if(rootNode->nodeName == "<R>") {
        if (rootNode->subTrees[0]->tk.tokenID == "IDTK") {
            os << rootNode->subTrees[0]->tk.tokenLiteral << endl;
        }
        if(rootNode->subTrees[0]->tk.tokenID == "NUMTK") {
	  token token = token::ID_Token(createTemporaryVariable(), rootNode->subTrees[0]->tk.linenumber);
	  symbolTable.push_back(token);

	  os << token.tokenLiteral << endl;
        }
    }

    for (int subTreeIndex = 0; subTreeIndex < rootNode->subTrees.size(); subTreeIndex++) {
        traversePreOrder(rootNode->subTrees[subTreeIndex], os, depth + 1);
    }
}

string Compile::createTemporaryVariable() {
  ostringstream os;

  os << "T" << tempVariableCount++;

  return os.str();
}

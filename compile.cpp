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
        if (rootNode->subTrees[1]->subTrees.size() > 1) {
            os << "LOAD ";
        }
        os << "WRITE ";
    }
    else if (rootNode->nodeName == "<M>") {
        bool ampFlag = false;
        if (rootNode->subTrees.size() == 1) {
            
            if (rootNode->subTrees[0]->tk.tokenLiteral == "&") {
                //do & math
                //more if where rootNode->subTree[1]->subTrees0 == IDTK or NUMTK
            }
            else {
                string hrNode = processHRnodes(rootNode->subTrees[0]->subTrees[0]->subTrees[0]->tk, ampFlag);
                os <<  hrNode << endl;
            }
        }

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

    for (int subTreeIndex = 0; subTreeIndex < rootNode->subTrees.size(); subTreeIndex++) {
        traversePreOrder(rootNode->subTrees[subTreeIndex], os, depth + 1);
    }
}
string Compile::processHRnodes(token& tk, bool ampersandFlag) {
    if (tk.tokenID == "IDTK") {
        return tk.tokenLiteral;
    }
    else if (tk.tokenID == "NUMTK") {
        token token = token::ID_Token(createTemporaryVariable(), tk.linenumber);
        symbolTable.push_back(token);

        return token.tokenLiteral;
    }
}

string Compile::createTemporaryVariable() {
  ostringstream os;

  os << "T" << tempVariableCount++;

  return os.str();
}

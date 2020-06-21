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
    string output;

    if (rootNode == NULL) return;

    if (rootNode->nodeName == "<V>") {
        symbolTable.push_back(rootNode->subTrees[1]->tk);
    }
    else if (rootNode->nodeName == "<W>") {
        if (rootNode->subTrees[1]->subTrees.size() > 1) {
            os << "LOAD " << processMNode(rootNode->subTrees[1], output);
            token token = token::ID_Token(createTemporaryVariable(), 0);
            symbolTable.push_back(token);
            os << "STORE T" << tempVariableCount - 1 << "\n";
            os << "WRITE T" << tempVariableCount - 1 << "\n";
            
        }
        else {
            os << "WRITE " << processMNode(rootNode->subTrees[1], output);
        }
   }
    /*else if (rootNode->nodeName == "<M>") {
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
        else {
            //processhRnodes(something)
            //switch operator
        }
    }*/
    else if (rootNode->nodeName == "<E>") {
        os << "STORE " << processMNode(rootNode->subTrees[3], output);
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
    //<I> if [ M Z M ] <T>
    // if = br
    //Z -> A > B means A - B > 0, A : B = true. A < B means A - B < 0, A = B means A-B = 0
    //resolve M_left to M0 and M_right to M1 then eval M0 - M1, then compare using Z 
    else if (rootNode->nodeName == "<I>") {
        int leftMNode, rightMNode;
        os << "LOAD " << processMNode(rootNode->subTrees[3], output);
        token token = token::ID_Token(createTemporaryVariable(), 0);
        symbolTable.push_back(token);
        leftMNode = tempVariableCount - 1;
        os << "STORE T" << leftMNode << "\n";
        
        os << "LOAD " << processMNode(rootNode->subTrees[3], output);
        token = token::ID_Token(createTemporaryVariable(), 0);
        symbolTable.push_back(token);
        rightMNode = tempVariableCount - 1;
        os << "STORE T" << rightMNode << "\n";
        
        os << "LOAD T" << leftMNode << "\n";
        os << "SUB T" << rightMNode << "\n";
        os << "STORE T" << leftMNode << "\n";


        //compare to 0

    }
    for (int subTreeIndex = 0; subTreeIndex < rootNode->subTrees.size(); subTreeIndex++) {
        traversePreOrder(rootNode->subTrees[subTreeIndex], os, depth + 1);
    }
}

string Compile::processMNode(Node* rootNode, string output ) {
    bool ampFlag = false;
    if (rootNode->subTrees.size() == 1) {

        if (rootNode->subTrees[0]->tk.tokenLiteral == "&") {
            //do & math
            //more if where rootNode->subTree[1]->subTrees0 == IDTK or NUMTK
        }
        else {
          output = processHRnodes(rootNode->subTrees[0]->subTrees[0]->subTrees[0]->tk, ampFlag) + "\n";
        }
    }
    else {
        output = processHRnodes(rootNode->subTrees[0]->subTrees[0]->subTrees[0]->tk, ampFlag) + "\n";
        switch (rootNode->subTrees[1]->tk.tokenLiteral[0]) {
        case '+':
            output += "ADD " + processMNode(rootNode->subTrees[2], output);
            break;
        case '-':
            output += "SUB " + processMNode(rootNode->subTrees[2], output);
            break;
        case '*':
            output += "MULT " + processMNode(rootNode->subTrees[2], output);
            break;
        case '/':
            output += "DIV " + processMNode(rootNode->subTrees[2], output);
            break;
        }
    }
    return output;
}
string Compile::processHRnodes(token& tk, bool ampersandFlag) {
    if (tk.tokenID == "IDTK") {
        return tk.tokenLiteral;
    }
    else if (tk.tokenID == "NUMTK") {
        return tk.tokenLiteral;
        //token token = token::ID_Token(createTemporaryVariable(), tk.linenumber);
        //symbolTable.push_back(token);

        //return token.tokenLiteral;
    }
}

string Compile::createTemporaryVariable() {
  ostringstream os;

  os << "T" << tempVariableCount++;

  return os.str();
}

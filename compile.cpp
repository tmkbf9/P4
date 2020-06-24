#include <iostream>
#include <sstream>
#include <string>

#include "node.h"
#include "token.h"
#include "compile.h"


using namespace std;

namespace {
    string IntToString(int a)
    {
        ostringstream temp;
        temp << a;
        return temp.str();
    }
}
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
    else if (rootNode->nodeName == "<T>") {
        os << processTNode(rootNode->subTrees[0], output);
    } 
    //<I> if [ M Z M ] <T>
    // if = br
    //Z -> A > B means A - B > 0, A : B = true. A < B means A - B < 0, A = B means A-B = 0
    //resolve M_left to M0 and M_right to M1 then eval M0 - M1, then compare using Z 
    else if (rootNode->nodeName == "<I>") {
        int leftMNode, rightMNode;
        os << "LOAD " << processMNode(rootNode->subTrees[2], output);
        token tokenLeft = token::ID_Token(createTemporaryVariable(), 0);
        symbolTable.push_back(tokenLeft);
        leftMNode = tempVariableCount - 1;
        os << "STORE T" << leftMNode << "\n";
        
        os << "LOAD " << processMNode(rootNode->subTrees[4], output);
        token tokenRight = token::ID_Token(createTemporaryVariable(), 0);
        symbolTable.push_back(tokenRight);
        rightMNode = tempVariableCount - 1;
        os << "STORE T" << rightMNode << "\n";
        
        os << "LOAD T" << leftMNode << "\n";
        os << "SUB T" << rightMNode << "\n";
        os << "STORE T" << leftMNode << "\n";

        //Create JUMPX where X is number of JUMPS
        string gotoString = "GOTO" + IntToString(tempVariableCount++);
        cout << gotoString << endl;
        switch (rootNode->subTrees[3]->subTrees[0]->tk.tokenLiteral[0]) {
        case '=':
            //???????
            os << "BRZERO ";
            break;
        case ':':
            //?????
            break;
        case '>':
            os << "BRZNEG " << gotoString << endl;
            break;
        case '<':
            os << "BRZPOS " << gotoString << endl;
            break;
        }

        os << processTNode(rootNode->subTrees[6], output);

        os << gotoString << ": ";
        
    }
    for (int subTreeIndex = 0; subTreeIndex < rootNode->subTrees.size(); subTreeIndex++) {
        traversePreOrder(rootNode->subTrees[subTreeIndex], os, depth + 1);
    }
}
string Compile::processTNode(Node* rootNode, string output) {
    if (rootNode->nodeName == "<W>") {
        output += processWNode(rootNode, output);
    }
    else if (rootNode->nodeName == "<E>") {
        output += "STORE " + processMNode(rootNode->subTrees[3], output);
    }
    else if (rootNode->nodeName == "<A>") {
        output += processANode(rootNode, output);
    }
    //need I and G nodes!
    return output;
}
string Compile::processANode(Node* rootNode, string output) {
    output += "READ ";

    if (rootNode->subTrees[1]->tk.tokenID == "IDTK") {
         output += rootNode->subTrees[1]->tk.tokenLiteral  + "\n";
    }
    else if (rootNode->subTrees[1]->tk.tokenID == "NUMTK") {
        token token = token::ID_Token(createTemporaryVariable(), rootNode->subTrees[1]->tk.linenumber);
        symbolTable.push_back(token);

        output += token.tokenLiteral + "\n";
    }

    return output;
}
string Compile::processWNode(Node* rootNode, string output) {
    if (rootNode->subTrees[1]->subTrees.size() > 1) {
        output += "LOAD " + processMNode(rootNode->subTrees[1], output);
        token token = token::ID_Token(createTemporaryVariable(), 0);
        symbolTable.push_back(token);
        output += "STORE T" + IntToString(tempVariableCount - 1) + "\n";
        output += "WRITE T" + IntToString(tempVariableCount - 1) + "\n";

    }
    else {
        output += "WRITE " + processMNode(rootNode->subTrees[1], output);
    }
    return output;
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
    return tk.tokenLiteral;
}

string Compile::createTemporaryVariable() {
  ostringstream os;

  os << "T" << tempVariableCount++;

  return os.str();
}

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
    else if (rootNode->nodeName == "<Q>") {
        os << processQNode(rootNode, output);
    } 
   
    for (int subTreeIndex = 0; subTreeIndex < rootNode->subTrees.size(); subTreeIndex++) {
        traversePreOrder(rootNode->subTrees[subTreeIndex], os, depth + 1);
    }
}
string Compile::processQNode(Node* rootNode, string output) {
    output += processTNode(rootNode->subTrees[0]->subTrees[0], output);

    return output;
}
string Compile::processTNode(Node* rootNode, string output) {
    
if (rootNode->nodeName == "<W>") {
        output += processWNode(rootNode, output);
    }
    else if (rootNode->nodeName == "<E>") {
        output += "STORE " + processMNode(rootNode->subTrees[3], output);
    }
    else if (rootNode->nodeName == "<A>") {
        output = processANode(rootNode, output);
    }
    else if (rootNode->nodeName == "<I>") {
        
        string gotoString = "GOTO" + IntToString(conditionalCount++);

        output += processINode(rootNode, output, gotoString);

        string tOutput = "";
        output += processTNode(rootNode->subTrees[6]->subTrees[0], tOutput);

        output += gotoString + ": ";
    }
    else if (rootNode->nodeName == "<G>") {

        string gotoString = "GOTO" + IntToString(conditionalCount);
        string backString = "BACK" + IntToString(conditionalCount++);
        output += backString + ": ";
        string gOutput = "";
        output += processGNode(rootNode, gOutput, gotoString);

        string tOutput = "";
        output += processTNode(rootNode->subTrees[6]->subTrees[0], tOutput);

        output += "BR " + backString + "\n";
        output += gotoString + ": ";

}
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
string Compile::processINode(Node* rootNode, string output, string gotoString) {
    string leftMNode, rightMNode;
    output += "LOAD " + processMNode(rootNode->subTrees[2], output);
    token tokenLeft = token::ID_Token(createTemporaryVariable(), 0);
    symbolTable.push_back(tokenLeft);
    leftMNode = IntToString(tempVariableCount - 1);
    output += "STORE T" + leftMNode + "\n";

    output += "LOAD " + processMNode(rootNode->subTrees[4], output);
    token tokenRight = token::ID_Token(createTemporaryVariable(), 0);
    symbolTable.push_back(tokenRight);
    rightMNode = IntToString(tempVariableCount - 1);
    output += "STORE T" + rightMNode + "\n";

    output += "LOAD T" + leftMNode + "\n";
    output += "SUB T" + rightMNode + "\n";

    switch (rootNode->subTrees[3]->subTrees[0]->tk.tokenLiteral[0]) {
    case '=':
        output += "BRPOS " + gotoString + "\n";
        output += "BRNEG " + gotoString + "\n";
        break;
    case ':':
        //always true so no break?
        break;
    case '>':
        output += "BRZNEG " + gotoString + "\n";
        break;
    case '<':
        output += "BRZPOS " + gotoString + "\n";
        break;
    }

    return output;
}
string Compile::processGNode(Node* rootNode, string output, string gotoString) {
    string leftMNode, rightMNode;
    
    output += "LOAD " + processMNode(rootNode->subTrees[2], output);
    token tokenLeft = token::ID_Token(createTemporaryVariable(), 0);
    symbolTable.push_back(tokenLeft);
    leftMNode = IntToString(tempVariableCount - 1);
    output += "STORE T" + leftMNode + "\n";

    output += "LOAD " + processMNode(rootNode->subTrees[4], output);
    token tokenRight = token::ID_Token(createTemporaryVariable(), 0);
    symbolTable.push_back(tokenRight);
    rightMNode = IntToString(tempVariableCount - 1);
    output += "STORE T" + rightMNode + "\n";

    output += "LOAD T" + leftMNode + "\n";
    output += "SUB T" + rightMNode + "\n";
    //output += "STORE T" + leftMNode + "\n";

    switch (rootNode->subTrees[3]->subTrees[0]->tk.tokenLiteral[0]) {
    case '=':
        //???????
        output += "BRPOS " + gotoString + "\n";
        output += "BRNEG " + gotoString + "\n";
        break;
    case ':':
        //always true so no break?
        break;
    case '>':
        output += "BRZNEG " + gotoString + "\n";
        break;
    case '<':
        output += "BRZPOS " + gotoString + "\n";
        break;
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

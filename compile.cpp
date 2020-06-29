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
    output += processMNode(rootNode->subTrees[3], output, "STORE ");
}
    else if (rootNode->nodeName == "<A>") {
        output = processANode(rootNode, output);
    }
    else if (rootNode->nodeName == "<I>") {
        
        string gotoString = "GOTO" + IntToString(conditionalCount++);

        output += processINode(rootNode, output, gotoString);

        string tOutput = "";
        output += processTNode(rootNode->subTrees[6]->subTrees[0], tOutput);

        output += gotoString + ": NOOP\n" ;
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
        output += gotoString + ": NOOP\n";

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
        output += processMNode(rootNode->subTrees[1], output, "LOAD ");
        token token = token::ID_Token(createTemporaryVariable(), 0);
        symbolTable.push_back(token);
        output += "STORE T" + IntToString(tempVariableCount - 1) + "\n";
        
        output += "WRITE T" + IntToString(tempVariableCount - 1) + "\n";

    }
    else if (rootNode->subTrees[1]->subTrees.size() == 1) {
        output += processMNode(rootNode->subTrees[1], output, "WRITE ");
    }
    return output;
}
string Compile::processINode(Node* rootNode, string output, string gotoString) {
    string leftMNode, rightMNode;
    output += processMNode(rootNode->subTrees[2], output, "LOAD ");
    token tokenLeft = token::ID_Token(createTemporaryVariable(), 0);
    symbolTable.push_back(tokenLeft);
    leftMNode = IntToString(tempVariableCount - 1);
    output += "STORE T" + leftMNode + "\n";

    output += processMNode(rootNode->subTrees[4], output, "LOAD ");
    token tokenRight = token::ID_Token(createTemporaryVariable(), 0);
    symbolTable.push_back(tokenRight);
    rightMNode = IntToString(tempVariableCount - 1);
    output += "STORE T" + rightMNode + "\n";
    if (rootNode->subTrees[3]->subTrees[0]->tk.tokenLiteral[0] != ':') {
        output += "LOAD T" + leftMNode + "\n";
        output += "SUB T" + rightMNode + "\n";
    }
    else {
        output += "LOAD T " + rightMNode + "\n";
        output += "STORE T" + leftMNode + "\n";
    }
    switch (rootNode->subTrees[3]->subTrees[0]->tk.tokenLiteral[0]) {
    case '=':
        output += "BRPOS " + gotoString + "\n";
        output += "BRNEG " + gotoString + "\n";
        break;
    case '>':
        output += "BRZNEG " + gotoString + "\n";
        break;
    case '<':
        output += "BRZPOS " + gotoString + "\n";
        break;
    default:
        break;
    }

    return output;
}
string Compile::processGNode(Node* rootNode, string output, string gotoString) {
    string leftMNode, rightMNode;
    
    output += processMNode(rootNode->subTrees[2], output, "LOAD ");
    token tokenLeft = token::ID_Token(createTemporaryVariable(), 0);
    symbolTable.push_back(tokenLeft);
    leftMNode = IntToString(tempVariableCount - 1);
    output += "STORE T" + leftMNode + "\n";

    output += processMNode(rootNode->subTrees[4], output, "LOAD ");
    token tokenRight = token::ID_Token(createTemporaryVariable(), 0);
    symbolTable.push_back(tokenRight);
    rightMNode = IntToString(tempVariableCount - 1);        
    output += "STORE T" + rightMNode + "\n";

    if (rootNode->subTrees[3]->subTrees[0]->tk.tokenLiteral[0] != ':') {
        output += "LOAD T" + leftMNode + "\n";
        output += "SUB T" + rightMNode + "\n";
    }
    else {
        output += "LOAD T " + rightMNode + "\n";
        output += "STORE T" + leftMNode + "\n";
    }
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
    default:
        break;
    }

    return output;
}
string Compile::processMNode(Node* rootNode, string output, string assemblyCommand ) {
    bool ampFlag = false;
    string ampString = "", tString = "";

    if (rootNode->subTrees[0]->subTrees[0]->tk.tokenLiteral == "&") {
        token tokenFirst = token::ID_Token(createTemporaryVariable(), 0);
        symbolTable.push_back(tokenFirst);

        ampString += "STORE " + tokenFirst.tokenLiteral + "\n";
        ampString += "LOAD " + processHRnodes(rootNode->subTrees[0]->subTrees[1]->subTrees[0]->tk, ampFlag) + "\n";
        
        token tokenSecond = token::ID_Token(createTemporaryVariable(), 0);
        symbolTable.push_back(tokenSecond);
        ampString += "STORE " + tokenSecond.tokenLiteral + "\n";
        string ampGoto = "AMP" + IntToString(conditionalCount++);
        ampString += "BRPOS "  + ampGoto + "\n";
        ampString += "MULT -1\n";
        ampString += "STORE " + tokenSecond.tokenLiteral + "\n";
        ampString += ampGoto + ": NOOP\n";
        ampString += "LOAD " + tokenSecond.tokenLiteral + "\n";
        output = tokenSecond.tokenLiteral + "\n";
    }
    else {
        output = processHRnodes(rootNode->subTrees[0]->subTrees[0]->subTrees[0]->tk, ampFlag) + "\n";
    }

    if (rootNode->subTrees.size() > 1) {
        switch (rootNode->subTrees[1]->tk.tokenLiteral[0]) {
        case '+':
            output += processMNode(rootNode->subTrees[2], output, "ADD ");
            break;
        case '-':
            output += processMNode(rootNode->subTrees[2], output, "SUB ");
            break;
        case '*':
            output += processMNode(rootNode->subTrees[2], output, "MULT ");
            break;
        case '/':
            output += processMNode(rootNode->subTrees[2], output, "DIV ");
            break;
        }
    }
    return ampString + assemblyCommand + output;
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

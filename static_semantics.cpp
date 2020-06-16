#include <string>
#include <sstream>
#include <iostream>


#include "token.h"
#include "node.h"
#include "static_semantics.h"

using namespace std;

void StaticSemantics::semantics(Node* rootNode, std::ostream& os) {
    redefiningVariableError = false;
    undefinedVariableError = false;

    if (rootNode->nodeName == "ERROR") {
        os << "Parse error encountered: " << rootNode->tk << endl;
    }
    else {
        traversePreOrder(rootNode, os, 0);
        printSymbolTable(errTk, os);
    }
    
}

void StaticSemantics::traversePreOrder(Node* rootNode, ostream& os, int depth) {
    if (rootNode == NULL || redefiningVariableError || undefinedVariableError) return;

    if (rootNode->nodeName == "<V>") {
        redefiningVariableError = insertSymbol(rootNode->subTrees[1]->tk);
    }
    else if (rootNode->nodeName == "<R>"  && rootNode->subTrees[0]->tk.tokenID == "IDTK" ) {
        undefinedVariableError = verify(rootNode->subTrees[0]->tk);
    }
    else if (rootNode->nodeName == "<A>" && rootNode->subTrees[1]->tk.tokenID == "IDTK")
    {
        undefinedVariableError = verify(rootNode->subTrees[1]->tk);
    }
    else if (rootNode->nodeName == "<E>") {
        undefinedVariableError = verify(rootNode->subTrees[1]->tk);
    }

    for (int subTreeIndex = 0; subTreeIndex < rootNode->subTrees.size(); subTreeIndex++) {
        traversePreOrder(rootNode->subTrees[subTreeIndex], os, depth + 1);
    }
}

bool StaticSemantics::insertSymbol(token & tk) {
    for(int index = 0; index < symbolTable.size(); index++) {
        if (symbolTable[index].tokenLiteral == tk.tokenLiteral) {
            errTk = tk;
            return true;
        }
    }

    symbolTable.push_back(tk);
    return false;
}

bool StaticSemantics::verify(token& tk) {
    for (int index = 0; index < symbolTable.size(); index++) {
        if (symbolTable[index].tokenLiteral == tk.tokenLiteral) {
            return false;
        }
    }

    errTk = tk;
    return true;
}

void StaticSemantics::printSymbolTable(token & err,std::ostream& os) {
    if (redefiningVariableError) {
        os << "Semantic error: redefined variable " << err.tokenLiteral << " at " << err.linenumber << endl;
    }
    else if (undefinedVariableError) {
        os << "Semantic error: undefiined variable " << err.tokenLiteral << " at " << err.linenumber << endl;
    }
    else {
        os << "Symbol Table: " << endl;
        for (int i = 0; i < symbolTable.size(); i++) {
            os << symbolTable[i] << endl;
        }
    }
    os << endl;
}
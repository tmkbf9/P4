#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "scanner.h"
#include "parser.h"
#include "static_semantics.h"
#include "node.h"
#include "token.h"

using namespace std;

void test_no_vars_declared() {
    istringstream is("program begin end");
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = parser.parse(scanner);
    StaticSemantics sem;

    sem.semantics(rootNode, cerr);

    assert(sem.symbolTable.size() == 0);
}

void test_one_var_declared() {
    istringstream is("program begin var aa . end");
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = parser.parse(scanner);
    StaticSemantics sem;

    sem.semantics(rootNode, cerr);

    assert(sem.symbolTable.size() == 1);
    assert(sem.symbolTable[0].tokenLiteral == "aa");
}

void test_two_vars_declared() {
    istringstream is("program begin var aa . var bb . end");
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = parser.parse(scanner);
    StaticSemantics sem;

    sem.semantics(rootNode, cerr);

    assert(sem.symbolTable.size() == 2);
    assert(sem.symbolTable[0].tokenLiteral == "aa");
    assert(sem.symbolTable[1].tokenLiteral == "bb");
}

void test_same_two_vars_declared_produces_error() {
    istringstream is("program begin var aa . var aa . end");
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = parser.parse(scanner);
    StaticSemantics sem;

    sem.semantics(rootNode, cerr);

    assert(sem.redefiningVariableError == true);
}

void test_same_three_vars_declared_produces_error() {
    istringstream is("program begin var aa . var aa . var aa . end");
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = parser.parse(scanner);
    StaticSemantics sem;

    sem.semantics(rootNode, cerr);

    assert(sem.redefiningVariableError == true);
}

void test_same_var_used_and_declared() {
    istringstream is("program var id1 . var id2 . begin var id3 . if [ id1 > 14 ]  write id3 , , # end");
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = parser.parse(scanner);
    StaticSemantics sem;

    sem.semantics(rootNode, cerr);

    assert(sem.symbolTable.size() == 3);
    assert(sem.symbolTable[0].tokenLiteral == "id1");
    assert(sem.symbolTable[1].tokenLiteral == "id2");
    assert(sem.symbolTable[2].tokenLiteral == "id3");
}

void test_vars_used_but_not_declared() {
    istringstream is("program var id1 . var id2 . begin if [ id1 > 14 ]  write id3 , , # end");
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = parser.parse(scanner);
    StaticSemantics sem;

    sem.semantics(rootNode, cerr);

    assert(sem.undefinedVariableError == true
    );
}

void test_multiple_var_used_and_declared() {
    istringstream is("program var red . var blue . begin var purple . repeat[ & red + blue == purple ] let purple : 5 * 3 + 2 , , # if [ 5 - 4 > 0 ] write 24 , , # end");
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = parser.parse(scanner);
    StaticSemantics sem;

    sem.semantics(rootNode, cerr);

    assert(sem.symbolTable.size() == 3);
    assert(sem.symbolTable[0].tokenLiteral == "red");
    assert(sem.symbolTable[1].tokenLiteral == "blue");
    assert(sem.symbolTable[2].tokenLiteral == "purple");
}

int main(int argc, char** argv) {
    test_no_vars_declared();
    test_one_var_declared();
    test_two_vars_declared(); 
    test_same_two_vars_declared_produces_error();
    test_same_three_vars_declared_produces_error();
    test_same_var_used_and_declared();
    test_vars_used_but_not_declared();
    test_multiple_var_used_and_declared();

    return 0;
}
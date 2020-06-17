#include <iostream>
#include <sstream>
#include <assert.h>
#include <string>

#include "scanner.h"
#include "parser.h"
#include "node.h"
#include "static_semantics.h"
#include "compile.h"

using namespace std;

void test_smallest_legal_program() {
    istringstream is("program begin end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;
    
    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);

    assert(os.str() == "STOP\n");
}

void test_single_variable() {
    istringstream is("program begin var foo . end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;
    
    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);

    string assembly = "STOP\nfoo 0\n";
    assert(os.str() == assembly);
}

void test_two_ID_variables() {
    istringstream is("program begin var foo . var foot . end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);

    string assembly = "STOP\nfoo 0\nfoot 0\n";
    assert(os.str() == assembly);
}

void test_writing_constant() {
    istringstream is("program begin var id1 . write id1 , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);

    string assembly = "WRITE id1 STOP\nid1 0\n";
    assert(os.str() == assembly);
}
void test_reading_identifier() {
    istringstream is("program begin var id1 . scan id1 , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
    string assembly = "READ id1\nSTOP\nid1 0\n";
    assert(os.str() == assembly);
}

void test_reading_number() {
    istringstream is("program begin scan 5 , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);

    string assembly = "READ 5\nSTOP\nT0 5\n";
    assert(os.str() == assembly);
}

int main(int argc, char ** argv) {
  test_smallest_legal_program();
  test_single_variable();
  test_two_ID_variables();
  //test_writing_constant();
  test_reading_identifier();
  test_reading_number();

  return 0;
}

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

    string assembly = "READ foo\nSTOP\nfoo 0\n";
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

    string assembly = "READ foo\nREAD foot\nSTOP\nfoo 0\nfoot 0\n";
    assert(os.str() == assembly);
}

void test_writing_constant() {
    istringstream is("program begin write 5,# end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);

    string assembly = "READ foo\nREAD foot\nSTOP\nfoo 0\nfoot 0\n";
    assert(os.str() == assembly);
}



int main(int argc, char ** argv) {
  test_smallest_legal_program();
  test_single_variable();
  test_two_ID_variables();

  return 0;
}

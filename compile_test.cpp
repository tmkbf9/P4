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

void test_writing_variable() {
    istringstream is("program begin var id1 . write id1 , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);

    string assembly = "WRITE id1\nSTOP\nid1 0\n";
    assert(os.str() == assembly);
}

void test_writing_constant() {
    istringstream is("program begin write 76 , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);

    string assembly = "WRITE 76\nSTOP\n";
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

    string assembly = "READ T0\nSTOP\nT0 0\n";
    assert(os.str() == assembly);
}

void test_reading_two_numbers() {
    istringstream is("program begin scan 5 , # scan 10 , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);

    string assembly = "READ T0\nREAD T1\nSTOP\nT0 0\nT1 0\n";
    assert(os.str() == assembly);
}
void test_writing_two_constants_sperated_by_operator() {
        istringstream is("program begin write 76 + 5 , # end");
        ostringstream os;
        Scanner scanner(is, cerr);
        Parser parser;
        Node* rootNode = NULL;
        StaticSemantics sem;
        Compile compiler;

        compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
        
        string assembly = "LOAD 76\nADD 5\nSTORE T0\nWRITE T0\nSTOP\nT0 0\n";
        assert(os.str() == assembly);
}
void test_writing_three_constants_sperated_by_operators() {
    istringstream is("program begin write 76 * 5 - 7 , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
    string assembly = "LOAD 76\nMULT 5\nSUB 7\nSTORE T0\nWRITE T0\nSTOP\nT0 0\n";
    assert(os.str() == assembly);
}

void test_if_with_two_numbers() {
    istringstream is("program begin if [ 7 > 5 ] write 76 , , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
    cout << os.str() << endl;
    string assembly = "LOAD 7\nSTORE T0\nLOAD 5\nSTORE T1\nLOAD T0\nSUB T1\nBRZNEG GOTO1\nWRITE 76\nGOTO1: STOP\nT0 0\nT1 0\n";
    assert(os.str() == assembly);
}

int main(int argc, char ** argv) {
  /*test_smallest_legal_program();
  test_single_variable();
  test_two_ID_variables();
  test_writing_variable();
  test_writing_constant();
  test_reading_identifier();
  test_reading_number();
  test_reading_two_numbers();
  test_writing_two_constants_sperated_by_operator();
  test_writing_three_constants_sperated_by_operators();
  */test_if_with_two_numbers();

  return 0;
}

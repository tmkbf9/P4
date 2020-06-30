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

void test_writing_identifier_and_number_sperated_by_operator() {
        istringstream is("program var foo . begin write foo + 5 , # end");
        ostringstream os;
        Scanner scanner(is, cerr);
        Parser parser;
        Node* rootNode = NULL;
        StaticSemantics sem;
        Compile compiler;

        compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
        
        string assembly = "LOAD foo\nADD 5\nSTORE T0\nWRITE T0\nSTOP\nfoo 0\nT0 0\n";
        assert(os.str() == assembly);
}

void test_writing_two_identifiers_sperated_by_operator() {
        istringstream is("program var foo . var bar . begin write foo + bar , # end");
        ostringstream os;
        Scanner scanner(is, cerr);
        Parser parser;
        Node* rootNode = NULL;
        StaticSemantics sem;
        Compile compiler;

        compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
        
        string assembly = "LOAD foo\nADD bar\nSTORE T0\nWRITE T0\nSTOP\nfoo 0\nbar 0\nT0 0\n";
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
    
    string assembly = "LOAD 7\nSTORE T0\nLOAD 5\nSTORE T1\nLOAD T0\nSUB T1\nBRZNEG GOTO1\nWRITE 76\nGOTO1: NOOP\nSTOP\nT0 0\nT1 0\n";
    assert(os.str() == assembly);
}

void test_repeat_with_two_numbers() {
    istringstream is("program begin repeat [ 7 > 5 ] write 76 , , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
    string assembly = "BACK1: LOAD 7\nSTORE T0\nLOAD 5\nSTORE T1\nLOAD T0\nSUB T1\nBRZNEG GOTO1\nWRITE 76\nBR BACK1\nGOTO1: NOOP\nSTOP\nT0 0\nT1 0\n";
    assert(os.str() == assembly);
}
void test_writing_if_with_single_Ms_seperated_by_comparison() {
    istringstream is("program var id1 . begin if [ id1 > 5 ] write 76 , , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
  
    string assembly = "LOAD id1\nSTORE T0\nLOAD 5\nSTORE T1\nLOAD T0\nSUB T1\nBRZNEG GOTO1\nWRITE 76\nGOTO1: NOOP\nSTOP\nid1 0\nT0 0\nT1 0\n";
    assert(os.str() == assembly);
}
void test_writing_if_with_multiple_Ms_on_left_seperated_by_comparison() {
    istringstream is("program var id1 . begin if [ id1 + 5 > 5 ] write 76 , , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
   
    string assembly = "LOAD id1\nADD 5\nSTORE T0\nLOAD 5\nSTORE T1\nLOAD T0\nSUB T1\nBRZNEG GOTO1\nWRITE 76\nGOTO1: NOOP\nSTOP\nid1 0\nT0 0\nT1 0\n";
    assert(os.str() == assembly);
}

void test_writing_if_with_multiple_Ms_on_right_seperated_by_comparison() {
    istringstream is("program var id1 . begin if [ 5 > id1 + 5 ] write 76 , , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
    string assembly = "LOAD 5\nSTORE T0\nLOAD id1\nADD 5\nSTORE T1\nLOAD T0\nSUB T1\nBRZNEG GOTO1\nWRITE 76\nGOTO1: NOOP\nSTOP\nid1 0\nT0 0\nT1 0\n";
    assert(os.str() == assembly);
}
void test_writing_if_with_nested_ifs() {
    //p <V> if [5 > id1 + 5] if [ 5 < 7 ] write 76 , , , # end
    istringstream is("program var id1 . begin if [ 5 > id1 + 5 ] if [ 5 < 7] write 76 , , , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
    string assembly = "LOAD 5\nSTORE T0\nLOAD id1\nADD 5\nSTORE T1\nLOAD T0\nSUB T1\nBRZNEG GOTO1\nLOAD 5\nSTORE T2\nLOAD 7\nSTORE T3\nLOAD T2\nSUB T3\nBRZPOS GOTO2\nWRITE 76\nGOTO2: NOOP\nGOTO1: NOOP\nSTOP\nid1 0\nT0 0\nT1 0\nT2 0\nT3 0\n";
    assert(os.str() == assembly);
}

void test_simplest_ampersand_use() {
    istringstream is("program begin write & 76 , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
    cout << os.str() << endl;
    string assembly = "STORE T0\nLOAD 76\nSTORE T1\nBRPOS AMP1\nMULT -1\nSTORE T1\nAMP1: NOOP\nLOAD T1\nWRITE T1\nSTOP\nT0 0\nT1 0\n";
    assert(os.str() == assembly);
}

void test_ampersand_then_number_use() {
    istringstream is("program begin write & 76 + 1 , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
    cout << os.str() << endl;
    string assembly = "STORE T0\nLOAD 76\nSTORE T1\nBRPOS AMP1\nMULT -1\nSTORE T1\nAMP1: NOOP\nLOAD T1\nLOAD T1\nADD 1\nSTORE T2\nWRITE T2\nSTOP\nT0 0\nT1 0\nT2 0\n";
    assert(os.str() == assembly);
}

void test_number_then_ampersand_use() {
    istringstream is("program begin write 1 + & 76 , # end");
    ostringstream os;
    Scanner scanner(is, cerr);
    Parser parser;
    Node* rootNode = NULL;
    StaticSemantics sem;
    Compile compiler;

    compiler.compile(sem.semantics(parser.parse(scanner), cerr), os);
    cout << os.str() << endl;
    string assembly = "STORE T0\nLOAD 76\nSTORE T1\nBRPOS AMP1\nMULT -1\nSTORE T1\nAMP1: NOOP\nLOAD T1\nADD 1\nSTORE T2\nWRITE T2\nSTOP\nT0 0\nT1 0\nT2 0\n";
    assert(os.str() == assembly);
}

int main(int argc, char ** argv) {
  test_smallest_legal_program();
  test_single_variable();
  test_two_ID_variables();
  test_writing_variable();
  test_writing_constant();
  test_reading_identifier();
  test_reading_number();
  test_writing_two_constants_sperated_by_operator();
  test_reading_two_numbers();
  test_writing_two_constants_sperated_by_operator();
  test_writing_three_constants_sperated_by_operators();
  test_if_with_two_numbers();
  test_repeat_with_two_numbers();
  test_writing_identifier_and_number_sperated_by_operator();
  test_writing_two_identifiers_sperated_by_operator();
  test_writing_if_with_single_Ms_seperated_by_comparison();
  test_writing_if_with_multiple_Ms_on_left_seperated_by_comparison();
  test_writing_if_with_multiple_Ms_on_right_seperated_by_comparison();
  test_writing_if_with_nested_ifs();
  test_simplest_ampersand_use();
  test_ampersand_then_number_use();
  test_number_then_ampersand_use();

  return 0;
}

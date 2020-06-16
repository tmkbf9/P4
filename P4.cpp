#include <iostream>
#include <fstream>
#include "scanner.h"
#include "parser.h"
#include "static_semantics.h"
#include "node.h"
#include "token.h"

using namespace std;

int main(int argc, char** argv) {
    StaticSemantics sem;
    token errorToken;


    if (argc == 1) { // read from stdin
        Scanner scanner(cin, cerr);
        Parser parser;
        Node* rootNode = parser.parse(scanner);
        sem.semantics(rootNode, cout);
    }
    else if (argc == 2) {// read from input file
        ifstream ifs(argv[1]);
        Scanner scanner(ifs, cerr);
        Parser parser;
        Node* rootNode = parser.parse(scanner);
        sem.semantics(rootNode, cout);
  }
  else {
      cerr << "Two many command line parameters given. Aborting" << endl;
      return 1;
  }

  return(0);
}

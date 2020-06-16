#include <iostream>

#include "compile.h"
#include "node.h"

using namespace std;

Compile::Compile() {}

void Compile::compile(Node * rootNode, ostream & os) {
  os << "STOP" << endl;
}

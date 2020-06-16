#include <iostream>
#include "token.h"

using namespace std;

void token::print(ostream & os) const {
  os << "Token type: " << tokenID << " literal: <" << tokenLiteral << "> linenumber: " << linenumber;
}

ostream & operator<<(ostream & os, const token & t) {
  t.print(os);
  return os;
}

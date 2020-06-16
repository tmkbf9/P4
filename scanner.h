#ifndef P1_SCANNER_H
#define P1_SCANNER_H

#include <iosfwd>
#include "token.h"

class Scanner {
 public:
 Scanner(std::istream & tokenStream, std::ostream & errorStream) 
   : tokenStream(tokenStream), errorStream(errorStream), linenumber(0) {}
  
  token scanner();

 private:
  Scanner(const Scanner & );
  Scanner & operator=(Scanner &);
  
  int typeOfChar(char) const;
  int FSATable(int, int) const;

  std::istream & tokenStream;
  std::ostream & errorStream;
  int linenumber;
};

#endif //P1_SCANNER_H

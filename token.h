#ifndef P1_TOKEN_H
#define P1_TOKEN_H

#include <string>

struct token {
  static token EOF_Token(int linenumber) { return token("EOFTK", "", linenumber); }
  static token NUM_Token(std::string digits, int linenumber) { return token("NUMTK", digits, linenumber); }
  static token ID_Token(std::string identifier, int linenumber) { return token("IDTK", identifier, linenumber); }
  static token SYM_Token(std::string symbol, int linenumber) { return token("SYMTK", symbol, linenumber); }
  static token KEY_Token(std::string symbol, int linenumber) { return token("KEYTK", symbol, linenumber); }
  static token ERR_Token(std::string errorString, int linenumber) { return token("ERRTK", errorString, linenumber); }

  token() {}
  void print(std::ostream & os) const;

  bool operator==(const token & other) const { return this->tokenID == other.tokenID; }
  bool operator!=(const token & other) const { return !(*this == other); }
  
  std::string tokenID;
  std::string tokenLiteral;
  int linenumber;

  token(std::string tokenID, std::string tokenLiteral, int linenumber) 
    : tokenID(tokenID), tokenLiteral(tokenLiteral), linenumber(linenumber) {}

private:
};

std::ostream & operator<<(std::ostream & os, const token & t);

#endif //P1_TOKEN_H


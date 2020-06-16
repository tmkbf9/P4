#ifndef __AVAILABLE_TOKENS_H__
#define __AVAILABLE_TOKENS_H_

#include "token.h"

class Scanner;

struct AvailableTokens {
AvailableTokens(Scanner & scanner, token & lookaheadToken) : _lookaheadToken(lookaheadToken), scanner(scanner) {}

  token nextToken();
  token lookaheadToken() const { return _lookaheadToken; }

private:
  token _lookaheadToken;
  Scanner & scanner;
};

#endif



#include "token.h"
#include "scanner.h"

#include "available_tokens.h"

token AvailableTokens::nextToken() {
  token nextToken = _lookaheadToken;
  _lookaheadToken = scanner.scanner();

  return nextToken;
}

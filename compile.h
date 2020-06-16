#ifndef __COMPILE_H__
#define __COMPILE_H__

#include <iosfwd>

class Node;

class Compile {
 public:
  Compile();

  void compile(Node * rootNode, std::ostream & os);

};

#endif

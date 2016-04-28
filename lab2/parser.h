#ifndef PARSER_H
#define PARSER_H


#include "lexer.h"
#include "tree.h"
#include <sstream>

struct parser {
  lexer l;
  parser(std::stringstream&);

  tree parse();

  tree B();
  tree VPrime();
  tree V();
  tree LPrime();
  tree L();
  tree N();
  tree D(); 
  tree S();
};

#endif

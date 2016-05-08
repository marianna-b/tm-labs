#ifndef PARSER_H
#define PARSER_H
#include <string>
#include "tree.h"
extern std::string _STR;

enum token_symb {
COMMA = 270,
DEREF = 269,
SEMICOLON = 268,
VAR = 267,
};

struct parser {
  int curr;
  parser();
  void next();
  tree decl();
  tree list();
  tree list_end();
  tree start();
  tree var();

};
#endif
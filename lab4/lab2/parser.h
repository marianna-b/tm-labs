#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include <iostream>
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
  std::vector<std::pair<std::string, std::string> >  decl();
  std::vector<std::pair<std::string, std::string> >  list(std::string);
  std::vector<std::pair<std::string, std::string> >  list_end(std::string);
  std::vector<std::pair<std::string, std::string> >  start();
  std::pair<std::string, std::string> var(std::string);

};
#endif
#ifndef PARSER_H
#define PARSER_H
#include <string>
extern std::string _STR;

enum token_symb {
AND = 270,
ASSIGN = 269,
DIV = 268,
EQ = 267,
EXP = 266,
FALSE = 265,
GT = 264,
IF = 263,
IFELSE = 262,
INTEGER = 261,
LT = 260,
MINUS = 259,
MUL = 258,
NEQ = 257,
NOT = 256,
OR = 255,
PLUS = 254,
PRINT = 253,
TRUE = 252,
VARIABLE = 251,
};

struct parser {
  int curr;
  parser();
  void next();
  std::string bool_expr();
  std::string expr();
  std::string int_expr();
  std::string list_expr();
  std::string start();

};
#endif
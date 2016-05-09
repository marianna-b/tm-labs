#include "parser.h"



std::string _STR;

extern int yylex();
parser::parser(){
  curr = yylex();
}

void parser::next(){
  curr = yylex();
}

std::string parser::bool_expr() {
  std::string res;
  if ((curr == NEQ)) {
    if (curr != NEQ)
      throw NEQ;
    next();
    std::string _s2 = int_expr();
    std::string _s3 = int_expr();

     res = "(" + _s2 + " != " + _s3 + ")"; 
    return res;
  }

  if ((curr == EQ)) {
    if (curr != EQ)
      throw EQ;
    next();
    std::string _s2 = int_expr();
    std::string _s3 = int_expr();

     res = "(" + _s2 + " == " + _s3 + ")"; 
    return res;
  }

  if ((curr == LT)) {
    if (curr != LT)
      throw LT;
    next();
    std::string _s2 = int_expr();
    std::string _s3 = int_expr();

     res = "(" + _s2 + " < " + _s3 + ")"; 
    return res;
  }

  if ((curr == GT)) {
    if (curr != GT)
      throw GT;
    next();
    std::string _s2 = int_expr();
    std::string _s3 = int_expr();

     res = "(" + _s2 + " > " + _s3 + ")"; 
    return res;
  }

  if ((curr == OR)) {
    if (curr != OR)
      throw OR;
    next();
    std::string _s2 = bool_expr();
    std::string _s3 = bool_expr();

     res = ("(" + _s2 + " or " + _s3 + ")"); 
    return res;
  }

  if ((curr == AND)) {
    if (curr != AND)
      throw AND;
    next();
    std::string _s2 = bool_expr();
    std::string _s3 = bool_expr();

     res = "(" + _s2 + " and " + _s3 + ")"; 
    return res;
  }

  if ((curr == NOT)) {
    if (curr != NOT)
      throw NOT;
    next();
    std::string _s2 = bool_expr();

     res = "not " + _s2; 
    return res;
  }

  if ((curr == FALSE)) {
    if (curr != FALSE)
      throw FALSE;
    next();

     res = "false"; 
    return res;
  }

  if ((curr == TRUE)) {
    if (curr != TRUE)
      throw TRUE;
    next();

     res = "true"; 
    return res;
  }
  throw curr;
}

std::string parser::expr() {
  std::string res;
  if ((curr == PRINT)) {
    if (curr != PRINT)
      throw PRINT;
    next();
    std::string _s2 = int_expr();

     res = "print " + _s2; 
    return res;
  }

  if ((curr == ASSIGN)) {
    if (curr != ASSIGN)
      throw ASSIGN;
    next();
    if (curr != VARIABLE)
      throw VARIABLE;
    std::string _s2 = _STR;
    next();
    std::string _s3 = int_expr();

     res = _s2 + " := " + _s3; 
    return res;
  }

  if ((curr == IFELSE)) {
    if (curr != IFELSE)
      throw IFELSE;
    next();
    std::string _s2 = bool_expr();
    std::string _s3 = expr();
    std::string _s4 = expr();

     res = "if " + _s2 + " {\n" + _s3  + "\n}" + " else { \n" + _s4 + "\n}"; 
    return res;
  }

  if ((curr == IF)) {
    if (curr != IF)
      throw IF;
    next();
    std::string _s2 = bool_expr();
    std::string _s3 = expr();

     res = "if " + _s2 + "{\n" + _s3  + "\n}"; 
    return res;
  }
  throw curr;
}

std::string parser::int_expr() {
  std::string res;
  if ((curr == VARIABLE)) {
    if (curr != VARIABLE)
      throw VARIABLE;
    std::string _s1 = _STR;
    next();

     res = _s1; 
    return res;
  }

  if ((curr == EXP)) {
    if (curr != EXP)
      throw EXP;
    next();
    std::string _s2 = int_expr();
    std::string _s3 = int_expr();

     res = "(" + _s3 + "^" + _s2 + ")"; 
    return res;
  }

  if ((curr == DIV)) {
    if (curr != DIV)
      throw DIV;
    next();
    std::string _s2 = int_expr();
    std::string _s3 = int_expr();

     res = "(" + _s2 + " / " + _s3 + ")"; 
    return res;
  }

  if ((curr == MUL)) {
    if (curr != MUL)
      throw MUL;
    next();
    std::string _s2 = int_expr();
    std::string _s3 = int_expr();

     res = "(" + _s2 + " * " + _s3 + ")"; 
    return res;
  }

  if ((curr == PLUS)) {
    if (curr != PLUS)
      throw PLUS;
    next();
    std::string _s2 = int_expr();
    std::string _s3 = int_expr();

     res = "(" + _s2 + " + " + _s3 + ")"; 
    return res;
  }

  if ((curr == MINUS)) {
    if (curr != MINUS)
      throw MINUS;
    next();
    std::string _s2 = int_expr();
    std::string _s3 = int_expr();

     res = "(" + _s2 + " - " + _s3 + ")"; 
    return res;
  }

  if ((curr == INTEGER)) {
    if (curr != INTEGER)
      throw INTEGER;
    std::string _s1 = _STR;
    next();

     res = _s1; 
    return res;
  }
  throw curr;
}

std::string parser::list_expr() {
  std::string res;
  if ((curr == ASSIGN) || (curr == IF) || (curr == IFELSE) || (curr == PRINT)) {
    std::string _s1 = expr();
    std::string _s2 = list_expr();

     res = _s1 + "\n" + _s2; 
    return res;
  }

  if ((curr == 0)) {

     res = ""; 
    return res;
  }
  throw curr;
}

std::string parser::start() {
  std::string res;
  if ((curr == ASSIGN) || (curr == IF) || (curr == IFELSE) || (curr == PRINT)) {
    std::string _s1 = list_expr();

     res = _s1; 
    return res;
  }

  if ((curr == 0)) {
    std::string _s1 = list_expr();

     res = _s1; 
    return res;
  }
  throw curr;
}


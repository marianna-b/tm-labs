#include "parser.h"



std::string _STR;

extern int yylex();
parser::parser(){
  curr = yylex();
}

void parser::next(){
  curr = yylex();
}

tree parser::decl() {
  tree res;
  if ((curr == VAR)) {
    if (curr != VAR)
      throw VAR;
    std::string _s1 = _STR;
    next();
    tree _s2 = list();
    if (curr != SEMICOLON)
      throw SEMICOLON;
    next();

     res = tree("decl"); res.add_child(_s1); res.add_child(_s2); 
    return res;
  }
  throw curr;
}

tree parser::list() {
  tree res;
  if ((curr == DEREF) || (curr == VAR)) {
    tree _s1 = var();
    tree _s2 = list_end();

     res = tree("list"); res.add_child(_s1); res.add_child(_s2); 
    return res;
  }
  throw curr;
}

tree parser::list_end() {
  tree res;
  if ((curr == COMMA)) {
    if (curr != COMMA)
      throw COMMA;
    next();
    tree _s2 = list();

    res = tree("list_end"); res.add_child(tree(",")); res.add_child(_s2); 
    return res;
  }

  if ((curr == SEMICOLON)) {

     res = tree("list_end"); 
    return res;
  }
  throw curr;
}

tree parser::start() {
  tree res;
  if ((curr == VAR)) {
    tree _s1 = decl();
    tree _s2 = start();

     res = tree("start"); res.add_child(_s1); res.add_child(_s2); 
    return res;
  }

  if ((curr == 0)) {

     res = tree("start"); 
    return res;
  }
  throw curr;
}

tree parser::var() {
  tree res;
  if ((curr == DEREF)) {
    if (curr != DEREF)
      throw DEREF;
    next();
    tree _s2 = var();

      res = tree("var"); res.add_child(tree("*")); res.add_child(_s2); 
    return res;
  }

  if ((curr == VAR)) {
    if (curr != VAR)
      throw VAR;
    std::string _s1 = _STR;
    next();

     res = tree("name: " + _s1); 
    return res;
  }
  throw curr;
}


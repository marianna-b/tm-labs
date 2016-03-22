#include "parser.h"
#include <iostream>
using namespace std;

lexer l;
parser::parser(stringstream& s) {
  l = move(lexer(s));
}
  
tree parser::parse() {
  l.next();
  return S();
}

tree parser::VPrime(){
  tree t("V'");
  if (l.cur().type == token_type::whitespace) {
    l.next();
    return t;
  }
  return t;
}
  
tree parser::V(){
  tree t("V");
  if (l.cur().type == token_type::whitespace) {
    l.next();
    return V();
  }
  if (l.cur().type == token_type::deref) {
    t.add_child(tree("*"));
    l.next();
    t.add_child(V());
    return t;
  }
  if (l.cur().type == token_type::name) {
    t.add_child(N());
    t.add_child(VPrime());
    return t;
  }
  throw l.cur();
}

tree parser::LPrime(){
  tree t("L'");
  if (l.cur().type == token_type::comma) {
    t.add_child(tree(","));
    l.next();
    t.add_child(L());
    return t;
  }
  return t;
}

tree parser::L(){
  tree t("L");
  if (l.cur().type == token_type::name || l.cur().type == token_type::whitespace || l.cur().type == token_type::deref) {
    t.add_child(V());
    t.add_child(LPrime());
    return t;
  }
  throw l.cur();
}

tree parser::N(){
  if (l.cur().type == token_type::name) {
    tree t("N: " + l.cur().s);
    l.next();
    return t;
  }
  throw l.cur();
}
  
tree parser::D(){
  tree t("D");
  if (l.cur().type == token_type::whitespace) {
    l.next();
  }
  t.add_child(N());
  if (l.cur().type == token_type::whitespace) {
    l.next();
  } else { 
    throw l.cur();
  }
  t.add_child(L());
  if (l.cur().type == token_type::semicolon) {
    t.add_child(tree(";"));
    l.next();
  } else { 
    throw l.cur();
  }
  return t;
}
  
tree parser::S() {
  if (l.cur().type == token_type::end) {
    return tree("S");
  }
  if (l.cur().type == token_type::name || l.cur().type == token_type::whitespace) {
    tree t("S");
    t.add_child(D());
    t.add_child(S());
    return t;
  }
  throw l.cur();
}

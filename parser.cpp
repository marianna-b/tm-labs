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
    cout << "VPrime (B)" << endl;
    return t;
  }
  cout << "VPrime ()" << endl;
  return t;
}
  
tree parser::V(){
  tree t("V");
  if (l.cur().type == token_type::whitespace) {
    l.next();
    return V();
  }
  if (l.cur().type == token_type::deref) {
    cout << "V (*, V)" << endl;
    t.add_child(tree("*"));
    l.next();
    t.add_child(V());
    return t;
  }
  if (l.cur().type == token_type::name) {
    cout << "V (N, VPrime)" << endl;
    t.add_child(N());
    t.add_child(VPrime());
    return t;
  }
  cout << "V fail" << endl;
  throw l.cur();
}

tree parser::LPrime(){
  tree t("L'");
  if (l.cur().type == token_type::comma) {
    t.add_child(tree(","));
    l.next();
    cout << "LPrime (, , L)" << endl;
    t.add_child(L());
    return t;
  }
  cout << "LPrime ()" << endl;
  return t;
}

tree parser::L(){
  tree t("L");
  if (l.cur().type == token_type::name || l.cur().type == token_type::whitespace || l.cur().type == token_type::deref) {
    cout << "L (V, LPrime)" << endl;
    t.add_child(V());
    t.add_child(LPrime());
    return t;
  }
  cout << "L fail" << endl;
  throw l.cur();
}

tree parser::N(){
  if (l.cur().type == token_type::name) {
    tree t("N: " + l.cur().s);
    l.next();
    cout << t.s << endl;
    return t;
  }
  cout << "N fail" << endl;
  throw l.cur();
}
  
tree parser::D(){
  tree t("D");
  if (l.cur().type == token_type::whitespace) {
    l.next();
  }
  cout << "D (N, L, ;)" << endl;
  t.add_child(N());
  if (l.cur().type == token_type::whitespace) {
    l.next();
  } else { 
    cout << "D fail" << endl;
    throw l.cur();
  }
  t.add_child(L());
  if (l.cur().type == token_type::semicolon) {
    t.add_child(tree(";"));
    l.next();
  } else { 
    cout << "D fail" << endl;
    throw l.cur();
  }
  return t;
}
  
tree parser::S() {
  if (l.cur().type == token_type::end) {
    return tree("S");
    cout << "S ()" << endl;
  }
  if (l.cur().type == token_type::name || l.cur().type == token_type::whitespace) {
    cout << "S (D, S)" << endl;
    tree t("S");
    t.add_child(D());
    t.add_child(S());
    return t;
  }
  cout << "S fail" << endl;
  throw l.cur();
}

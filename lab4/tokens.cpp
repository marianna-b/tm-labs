#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include "tokens.h"
using namespace std;

token::token(){}
  
token::token(token_type t) {
  type = t;
  s = "";
}
token::token(token_type t, string str) {
  type = t;
  s = str;
}
token::token(token_type t, int x) {
  type = t;
  val = x;
}


string token::show() {
 switch (type) {
 case token_type::var:
   return s; 
 case token_type::iff:
   return "if";
 case token_type::ifelse:
   return "ifelse";
 case token_type::print:
   return "print";
 case token_type::integer:
   return s;
 }
 throw *this;
}

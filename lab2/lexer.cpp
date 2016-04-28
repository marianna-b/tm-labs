#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include "lexer.h"
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


string token::show() {
 switch (type) {
 case token_type::name:
   return s; 
 case token_type::whitespace:
   return "blank";
 case token_type::comma:
   return ",";
 case token_type::semicolon:
   return ";";
 case token_type::deref:
   return "*";
 case token_type::end:
   return "$";
 case token_type::eq:
   return "=";
 }
 throw *this;
}


lexer::lexer(){}
lexer::lexer(stringstream& s) {
  stream = move(s);
  pos = -1;
  next_char();
}

bool lexer::is_space() {
  return (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t');
}

bool lexer::is_letter() {
  return (ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a');
}

bool lexer::is_digit() {
  return (ch <= '9' && ch >= '0');
}              

bool lexer::is_underscore() {
  return (ch == '_');
}

void lexer::next() {
  if (is_space()) {
    while (is_space())
      next_char();
  }
  if (is_letter()) {
    string cur_str = "";
    while (is_letter() || is_digit() || is_underscore()) {
      cur_str += ch;
      next_char();
    }
    t = token(token_type::name, cur_str);
    return;
  }
  switch (ch) {
  case '*':
    t = token(token_type::deref);
    next_char();
    break;
  case ',':
    t = token(token_type::comma);
    next_char();
    break;
  case ';':
    t = token(token_type::semicolon);
    next_char();
    break;
  case '=':
    t = token(token_type::eq);
    next_char();
    break;
  case 0:
    t = token(token_type::end);
    break;
  default:
    throw ch;
  }
}

token lexer::cur() {
  return t;
}

void lexer::next_char() {
  pos++;
  ch = 0;
  if (!stream.eof()) 
    stream.get(ch);
}

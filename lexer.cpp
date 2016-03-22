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

lexer::lexer(){}
lexer::lexer(stringstream& s) {
  stream = move(s);
  pos = 0;
  stream.get(ch);
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
    t = token(token_type::whitespace);
    while (is_space())
      next_char();
    return; 
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
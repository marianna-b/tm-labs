#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <sstream>

enum class token_type {name, whitespace, comma, semicolon, deref, end};

struct token {
  token_type type;
  std::string s;
  
  token();
  
  token(token_type);
  token(token_type, std::string);
  std::string show();
};

struct lexer {
  lexer();
  lexer(std::stringstream&);
  bool is_space();
  bool is_letter();
  bool is_digit();
  bool is_underscore();

  void next();
  token cur();
  void next_char();

private:
  int pos;
  char ch;
  std::stringstream stream;
  token t;
};
#endif

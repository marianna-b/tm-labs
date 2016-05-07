#ifndef TOKENS_H
#define TOKENS_H

enum class token_type {var, iff, ifelse, print, integer};

struct token {
  token_type type;
  std::string s;
  int val;
  
  token();
  
  token(token_type);
  token(token_type, std::string);
  token(token_type, int);
  std::string show();
};

#endif // TOKENS_H

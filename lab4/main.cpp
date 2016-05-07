#include <iostream>
#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif
#include "tokens.h"

token t;

int main( int /* argc */, char** /* argv */ )
{
  FlexLexer* lexer = new yyFlexLexer;
  int res;
    std::cout << "lol" << std::endl;
  while((res = lexer->yylex()) != 0) {
    std::cout << t.show() << std::endl;
    std::cout << "lol" << std::endl;
  }
  return 0;
}

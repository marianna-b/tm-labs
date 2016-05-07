#include <iostream>
#include "tokens.h"
#include "gen.h"
extern int yyparse();
extern parsed_info* result;

token t;

int main( int /* argc */, char** /* argv */ )
{
  yyparse();
  std::cout << result->generate();
  return 0;
}

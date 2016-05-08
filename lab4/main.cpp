#include <iostream>
#include "gen.h"
extern int yyparse();
extern parsed_info* result;

int main( int /* argc */, char** /* argv */ )
{
  yyparse();
  std::cout << result->generate();
  return 0;
}

#include <iostream>
#include "gen.h"
extern int yyparse();
extern parsed_info* result;

int main( int /* argc */, char** /* argv */ )
{
  yyparse();
  //std::cout << result->generate();
  //std::cout << "\n=====================================================\n";
  std::cout << result->generate_file();
  return 0;
}

#include <iostream>
#include <fstream>
#include "gen.h"
extern int yyparse();
extern parsed_info* result;

int main( int argc, char** argv)
{
  if (argc > 3)
    return 1;
  yyparse();
  std::cout << result->verify() << std::endl;
  /*
  std::ofstream file;
  file.open(argv[1]);
  file << result->generate_file();
  file.close();

  std::ofstream header;
  header.open(argv[2]);
  header << result->generate_header();
  header.close();
  */
  return 0;
}

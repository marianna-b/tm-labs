#include <iostream>
extern std::string* result;
extern int yyparse();

int main(int argc, char **argv)
{
    yyparse();
    if (result != 0)
      std::cout << *result << std::endl;
    return 0;
}

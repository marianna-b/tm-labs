#include <iostream>
extern std::string* result;
extern int yyparse();

int main(int argc, char **argv)
{
    yyparse();
    std::cout << *result << std::endl;
    return 0;
}

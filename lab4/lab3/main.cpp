#include <iostream>
#include "parser.h"

int main(int argc, char **argv)
{
  try {
    parser p;
    std::string res = p.start();
    std::cout << res << std::endl;
  } catch (int c) {
    std::cout << " Invalid char " << c << std::endl;
  }
  return 0;
}

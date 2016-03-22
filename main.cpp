#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include "parser.h"
using namespace std;

int main(int argc, char** argv) {
  stringstream s;
  s.str("int a, *b, * **c , d ; int a, *b, * **c , d ;");
  parser p(s);
  tree t = p.parse();
  return t.show(argc, argv);
}

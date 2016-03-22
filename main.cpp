#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include "parser.h"
using namespace std;

int main(int argc, char** argv) {
  stringstream s;
  //s.str("int a, *b, * **c , d ; int a, *b, * **c , d ;");
  //s.str("int a, *b, * **c , d ;");
  //s.str("int a;");
  //s.str("int *a;");
  //s.str("int a ;");
  //s.str("int * a ;");
  //s.str("");
  s.str("jddjdjdj;");
  parser p(s);
  try {
    tree t = p.parse();
    return t.show(argc, argv);
  } catch (token e) {
    cout << "Unexpected " << e.show(); 
    return 1;
  }
}

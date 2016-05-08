#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include "parser.h"
using namespace std;

const int TESTC = 1;
const int TESTC2 = 11;

string tests[TESTC] = {
  "int a, b = c;"
};
string test[TESTC2] = {
  "int a;",
  "int *a;",
  "int a ;",
  "int * a ;",
  "int a, *b, ***c, d; int a, *b, * * *c , d;",
  "int a, *b, ***c, d;",
  "",
  "a;",
  "a a a;",
  "a a,,a;",
  "a a,a^;"
};

int main(int argc, char** argv) {
  for (int i = 0; i < TESTC; ++i) {
    try {
      parser p;
      tree t = p.start();
      t.show(argc, argv);
      cout << tests[i] << " OK" << endl; 
    } catch (int c) {
      cout << tests[i] << " Invalid char " << c << endl;
    }
  }
  return 0;
}

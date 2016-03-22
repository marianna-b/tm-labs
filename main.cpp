#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include "parser.h"
using namespace std;

const int TESTC = 11;
string tests[TESTC] = {
  "int a, *b, ***c, d; int a, *b, * * *c , d;",
  "int a, *b, ***c, d;",
  "int a;",
  "int *a;",
  "int a ;",
  "int * a ;",
  "",
  "a;",
  "a a a;",
  "a a,,a;",
  "a a,a^;"
};


int main(int argc, char** argv) {
  for (int i = 0; i < TESTC; ++i) {
    stringstream s;
    s.str(tests[i]);
    parser p(s);
    try {
      tree t = p.parse();
      t.show(argc, argv);
      cout << tests[i] << " OK" << endl; 
    } catch (token e) {
      cout << tests[i] << " Unexpected " << e.show() << endl; 
    } catch (char c) {
      cout << tests[i] << " Invalid char " << c << endl;
    }
  }
  return 0;
}

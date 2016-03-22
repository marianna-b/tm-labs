#include "tree.h"
using namespace std;

tree::tree(string name) {
  s = name;
}

tree::tree() {
  s = "empty";
}

void tree::add_child(tree child) {
  children.push_back(child);
}

int tree::count_depth() {
  if (children.size() == 0)
    return 1;
  int res = 0;
  for (int i = 0; i < (int)children.size(); ++i) {
    res = max(children[i].count_depth(), res);
  }
  return res + 1;
}

int tree::count_leaves() {
  if (children.size() == 0)
    return 1;
  int sum = 0;
  for (int i = 0; i < (int)children.size(); ++i) {
    sum += children[i].count_leaves();
  }
  return sum;
}

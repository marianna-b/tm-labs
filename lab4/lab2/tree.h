#ifndef TREE_H
#define TREE_N

#include <vector>
#include <string>

struct tree {
  std::vector <tree> children;
  std::string s;

  tree(std::string);
  tree();
  void add_child(tree);
  int show(int argc, char ** argv);
  int count_leaves();
  int count_depth();
};

#endif

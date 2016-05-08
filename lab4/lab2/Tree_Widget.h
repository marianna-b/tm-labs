#ifndef TREE_WIDGET_H
#define TREE_WIDGET_H

#include <Fl.H>
#include <x.H>
#include "tree.h"

struct Tree_Widget:Fl_Widget {
  Tree_Widget(int, int, int, int, const char *);
  void draw();

  tree t;

private:
  int h, w;
  std::pair<int, int> show_tree(tree&, int, int);
};


#endif

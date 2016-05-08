#include "Tree_Widget.h"
#include <Fl_Window.H>
#include <Fl_Output.H>
#include <fl_draw.H>
#include <iostream>
using namespace std;

const int WIDTH = 1350;
const int HEIGHT = 750;
const int ALIGN = 3;

Tree_Widget::Tree_Widget(int x, int y, int w, int h, const char * label = 0)
  : Fl_Widget(x, y, w, h, label) {}

pair<int, int> Tree_Widget::show_tree(tree& cur_t, int depth, int left_bound) {
  vector<pair<int, int> > child_res;
  int curr_bound = left_bound;
  int new_depth = depth + 2 * h;
  
  for (int i = 0; i < cur_t.children.size(); ++i) {
    child_res.push_back(show_tree(cur_t.children[i], new_depth, curr_bound));
    curr_bound = child_res[i].first;
  }
  if (cur_t.children.size() == 0)
    curr_bound = left_bound + w + ALIGN;
  int d = (curr_bound - left_bound) / 2 + left_bound;
  
  for (int i = 0; i < cur_t.children.size(); ++i) {
    fl_color(FL_BLACK);
    fl_line(d, depth + h, child_res[i].second, new_depth);
  }

  int lx = d - w / 2;
  int rx = d + w / 2;
  int uy = depth;
  int dy = depth + h;

  fl_color(FL_WHITE);
  fl_polygon(lx, uy, lx, dy, rx, dy, rx, uy);

  fl_color(FL_BLACK);
  fl_draw(cur_t.s.c_str(), lx, uy, w, h, FL_ALIGN_INSIDE, NULL, 0);

  return make_pair(curr_bound, d);
}

void Tree_Widget::draw() {
  int l = t.count_leaves();
  int d = t.count_depth();

  h = HEIGHT / (2 * d - 1);
  w = WIDTH / l - 2 * ALIGN;
  fl_color(FL_BLACK);
  show_tree(t, 0, 0);
}

int tree::show(int argc, char** argv) {
  Fl_Window window(WIDTH, HEIGHT);
  Tree_Widget widg(0, 0, WIDTH, HEIGHT);
  widg.t = *this;

  window.end();
  window.show(argc, argv);
  return Fl::run();
}

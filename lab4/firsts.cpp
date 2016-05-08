#include "gen.h"
#include <iostream>
#include <algorithm>
#include <sstream>
using namespace std;


set<string> parsed_info::get_first(vector <token> terms) {
  set <string> ans;
  for (int i = 0; i < (int)terms.size(); i++) {
    token t = terms[i];
    if (t.is_token) {
      ans.insert(t.name);
      break;
    }
    for (auto j : first[t.name]) {
      ans.insert(j);
    }
    if (first[t.name].count("") == 0)
      break;
    if (i + 1 == (int)terms.size())
      break;
    ans.erase("");
  }
  if (terms.empty())
    ans.insert("");
  return ans;
}

bool parsed_info::add_firsts(string name, rule& r, int idx) {
  string a = r.terms[idx].name;
  bool changed = false;
  bool eps = false;

  if (r.terms[idx].is_token) {
    if (first[name].count(a) == 0) {
      first[name].insert(a);
      return true;
    }
    return false;
  } 
  for (auto f : first[a]) {
    if (f != "") {
      if (first[name].count(f) == 0) {
        changed = true;
        first[name].insert(f);
      }
    } else {
      eps = true;
    }
  }
  if (eps) {
    if (idx + 1 < (int)r.terms.size()) {
      changed |= add_firsts(name, r, idx + 1);
    } else {
      if (first[name].count("") == 0) {
        changed = true;
        first[name].insert("");
      }
    }
  }
  return changed;
}

void parsed_info::gen_first() {
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto g : (*grammar)) {
      string name = g.first;
      for (auto rule : g.second) {
        if (rule.terms.empty()) {
          if (first[name].count("") == 0) {
            changed = true;
            first[name].insert("");
          }
          continue;
        }
        changed |= add_firsts(name, rule, 0);
      }
    }
  }
}

void parsed_info::gen_follow() {
  follow["start"].insert("$"); 
  bool changed = true;

  while (changed) {
    changed = false;
    for (auto g : (*grammar)) {
      string name = g.first;
      
      for (auto rule : g.second) {
        for (int i = 0; i < (int)rule.terms.size(); ++i) {
          if (rule.terms[i].is_token) {
            continue;
          }
          int j = i + 1;
          for (j; j < (int)rule.terms.size(); ++j) {
            if (rule.terms[j].is_token)
              break;
            if (follow[rule.terms[j].name].count("") == 0) 
              break;
          }
          if (j == (int)rule.terms.size()) {
            for (auto elem : follow[name]) {
              if (follow[rule.terms[i].name].count(elem) == 0) {
                follow[rule.terms[i].name].insert(elem);
                changed = true;
              }
            }
          }
          if (i + 1 == (int)rule.terms.size()) {
            continue;
          }
          changed |= add_follows(rule.terms[i].name, rule, i + 1);
        }
      }
    }
  }
}

bool parsed_info::add_follows(string name, rule& r, int idx) {
  bool changed = false;
  bool eps = false;
  if (r.terms[idx].is_token) {
    if (follow[name].count(r.terms[idx].name) == 0) {
      follow[name].insert(r.terms[idx].name);
      changed = true;
    }
    return changed;
  }
  for (auto f : first[r.terms[idx].name]) {
    if (f != "") {
      if (follow[name].count(f) == 0) {
        follow[name].insert(f);
        changed = true;
      }
    } else {
      eps = true;
    }
  }
  if (eps) {
    if (idx + 1 < (int)r.terms.size())
      changed |= add_follows(name, r, idx + 1);
  }
  return changed;
}


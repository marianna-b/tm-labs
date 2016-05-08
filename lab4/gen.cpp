#include "gen.h"
#include <iostream>
#include <algorithm>
using namespace std;

token::token(string n) {
  name = n;
  is_token = true;
}

token::token(string n, vector<string> a) {
  name = n;
  is_token = false;
  arg = a;
  reverse(arg.begin(), arg.end());
}

rule::rule(vector<token> t, string c) {
  terms = t;
  reverse(terms.begin(), terms.end());
  code = c;
}

string rule::show() {
  string s = "";
  for (auto i : terms) {
    s += i.name + " [";
    for (auto j : i.arg) {
      s += j + " ";
    }
    s += "] ";
  }
  s += code;
  return s;
}

parsed_info::parsed_info(string* b, string* e, map<string, string>* tok, map<string, pair<string, vector<string> > >* nont, map<string, vector<rule> >* gr) {
  begin = b;
  end = e;
  token = tok;
  nonterm = nont;
  grammar = gr;
  gen_first();
  gen_follow();
}

string parsed_info::generate() {
  string s = "";
  s += *begin + "\n";
  s += *end + "\n";

  for (auto i : (*token)) 
    s += i.first + " " + i.second + "\n";
  for (auto i : (*nonterm)) {
    s += i.first + " " + i.second.first + "\n";
    s += "[";
    for (auto j : i.second.second)
      s += j + " ";
    s += "]\n";
  }
  for (auto i : (*grammar)) {
    s += i.first + "\n";
    for (auto j : i.second) 
      s += j.show() + "\n";
  }

  s += "\n\nfirst\n";

  for (auto i : first) {
    s += i.first + "\n";
    for (auto j : i.second) 
      if (j != "")
        s += j + "\n";
      else
        s += "eps\n"; 
  }

  s += "\nfollow\n";

  for (auto i : follow) {
    s += i.first + "\n";
    for (auto j : i.second) 
      s += j + "\n";
  }
  
  return s;
}

bool is_token(string a) {
  if (a == "")
    return false;
  if (a[0] <= 'Z' && a[0] >= 'A')
    return true;
  if (a == "$")
    return true;
  return false;
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

string parsed_info::generate_file() {
  return "";
}

string parsed_info::generate_header() {
  return "";
}
  
parsed_info::~parsed_info() {
  delete begin;
  delete end;
  delete token;
  delete nonterm;
  delete grammar;
}
 

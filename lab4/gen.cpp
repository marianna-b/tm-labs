#include "gen.h"
#include <iostream>
using namespace std;


rule::rule(vector<string> t, string c) {
  terms = t;
  code = c;
}

string rule::show() {
  string s = "";
  for (auto i : terms) {
    s += i + " ";
  }
  s += code;
  return s;
}

parsed_info::parsed_info(string* b, string* e, map<string, string>* tok, map<string, string>* nont, map<string, vector<rule> >* gr) {
  begin = b;
  end = e;
  token = tok;
  nonterm = nont;
  grammar = gr;
}

string parsed_info::generate() {
  string s = "";
  s += *begin;
  s += "\n\n";
  s += *end;
  s += "\n\n";

  for (auto i : (*token)) {
    s += i.first + " " + i.second + "\n";
  }
  s += "\n\n";
  for (auto i : (*nonterm)) {
    s += i.first + " " + i.second + "\n";
  }
  s += "\n\n";
  s += "\n\n";
  for (auto i : (*grammar)) {
    s += i.first + "\n";
    for (auto j : i.second) {
      s += j.show() + "\n";
    }
    s += "\n\n";
  }
  return s;
}

void parsed_info::generate(string, string) {
 
}
  
parsed_info::~parsed_info() {
  delete begin;
  delete end;
  delete token;
  delete nonterm;
  delete grammar;
}
 

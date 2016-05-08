#include "gen.h"
#include <iostream>
#include <algorithm>
#include <sstream>
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

parsed_info::parsed_info(string* b, string* e, map<string, pair<string, string> >* tok, map<string, pair<string, vector<string> > >* nont, map<string, vector<rule> >* gr) {
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
    s += i.first + " " + i.second.first + " " + i.second.second + "\n";
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

string to_str(int x) {
  ostringstream convert;
  convert << x;
  return convert.str();
}

string parsed_info::gen_enum() {
  string s = "";
  s += "enum token_symb {\n";
  int ans = 270;
  for (auto i : (*token)) {
    s += i.first + " = " + to_str(ans) + ",\n";
    ans--;
  }
  s += "};\n";
  return s;
}

string parsed_info::gen_func_list() {
  string s = "";
  for (auto i : (*nonterm)) {
    string name = i.first;
    string res = i.second.first;
    vector<string> arg = i.second.second;
    s += "  " + res.substr(2, (int)res.size() - 4) + " ";
    s += name + "(";
    for (int j = 0; j < arg.size(); j++) {
      s += arg[j].substr(2, (int)arg[j].size() - 4) + " _h" + to_str(j + 1);
      if (j + 1 < arg.size()) {
        s += ", ";
      }
    }
    s += ");\n";
  }
  return s;
}


string parsed_info::generate_header() {
  string s = "";
  s += "#ifndef PARSER_H\n";
  s += "#define PARSER_H\n";
  s += "#if ! defined(yyFlexLexerOnce)\n";
  s += "#include <FlexLexer.h>\n";
  s += "#endif\n\n";

  for (auto tok : (*token)) {
    string name = tok.second.second;
    string type = tok.second.first.substr(2, (int)tok.second.first.size() - 4);
    if (name != "" && type != "void") {
      s += "extern " + type + " " + name + ";\n";
    } 
  }
  
  s += "\n";
  s += gen_enum() + "\n";
  
  s += "struct parser {\n";
  s += "  FlexLexer* lexer = new yyFlexLexer();\n";
  s += "  token_symb curr;\n";
  s += "  parser()\n";
  s += "  ~parser()\n";

  s += gen_func_list() + "\n";

  s += "}\n";
  s += "#endif";
  return s;
}
  
parsed_info::~parsed_info() {
  delete begin;
  delete end;
  delete token;
  delete nonterm;
  delete grammar;
}
 

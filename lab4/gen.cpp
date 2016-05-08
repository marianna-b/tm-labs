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
  tokens = tok;
  nonterm = nont;
  grammar = gr;
  gen_first();
  gen_follow();
}

string parsed_info::generate() {
  string s = "";
  s += *begin + "\n";
  s += *end + "\n";

  for (auto i : (*tokens)) 
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

string to_str(int x) {
  ostringstream convert;
  convert << x;
  return convert.str();
}


string parsed_info::generate_file() {
  string s = "";

  s += "#include ";
  s += '"';
  s += "parser.h";
  s += '"';
  s += "\n";
  s += "\n";


  s += "\n";
  s += "\n";
  
  set<pair<string, string> > decl;
  for (auto tok : (*tokens)) {
    string name = tok.second.second;
    string type = tok.second.first.substr(2, (int)tok.second.first.size() - 4);
    if (name != "" && type != "void") {
      decl.insert(make_pair(name, type));
    } 
  }
  
  for (auto tok : decl) {
    s += tok.second + " " + tok.first + ";\n";
  }
  s += "\n";
  s += "extern int yylex();";
  s += "\n";
  s += "parser::parser(){\n";
  s += "  curr = yylex();\n";
  s += "}\n";
  s += "\n";
  s += "void parser::next(){\n";
  s += "  curr = yylex();\n";
  s += "}\n\n";

  for (auto r : (*grammar)) {
    s += gen_function(r.first) + "\n";
  }
  return s;
}

string gen_if(set<string> s) {
  string b = "if (";
  bool is_first = true;
  for (auto t : s) {
    if (!is_first) {
      b += " || ";
    } else {
      is_first = false;
    }
    if (t != "$")
      b += "(curr == " + t + ")";
    else
      b += "(curr == 0)";
  }
  b += ")";
  return b;
}

string parsed_info::gen_function(string name) {
  string s = "";
  string res = (*nonterm)[name].first;
  vector<string> arg = (*nonterm)[name].second;
  vector<rule> rules = (*grammar)[name];
  s += res.substr(2, (int)res.size() - 4) + " ";
  s += "parser::" + name + "(";
  for (int j = 0; j < arg.size(); j++) {
    s += "  ";
    s += arg[j].substr(2, (int)arg[j].size() - 4) + " _h" + to_str(j + 1);
    if (j + 1 < arg.size()) {
      s += ", ";
    }
  }
  s += ") {\n";
  s += "  " + res.substr(2, (int)res.size() - 4) + " res;";
  string eps_handling = "";
  for (int i = 0; i < (int)rules.size(); i++) {
    bool has_eps = false;
    set<string> f = get_first(rules[i].terms);
    if (f.count("") != 0) {
      has_eps = true;
      f.erase("");
    }
    
    string b = gen_if(f);
    string c = "";
    for (int j = 0; j < (int)rules[i].terms.size(); j++) {
      token t = rules[i].terms[j];
      if (t.is_token) {
        string type = (*tokens)[t.name].first.substr(2, (*tokens)[t.name].first.size() - 4);
        string name = (*tokens)[t.name].second;
        c += "    if (curr != " + t.name + ")\n";
        c += "      throw " + t.name + ";\n";
        if (type != "void") {
          c += "    ";
          c += type + " _s" + to_str(j + 1) + " = " + name + ";\n";
        }
        c += "    ";
        c += "next();\n";
      } else {
        string res = (*nonterm)[t.name].first.substr(2, (*nonterm)[t.name].first.size() - 4);
        vector <string> arg = t.arg;
        if (res != "void") {
          c += "    ";
          c += res + " _s" + to_str(j + 1) + " = " + t.name + "(";
        } else {
          c += "    ";
          c += t.name + "(";
        } 
        for (int k = 0; k < (int)arg.size(); ++k) {
          if (k + 1 < (int)arg.size()) {
            c += arg[k] + ", ";
          } else {
            c += arg[k];
          }
        }
        c +=  ");\n";
      }
    }
    c += "\n    " + rules[i].code.substr(1, rules[i].code.size() - 2);
    c += "\n    return res;";
    if (has_eps) {
      eps_handling = c;
    }
    if (!f.empty()) {
      s += "\n  " + b + " {\n" + c + "\n  }\n";
    }

  }
  if (eps_handling.size() > 0) {
    string a = gen_if(follow[name]);
    s += "\n  " + a + " {\n" + eps_handling + "\n  }\n";
  }
  s += "  throw curr;\n";
  s += "}\n";
  return s;
}

string parsed_info::gen_enum() {
  string s = "";
  s += "enum token_symb {\n";
  int ans = 270;
  for (auto i : (*tokens)) {
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
  s += (*begin).substr(2, (*begin).size() - 4);
  
  for (auto tok : (*tokens)) {
    string name = tok.second.second;
    string type = tok.second.first.substr(2, (int)tok.second.first.size() - 4);
    if (name != "" && type != "void") {
      s += "extern " + type + " " + name + ";\n";
    } 
  }
  
  s += "\n";
  s += gen_enum() + "\n";
  
  s += "struct parser {\n";
  s += "  int curr;\n";
  s += "  parser();\n";
  s += "  void next();\n";

  s += gen_func_list() + "\n";

  s += "};\n";
  s += "#endif";
  return s;
}
  
parsed_info::~parsed_info() {
  delete begin;
  delete end;
  delete tokens;
  delete nonterm;
  delete grammar;
}
 

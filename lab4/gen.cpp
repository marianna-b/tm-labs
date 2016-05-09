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

parsed_info::parsed_info(string* b, string* e, map<string, pair<string, string> >* tok, map<string, pair<string, vector<string> > >* nont, map<string, vector<rule> >* gr) {
  begin = b;
  end = e;
  tokens = tok;
  nonterm = nont;
  grammar = gr;
  gen_first();
  gen_follow();
  gen_token_decl();
}

string to_str(int x) {
  ostringstream convert;
  convert << x;
  return convert.str();
}

string header() {
  string s = "";

  s += "#include ";
  s += '"';
  s += "parser.h";
  s += '"';
  s += "\n";
  s += "\n";
  
  return s;
}

string parser_body() {
  string s = "";
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
  return s;
}

string parsed_info::generate_file() {
  string s = header();
  // Global variables for lexer
  for (auto tok : token_decl) {
    s += tok.second + " " + tok.first + ";\n";
  }
  // Some parser functions
  s += parser_body();
  // Functions for each nonterminal
  for (auto r : (*grammar)) {
    s += gen_function(r.first) + "\n";
  }
  // Ending code
  if ((int)end->size() > 3)
    s += (*end).substr(2, (int)(*end).size() - 4);
  return s;
}

string parsed_info::generate_header() {
  string s = "";
  s += "#ifndef PARSER_H\n";
  s += "#define PARSER_H\n";
  // Beginning code
  if ((int)begin->size() > 3)
    s += (*begin).substr(2, (*begin).size() - 4);
  s += "\n";
  // Externing global variables for lexer
  for (auto tok : token_decl) {
    s += "extern " + tok.second + " " + tok.first + ";\n";
  }
  
  s += "\n";
  // Enum for lexer
  s += gen_enum() + "\n";

  // Struct for parser (function for nonterminals declarations)
  s += gen_parse_struct();
  s += "#endif";
  return s;
}

// Generating check for current "symbol"
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

// Declarations for functions for nonterminals
string parsed_info::gen_func_list() {
  string s = "";
  for (auto i : (*nonterm)) {
    string name = i.first;
    string res = i.second.first;
    vector<string> arg = i.second.second;

    s += "  " + res.substr(2, (int)res.size() - 4) + " ";
    s += name + "(";

    for (int j = 0; j < arg.size(); j++) {
      s += arg[j].substr(2, (int)arg[j].size() - 4);
      if (j + 1 < arg.size())
        s += ", ";
    }
    s += ");\n";
  }
  return s;
}


string parsed_info::gen_parse_struct() {
  string s = "";
  s += "struct parser {\n";
  s += "  int curr;\n";
  s += "  parser();\n";
  s += "  void next();\n";
  s += gen_func_list() + "\n";
  s += "};\n";
  return s;
}

string token_handling(string name, string tname, string type, int j) {
  string c = "";
  // Check if correct token
  c += "    if (curr != " + tname + ")\n";
  c += "      throw " + tname + ";\n";
  // Check if has attribute
  if (type != "void") {
    c += "    ";
    // Declare a variable and copy attribute value from lexers global
    c += type + " _s" + to_str(j + 1) + " = " + name + ";\n";
  }
  c += "    ";
  // Move to next token
  c += "next();\n";
  return c;
}

string nonterm_handling(string res, int j, string tname, vector <string> arg) {
  string c = "";
  // Check if has synt attribute and assign result of function to variable if so
  if (res != "void") {
    c += "    ";
    c += res + " _s" + to_str(j + 1) + " = " + tname + "(";
  } else {
    c += "    ";
    c += tname + "(";
  }
  // Add list of arguments
  for (int k = 0; k < (int)arg.size(); ++k) {
    if (k + 1 < (int)arg.size()) {
      c += arg[k] + ", ";
    } else {
      c += arg[k];
    }
  }
  c +=  ");\n";
  return c;
}

string parsed_info::gen_function(string name) {
  string s = "";
  string res = (*nonterm)[name].first;
  vector<string> arg = (*nonterm)[name].second;
  vector<rule> rules = (*grammar)[name];
  s += res.substr(2, (int)res.size() - 4) + " ";
  // Generating function header
  s += "parser::" + name + "(";
  for (int j = 0; j < (int)arg.size(); j++) {
    s += "  ";
    s += arg[j].substr(2, (int)arg[j].size() - 4) + " _h" + to_str(j + 1);
    if (j + 1 < (int)arg.size()) {
      s += ", ";
    }
  }
  s += ") {\n";

  // Declare variable for synt attributes
  if (res != "%<void%>")
    s += "  " + res.substr(2, (int)res.size() - 4) + " res;";

  // Declaring if clauses for different rules
  for (int i = 0; i < (int)rules.size(); i++) {
    // Check if first contains eps
    set<string> f = get_first(rules[i].terms);
    if (f.count("") != 0) {
      f.erase("");
      // Add follows to set of tokens if contains eps
      for (auto elem : follow[name]) {
        f.insert(elem);
      }
    }
    
    // Generate if statement 
    string b = gen_if(f);
    string c = "";
    
    for (int j = 0; j < (int)rules[i].terms.size(); j++) {
      token t = rules[i].terms[j];
      if (t.is_token) {
        string type = (*tokens)[t.name].first.substr(2, (*tokens)[t.name].first.size() - 4);
        string name = (*tokens)[t.name].second;

        // Handling a token in rule
        c += token_handling(name, t.name, type, j);
      } else {
        string res = (*nonterm)[t.name].first.substr(2, (*nonterm)[t.name].first.size() - 4);
        vector <string> arg = t.arg;

        // Handling nonterminal in a rule by invoking function for it 
        c += nonterm_handling(res, j, t.name, arg);
      }
    }
    
    // Add attribute computation
    c += "\n    " + rules[i].code.substr(2, rules[i].code.size() - 4);

    // Return statement
    if (res == "%<void%>")
      c += "\n    return;";
    else
      c += "\n    return res;";

    // Check if set of tokens is not empty
    if (!f.empty()) {
      s += "\n  " + b + " {\n" + c + "\n  }\n";
    }

  }
  // Throwing exception if no rules match
  s += "  throw curr;\n";
  s += "}\n";
  return s;
}

parsed_info::~parsed_info() {
  delete begin;
  delete end;
  delete tokens;
  delete nonterm;
  delete grammar;
}
 

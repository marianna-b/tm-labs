#include "gen.h"
#include <iostream>
#include <sstream>
using namespace std;
extern string to_str(int);

string parsed_info::verify() {

  if (grammar->count("start") == 0) {
    return "start nonterminal is not declared";
  }
  
  for (auto g : (*grammar)) {
    string name = g.first;
    vector<rule> terms = g.second;

    if (nonterm->count(name) == 0) {
      return "Nonterminal is not declared: " + name;
    }
    for (auto r : terms) {
      for (int i = 0; i < (int)r.terms.size(); ++i) {
        token t = r.terms[i];
        if (t.is_token) {
          if (tokens->count(t.name) == 0) {
            return "Token is not declared: " + t.name;
          }
        } else {
          if (nonterm->count(t.name) == 0) {
            return "Nonterminal is not declared: " + t.name;
          }
          if (grammar->count(t.name) == 0) {
            return "Nonterminal does not have a rule declared: " + t.name;
          }
        }

        if (nonterm->count(t.name) != 0) {
          if ((*nonterm)[t.name].second.size() < t.arg.size()) {
            return "Too much arguments for " + t.name + " in " + name;
          }
          if ((*nonterm)[t.name].second.size() > t.arg.size()) {
            return "Not enough arguments for " + t.name + " in " + name;
          }
        } else {
          if (t.arg.size() != 0) {
            return "Too much arguments for " + t.name + " in " + name;
          }
        }
        
        int harg = (*nonterm)[name].second.size();
        
        for (int j = 0; j < (int)t.arg.size(); ++j) {
          string carg = t.arg[j];
          if (carg == "") continue;
          
          int num = stoi(carg.substr(2, (int)carg.size() - 2));
          if (carg[1] == 'h') {
            if (num > harg)
              return "Invalid argument: " + name + " doesn't have " + carg + " argument";
          } else {
            if (i < num)
              return "Invalid argument: " + carg + " doesn't exist yet in " + name + " rule when " + t.name; 
            token w = r.terms[num - 1];
            if (w.is_token) {
              if ((*tokens)[w.name].first == "%<void%>")
                return "In " + name + " argument " + carg + " in " + t.name + " doesn't have a type";
            } else {
              if ((*nonterm)[w.name].first == "%<void%>")
                return "In " + name + " argument " + carg + " in " + t.name + " doesn't have a type";
            }
          }
          }
      }
    }
  }
  /*
  for (auto g : (*grammar)) {
    string name = g.first;
    cout << name << endl;
    for (int i = 0; i < (int)g.second.size(); i++) {
      cout << i << endl;
      set<string> f1 = get_first(g.second[i].terms);
      for (auto elem : f1)
        cout << elem << " ";
      cout << endl;
    }
   }
*/  

  for (auto g : (*grammar)) {
    string name = g.first;
    for (int i = 0; i < (int)g.second.size(); i++) {
      for (int j = i + 1; j < (int)g.second.size(); ++j) {
        set<string> f1 = get_first(g.second[i].terms);
        set<string> f2 = get_first(g.second[j].terms);
        for (auto elem : f1) {
          if (f2.count(elem) != 0) {
            return "FIRSTs have non-empty intersection in: " + name + ", rules " + to_str(i + 1) + " and " + to_str(j + 1);
          }
        }
        if (f1.count("") != 0) {
          for (auto elem : follow[name]) {
            if (f2.count(elem) != 0) {
              string s = "FOLLOW for " + name + " has non-empty intersection with FIRST for rule " + to_str(i + 1);
              s += "\nCause: eps in rule " + to_str(j + 1);
              return s;
            }
          }
        }
        if (f2.count("") != 0) {
          for (auto elem : follow[name]) {
            if (f1.count(elem) != 0) {
              string s = "FOLLOW for: " + name + " has non-empty intersection with FIRST for rule " + to_str(j + 1);
              s += "\nCause: eps in rule " + to_str(j + 1);
              return s;
            }
          }
        }
      }
    }
  }
  
  return "";
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

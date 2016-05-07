#ifndef GEN_H
#define GEN_H

#include <vector>
#include <string>
#include <map>
#include <set>

struct rule {
  std::vector<std::string> terms;
  std::string code;
  std::string show();
  rule(std::vector<std::string>, std::string);
};

struct parsed_info {
  parsed_info(std::string*, std::string*, std::map<std::string, std::string>*, std::map<std::string, std::string>*, std::map<std::string, std::vector<rule> >*);

  std::string generate();
  std::string generate_file();
  std::string generate_header();
  
  ~parsed_info();
private:
  std::string *begin, *end;
  std::map<std::string, std::string> *token, *nonterm;
  std::map<std::string, std::vector<rule> > *grammar;
  std::map<std::string, std::set<std::string> > first;
  std::map<std::string, std::set<std::string> > follow;
  void gen_first();
  void gen_follow();
  bool add_firsts(std::string, rule&, int);
  bool add_follows(std::string, rule&, int);
};
 

#endif // GEN_H

#ifndef GEN_H
#define GEN_H

#include <vector>
#include <string>
#include <map>
#include <set>

struct token {
  bool is_token;
  std::string name;
  std::vector<std::string> arg;

  token(std::string);
  token(std::string, std::vector<std::string>);
  
};

struct rule {
  std::vector<token> terms;
  std::string code;
  std::string show();
  rule(std::vector<token>, std::string);
};

struct parsed_info {
  parsed_info(std::string*, std::string*, std::map<std::string, std::pair<std::string, std::string> >*, std::map<std::string, std::pair<std::string, std::vector<std::string> > >*, std::map<std::string, std::vector<rule> >*);

  std::string generate();
  std::string generate_file();
  std::string generate_header();
  std::string verify();
  
  ~parsed_info();
private:
  std::string *begin, *end;
  std::map<std::string, std::pair<std::string, std::string>> *tokens;
  std::map<std::string, std::pair<std::string, std::vector<std::string> > > *nonterm;
  std::map<std::string, std::vector<rule> > *grammar;
  std::map<std::string, std::set<std::string> > first;
  std::map<std::string, std::set<std::string> > follow;
  std::string gen_enum();
  std::string gen_func_list();
  std::string gen_function(std::string);
  void gen_first();
  void gen_follow();
  bool add_firsts(std::string, rule&, int);
  bool add_follows(std::string, rule&, int);
  std::set<std::string> get_first(std::vector <token>);
};
 

#endif // GEN_H

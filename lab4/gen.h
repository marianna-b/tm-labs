#ifndef GEN_H
#define GEN_H

#include <vector>
#include <string>
#include <map>

struct rule {
  std::vector<std::string> terms;
  std::string code;
  std::string show();
  rule(std::vector<std::string>, std::string);
};

struct parsed_info {
  parsed_info(std::string*, std::string*, std::map<std::string, std::string>*, std::map<std::string, std::string>*, std::map<std::string, std::vector<rule> >*);

  void generate(std::string, std::string);
  std::string generate();
  
  ~parsed_info();
private:
  std::string *begin, *end;
  std::map<std::string, std::string> *token, *nonterm;
  std::map<std::string, std::vector<rule> > *grammar;
};
 

#endif // GEN_H

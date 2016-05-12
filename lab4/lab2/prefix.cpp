#include "parser.h"

std::string _STR;

extern int yylex();
parser::parser(){
  curr = yylex();
}

void parser::next(){
  curr = yylex();
}

std::vector<std::pair<std::string, std::string> >  parser::decl() {
  std::vector<std::pair<std::string, std::string> >  res;
  if ((curr == VAR)) {
    if (curr != VAR)
      throw VAR;
    std::string _s1 = _STR;
    next();
    std::vector<std::pair<std::string, std::string> >  _s2 = list(_s1);
    if (curr != SEMICOLON)
      throw SEMICOLON;
    next();

     res = _s2; 
    return res;
  }
  throw curr;
}

std::vector<std::pair<std::string, std::string> >  parser::list(  std::string _h1) {
  std::vector<std::pair<std::string, std::string> >  res;
  if ((curr == DEREF) || (curr == VAR)) {
    std::pair<std::string, std::string> _s1 = var(_h1);
    std::vector<std::pair<std::string, std::string> >  _s2 = list_end(_h1);

     res = _s2; std::cout << _s1.first << " : " << _s1.second << std::endl; 
    return res;
  }
  throw curr;
}

std::vector<std::pair<std::string, std::string> >  parser::list_end(  std::string _h1) {
  std::vector<std::pair<std::string, std::string> >  res;
  if ((curr == COMMA)) {
    if (curr != COMMA)
      throw COMMA;
    next();
    std::vector<std::pair<std::string, std::string> >  _s2 = list(_h1);

    res = _s2; 
    return res;
  }

  if ((curr == SEMICOLON)) {

     res = std::vector<std::pair<std::string, std::string> >(); 
    return res;
  }
  throw curr;
}

std::vector<std::pair<std::string, std::string> >  parser::start() {
  std::vector<std::pair<std::string, std::string> >  res;
  if ((curr == VAR)) {
    std::vector<std::pair<std::string, std::string> >  _s1 = decl();
    std::vector<std::pair<std::string, std::string> >  _s2 = start();

     res = _s1; res.insert(res.end(), _s2.begin(), _s2.end());  
    return res;
  }

  if ((curr == 0)) {

     res = std::vector<std::pair<std::string, std::string> >();  
    return res;
  }
  throw curr;
}

std::pair<std::string, std::string> parser::var(  std::string _h1) {
  std::pair<std::string, std::string> res;
  if ((curr == DEREF)) {
    if (curr != DEREF)
      throw DEREF;
    next();
    std::pair<std::string, std::string> _s2 = var(_h1);

     res = std::pair<std::string, std::string>(_s2.first + "*", _s2.second); 
    return res;
  }

  if ((curr == VAR)) {
    if (curr != VAR)
      throw VAR;
    std::string _s1 = _STR;
    next();

     res = std::pair<std::string, std::string>(_h1, _s1); 
    return res;
  }
  throw curr;
}


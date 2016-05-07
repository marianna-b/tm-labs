%{
    #include <iostream>
    #include <unistd.h>
    #include "gen.h"
    parsed_info* result;
    extern int yylex();
    void yyerror(const char *s) { std::cout << "ERROR: " << s << std::endl; }
%}

%union {
    std::string *string;
    std::map<std::string, std::string> *types;
    std::map<std::string, std::vector<rule> > *gram_type;
    std::vector<std::string> *list;
    std::vector<rule> *rulelist;
}

%token <string> NONTERM TOKEN TYPE_NAME INSERT CODE 
%token TYPE TOKEN_SYMB SEP OR SEMICOLON COLON

%type <gram_type> grammar
%type <list> term_list
%type <string> insert 
%type <types> token_list
%type <types> nonterm_list
%type <rulelist> rule_list

%start start 

%%

start:
     INSERT token_list nonterm_list SEP grammar SEP insert { result = new parsed_info(($1), ($7), ($2), ($3), ($5)); }
;

insert:
    { $$ = new std::string(""); }
|   INSERT { $$ = $1; }

token_list:
    { $$ = new std::map<std::string, std::string>(); }
|   TOKEN_SYMB TYPE_NAME TOKEN token_list { ($4)->insert(std::pair<std::string, std::string>(*($3), *($2))); $$ = $4; delete ($2); delete ($3); }

nonterm_list:
    { $$ = new std::map<std::string, std::string>(); } 
|   TYPE TYPE_NAME NONTERM nonterm_list { ($4)->insert(std::pair<std::string, std::string>(*($3), *($2))); $$ = $4; delete ($2); delete ($3); } 
;

grammar:
    { $$ = new std::map<std::string, std::vector<rule>>(); }
|   NONTERM COLON rule_list SEMICOLON grammar { ($5)-> insert(std::pair<std::string, std::vector<rule> >(*($1), *($3))); $$ = $5; delete ($1); delete ($3); }
;

rule_list:
    { $$ = new std::vector<rule>(); }
|   term_list CODE OR rule_list { ($4)->push_back(rule(*($1), *($2))); $$ = $4; delete ($1); delete ($2); }
;

term_list:
    { $$ = new std::vector<std::string>(); }
|   NONTERM term_list { ($2)->push_back(*($1)); $$ = $2; delete ($1); }
|   TOKEN term_list { ($2)->push_back(*($1)); $$ = $2; delete ($1); }
;

%%

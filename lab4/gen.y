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
    std::map<std::string, std::pair<std::string, std::vector<std::string> > > *nonterms;
    std::map<std::string, std::vector<rule> > *gram_type;
    std::vector<std::string> *list;
    std::vector<token> *tokens;
    std::vector<rule> *rulelist;
}

%token <string> NONTERM TOKEN TYPE_NAME INSERT CODE ARG
%token TYPE TOKEN_SYMB SEP OR SEMICOLON COLON LEFTBR RIGHTBR

%type <gram_type> grammar
%type <tokens> term_list
%type <list> arg_list
%type <list> args
%type <list> type_list
%type <string> insert 
%type <types> token_list
%type <nonterms> nonterm_list
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
    { $$ = new std::map<std::string, std::pair<std::string, std::vector<std::string> > >(); } 
|   TYPE TYPE_NAME NONTERM LEFTBR type_list RIGHTBR nonterm_list { ($7)->insert(std::pair<std::string, std::pair<std::string, std::vector<std::string> > > (*($3), std::pair<std::string, std::vector<std::string> > (*($2), *($5)))); $$ = $7; delete ($2); delete ($3); } 
;

type_list:
    { $$ = new std::vector<std::string>(); }
|   TYPE_NAME type_list { ($2)->push_back(*($1)); $$ = $2; delete ($1); }
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
    { $$ = new std::vector<token>(); }
|   NONTERM args term_list { ($3)->push_back(token(*($1), *($2))); $$ = $3; delete ($1); delete ($2); }
|   TOKEN term_list { ($2)->push_back(token(*($1))); $$ = $2; delete ($1); }
;

args:
    { $$ = new std::vector<std::string>(); }
|   LEFTBR ARG arg_list RIGHTBR { ($3)->push_back(*($2)); $$ = $3; delete ($2); }
;

arg_list:
    { $$ = new std::vector<std::string>(); }
|   ARG arg_list { ($2)->push_back(*($1)); $$ = $2; delete ($1); }
;
%%

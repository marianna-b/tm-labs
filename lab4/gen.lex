%{
#include "gen.h"
#include <iostream>
#include <unistd.h>
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
extern "C" int yywrap() { }
%}

%%

[ \t\n]                 ;
"%type"                 return TYPE;
"%token"                return TOKEN_SYMB;
"%%"                    return SEP;
"|"                     return OR;
";"                     return SEMICOLON;
":"                     return COLON;
"["                     return LEFTBR;
"]"                     return RIGHTBR;
[a-z][a-z0-9_]*         SAVE_TOKEN; return NONTERM;
[A-Z][A-Z0-9_]*         SAVE_TOKEN; return TOKEN;
_(h|s)[0-9]+            SAVE_TOKEN; return ARG;
_[A-Z][A-Z0-9_]*        SAVE_TOKEN; return GLOB;
"%<"[^%]*"%>"           SAVE_TOKEN; return TYPE_NAME;
"%{"([^%]|[\t\n])*"%}"  SAVE_TOKEN; return INSERT;
"{"([^}]|[\n])*"}"      SAVE_TOKEN; return CODE;
.                       std::cout << "Unknown token! " << std::string(yytext, yyleng); yyterminate();                     

%%
 

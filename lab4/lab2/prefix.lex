%{
#include <string>
#include <iostream>
#include <unistd.h>
#include "parser.h"
#define SAVE_TOKEN result = new std::string(yytext, yyleng)
%}

%option noyywrap
%%


[ \t\n]                 ;
[a-zA-Z][a-zA-Z0-9_]*   _STR = std::string(yytext, yyleng); return VAR;
","                     return COMMA;
"*"                     return DEREF;
";"                     return SEMICOLON;
<<EOF>>                 return 0;
.                       printf("Unknown token! %s", std::string(yytext, yyleng)); yyterminate();
%%



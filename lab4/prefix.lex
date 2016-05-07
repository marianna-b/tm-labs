%{
#include <string>
#include <iostream>
#include <unistd.h>
#include "tokens.h"
#define SAVE_TOKEN result = new std::string(yytext, yyleng)
 extern token t;
 extern int yyFlexLexer::yywrap() { }
%}

digit [0-9]
character [a-zA-Z]
int_const {digit}+
string_const {character}+

%%

[ \t\n]                 ;
"ifelse"                t = token(token_type::ifelse); return 1;
"print"                 t = token(token_type::print);  return 1;
[a-zA-Z_][a-zA-Z0-9_]*  t = token(token_type::var, std::string(yytext, yyleng)); return 1;
[0-9]+                  t = token(token_type::integer, std::string(yytext, yyleng)); return 1;
.                       printf("Unknown token! %s", std::string(yytext, yyleng)); yyterminate();

%%



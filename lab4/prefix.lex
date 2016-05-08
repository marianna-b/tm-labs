%{
#include <string>
#include <iostream>
#include <unistd.h>
#include "parser.h"
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
[a-zA-Z][a-zA-Z0-9_]*   ;
.                       printf("Unknown token! %s", std::string(yytext, yyleng)); yyterminate();

%%



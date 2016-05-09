%{
#include <string>
#include <iostream>
#include "parser.h"
#define SAVE_TOKEN _STR = std::string(yytext, yyleng)
extern "C" int yywrap() { }
%}

digit [0-9]
character [a-zA-Z]
int_const {digit}+
string_const {character}+

%%

[ \t\n]                 ;
"if"                    return IF;
"ifelse"                return IFELSE;
"print"                 return PRINT;
"false"                 return FALSE;
"true"                  return TRUE;
[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return VARIABLE;
[0-9]+                  SAVE_TOKEN; return INTEGER;
"^"                     return EXP;
"="                     return ASSIGN;
"=="                    return EQ;
"!="                    return NEQ;
"<"                     return LT;
">"                     return GT;
"+"                     return PLUS;
"-"                     return MINUS;
"*"                     return MUL;
"/"                     return DIV;
"!"                     return NOT;
"&&"                    return AND;
"||"                    return OR;
.                       std::cout << "Unknown token! " << std::string(yytext, yyleng) << std::endl; yyterminate();

%%

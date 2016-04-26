%{
    #include <iostream>
    #include <string>
    #include <unistd.h>
    std::string* result;
    extern int yylex();
    void yyerror(const char *s) { std::cout << "ERROR: " << s << std::endl; }
%}

%union {
    std::string *string;
}



%token <string> VARIABLE INTEGER 
%token EQ NEQ LT GT ASSIGN IF IFELSE PRINT FALSE TRUE
%token PLUS MINUS MUL DIV NOT AND OR

%type <string> expr
%type <string> int_expr
%type <string> bool_expr

%start start 

%glr-parser
%%

start:
    expr { result = $1; }
;

expr:
    IF bool_expr expr { $$ = new std::string("if " + *($2) + "{\n" + *($3)  + "\n}"); delete $2; delete $3; } 
|   IFELSE bool_expr expr expr { $$ = new std::string("if " + *($2) + " {\n" + *($3)  + "\n}" + " else { \n" + *($4) + "\n}"); delete $2; delete $3; delete $4; } 
|   ASSIGN VARIABLE int_expr { $$ = new std::string(*($2) + " := " + *($3)); delete $2; delete $3; } 
|   PRINT int_expr { $$ = new std::string("print " + *($2)); delete $2; } 
;

int_expr:
    INTEGER { $$ = $1; }
|   MINUS int_expr expr { $$ = new std::string(*($2) + " - " + *($3)); delete $2; delete $3; }
|   PLUS int_expr int_expr { $$ = new std::string("(" + *($2) + " + " + *($3) + ")"); delete $2; delete $3; }
|   MUL int_expr int_expr { $$ = new std::string("(" + *($2) + " * " + *($3) + ")"); delete $2; delete $3; }
|   DIV int_expr int_expr { $$ = new std::string("(" + *($2) + " / " + *($3) + ")"); delete $2; delete $3; }
|   VARIABLE { $$ = $1; }
;

bool_expr:
TRUE { $$ = new std::string("true"); }
|   FALSE { $$ = new std::string("false"); }
|   NOT bool_expr { $$ = new std::string("not " + *($2));}
|   AND bool_expr bool_expr { $$ = new std::string("(" + *($2) + " and " + *($3) + ")"); delete $2; delete $3; }
|   OR bool_expr bool_expr { $$ = new std::string("(" + *($2) + " or " + *($3) + ")"); delete $2; delete $3; }
|   GT int_expr int_expr { $$ = new std::string("(" + *($2) + " > " + *($3) + ")"); delete $2; delete $3; }
|   LT int_expr int_expr { $$ = new std::string("(" + *($2) + " < " + *($3) + ")"); delete $2; delete $3; }
|   EQ int_expr int_expr { $$ = new std::string("(" + *($2) + " = " + *($3) + ")"); delete $2; delete $3; }
|   NEQ int_expr int_expr { $$ = new std::string("(" + *($2) + " != " + *($3) + ")"); delete $2; delete $3; }
;

%%

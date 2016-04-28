#ifndef PARSER_H
#define PARSER_H

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

class Parser : public yyFlexLexer{
public:
   
   Parser(std::istream *in) : yyFlexLexer(in),
                                  yylval( nullptr ){};
   
   int yylex(Parser::semantic_type *lval)
   {
      yylval = lval;
      return( yylex() ); 
   }
   

private:
   int yylex();
   Parser::semantic_type *yylval;
};


#endif /* */

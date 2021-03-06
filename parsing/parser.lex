%{

#include "parser.tab.hpp"
#include "global.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstring>

extern int yylex();


%}
%option noyywrap

comment     --[^\n]*\n

blanks      [ \t\r\n]+

digit       [0-9]

letter      [a-zA-Z]

letter_or_digit [a-zA-Z0-9]

constant     {digit}+

name2        {letter_or_digit}+

name         {letter}+{name2}?

variable     ${name}

%%

"SOURCE" {return SOURCE;}
"TARGET" {return TARGET;}
"MAPPING" {return MAPPING;}
"(" {return LPAR;}
")" {return RPAR;}
"," {return COMMA;}
"->" {return ARROW;}
"." {return DOT;}

{constant} {
	yylval.number = atoi(yytext); return CONSTANT;
}
{variable} {
	yylval.string = strdup(yytext); return VARIABLE;
}

{name} {
	yylval.string = strdup(yytext); return NAME;
}

{blanks} {}
{comment} {}

. ;


%%



int yylex() {
	static yyFlexLexer fl;
	return fl.yylex();
}



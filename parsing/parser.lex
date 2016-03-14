%{

#include "parser.tab.h"
#include "global.h"

#include <stdlib.h>
#include <stdio.h>

extern int yylex();

%}
%option noyywrap

comment     --[^\n]*\n

blanks      [ \t\r\n]+

digit       [0-9]

letter      [a-zA-Z]

letter_or_digit [{letter}{digit}]

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
	yylval=atoi(yytext);
	return CONSTANT;
}
{variable} {
	return VARIABLE;
}

{name} {
	return NAME;
}

{blanks} {}
{comment} {}

. ;


%%

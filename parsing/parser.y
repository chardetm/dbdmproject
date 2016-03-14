%{

#include "global.h"
#include <stdio.h>
#include <stdlib.h>

extern int yyparse();
extern FILE* cnfin;

/*
extern void reportError(char *s);
extern void reportError2();
extern void init(long long v, long long c);
extern void newClause();
extern void addLit(long long lit);
extern void addClause();
extern void reportClauseVide();
*/


%}

%token SOURCE TARGET MAPPING LPAR RPAR COMMA ARROW DOT CONSTANT VARIABLE NAME

%start Start
%%

Start:
	SOURCE Schema TARGET Schema MAPPING Tgds
	{
		// Du code ($1 -> $n)
	}
;
Schema:
	Schema Relation
	{
		// Du code
	}
|
	Relation
	{
		// Du code
	}
;
Relation:
	NAME LPAR Atts RPAR
	{
		// Du code
	}
;
Atts:
	Atts COMMA NAME
	{
		// Du code
	}
|
	NAME
;
Tgds:
	Tgds Tgd
	{
		// Du code
	}
|
	Tgd
	{
		// Du code
	}
;
Tgd:
	Query ARROW Query DOT
	{
		// Du code
	}
;
Query:
	Query COMMA Atom
	{
		// Du code
	}
|
	Atom
	{
		// Du code
	}
;
Atom:
	NAME LPAR Args LPAR
	{
		// Du code
	}
;
Args:
	Args COMMA Value
	{
		// Du code
	}
|
	Value
	{
		// Du code
	}
;
Value:
	VARIABLE
	{
		// Du code
	}
|
	CONSTANT
	{
		// Du code
	}
;

%%

int yyerror(char *s) {
	// Du code
	printf("Error: %s\n", s);
}
